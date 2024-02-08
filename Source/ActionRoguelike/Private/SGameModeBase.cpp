// 2023 Jorge Sánchez, All rights reserved

#include "SGameModeBase.h"

#include "EngineUtils.h"
#include "SAttributeComponent.h"
#include "Scharacter.h"
#include "SGameplayInterface.h"
#include "SPlayerState.h"
#include "SSaveGame.h"
#include "AI/SAICharacter.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("su.SpawnBots"), true, TEXT("Enable spawning of bots via timer."), ECVF_Cheat);

ASGameModeBase::ASGameModeBase()
{
	SpawnTimerInterval = 5.0f;
	bAISpawnDisabled = false;

	SlotName = "SaveGame01";
}

void ASGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	// LoadSaveGame();
}

void ASGameModeBase::StartPlay()
{
	Super::StartPlay();

	LoadSaveGame();
	
	if(bAISpawnDisabled)
	{
		return;
	}
 
	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &ASGameModeBase::SpawnBotTimerElapsed, SpawnTimerInterval, true);

	// TODO(Jsanchez): Spawn pickups using EQS. Create an EQS, run it here and use the results to spawn pickups in random positions. Lecture 18 first video
}

void ASGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);

	ASPlayerState *PS = NewPlayer->GetPlayerState<ASPlayerState>();
	if(PS)
	{
		PS->LoadPlayerState(CurrentSaveGame);
	}
}

void ASGameModeBase::OnActorKilled(AActor* VictimActor, AActor* Killer)
{
	AScharacter *Player = Cast<AScharacter>(VictimActor);
	if(Player)
	{
		FTimerHandle TimerHandle_RespawnDelay;
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "RespawnPlayerElapsed", Player->GetController());

		float RespawnDelay = 2.0f;
		GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay, Delegate, RespawnDelay, false);
	}

	{ // Add Credits if player killed a minion
		ASAICharacter *VictimMinion = Cast<ASAICharacter>(VictimActor);
		AScharacter *KillerPlayerCharacter = Cast<AScharacter>(Killer);
		if(VictimMinion && KillerPlayerCharacter)
		{
			ASPlayerState *PlayerState = Cast<ASPlayerState>(KillerPlayerCharacter->GetPlayerState());
			if(PlayerState)
			{
				PlayerState->AddCredits(VictimMinion->CreditWorthAmount);
			}
		}
	}
}

void ASGameModeBase::RespawnPlayerElapsed(AController* Controller)
{
	if(ensure(Controller))
	{
		Controller->UnPossess();
		RestartPlayer(Controller);
	}
}

void ASGameModeBase::KillAllAI()
{
	for(TActorIterator<ASAICharacter> It(GetWorld()); It; ++It)
	{
		ASAICharacter *Bot = *It;

		USAttributeComponent *AttributeComp =  USAttributeComponent::GetAttributes(Bot);
		if(ensure(AttributeComp) && AttributeComp->IsAlive())
		{
			AttributeComp->Kill(this); // TODO: pass in player? for kill credit
		}
	}
}


void ASGameModeBase::SpawnBotTimerElapsed()
{
	if(!CVarSpawnBots.GetValueOnGameThread())
	{
		UE_LOG(LogTemp, Warning, TEXT("Bot spawning disabled via cvar 'CVarSpawnBots'"));
		return;
	}
	
	int32 NrOfAliveBots = 0;
	for(TActorIterator<ASAICharacter> It(GetWorld()); It; ++It)
	{
		ASAICharacter *Bot = *It;

		USAttributeComponent *AttributeComp =  USAttributeComponent::GetAttributes(Bot);
		if(ensure(AttributeComp) && AttributeComp->IsAlive())
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("SPAWNING BOT"));
			NrOfAliveBots++;
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Found %i alive bots."), NrOfAliveBots);

	float MaxBotCount = 10.0f;
	if(DifficultyCurve)
	{
		MaxBotCount = DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);
	}

	if(NrOfAliveBots >= MaxBotCount)
	{
		UE_LOG(LogTemp, Log, TEXT("At maximum bot capacity. Skipping bot spawn."));
		return;
	}
	
	UEnvQueryInstanceBlueprintWrapper *QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);
	
	if(ensure(QueryInstance))
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::OnQueryCompleted);	
	}
}

void ASGameModeBase::OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if(QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Log, TEXT("Spawn bot EQS Query Failed!"));
		return;
	}
	
	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();

	if(Locations.IsValidIndex(0))
	{
		GetWorld()->SpawnActor<AActor>(MinionClass, Locations[0], FRotator::ZeroRotator);
		DrawDebugSphere(GetWorld(), Locations[0], 50.0f, 20, FColor::Blue, false, 60.0f);
	}
}

void ASGameModeBase::WriteSaveGame()
{
	// Iterate all player states, we don't have proper ID to match yet (requires Steam or EOS)
	for(int32 i = 0; i < GameState->PlayerArray.Num(); i++)
	{
		ASPlayerState *PS = Cast<ASPlayerState>(GameState->PlayerArray[i]);
		if(PS)
		{
			PS->SavePlayerState(CurrentSaveGame);
			break; // Single player only at this point
		}
	}

	// Clear the previously saved actors for a new batch. If we don't do this we might get duplicates
	CurrentSaveGame->SavedActors.Empty();

	// Iterate the entire world of actors
	for(FActorIterator It(GetWorld()); It; ++It)
	{
		AActor *Actor = *It;

		// Only interested in our 'gameplay actors'
		if(!Actor->Implements<USGameplayInterface>())
		{
			continue;
		}

		FActorSaveData ActorData;
		ActorData.ActorName = Actor->GetName();
		ActorData.Transform = Actor->GetActorTransform();

		// Serialize and store actor variables in ActorData.ByteData, ByteData is an array of bytes
		FMemoryWriter MemWriter(ActorData.ByteData); // Pass the array to fill with data from Actor
		FObjectAndNameAsStringProxyArchive Archive(MemWriter, true);
		Archive.ArIsSaveGame = true; // Find only variables with UPROPERTY(SaveGame)
		Actor->Serialize(Archive); // Converts Actor's SaveGame UPROPERTIES into binary array

		CurrentSaveGame->SavedActors.Add(ActorData);
	}

	UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SlotName, 0);
}

void ASGameModeBase::LoadSaveGame()
{
	if(UGameplayStatics::DoesSaveGameExist(SlotName, 0))
	{
		CurrentSaveGame = Cast<USSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));

		if(CurrentSaveGame == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to load SaveGame Data."));
			return;;
		}

		UE_LOG(LogTemp, Log, TEXT("Loaded SaveGame Data."));


		for(FActorIterator It(GetWorld()); It; ++It)
		{
			AActor *Actor = *It;
			// Only interested in our 'gameplay actors'
			if(!Actor->Implements<USGameplayInterface>())
			{
				continue;
			}

			for(FActorSaveData ActorData : CurrentSaveGame->SavedActors)
			{
				if(ActorData.ActorName == Actor->GetName())
				{
					Actor->SetActorTransform(ActorData.Transform);

					FMemoryReader MemReader(ActorData.ByteData);
					FObjectAndNameAsStringProxyArchive Ar(MemReader, true);
					Ar.ArIsSaveGame = true;
					Actor->Serialize(Ar); // Convert binary array back into actor's variables

					ISGameplayInterface::Execute_OnActorLoaded(Actor);

					break;
				}
			}
		}
	}
	else
	{
		CurrentSaveGame = Cast<USSaveGame>(UGameplayStatics::CreateSaveGameObject(USSaveGame::StaticClass()));

		UE_LOG(LogTemp, Log, TEXT("Created New SaveGame Data."));
	}
}