// 2023 Jorge Sánchez, All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "GameFramework/GameModeBase.h"
#include "SGameModeBase.generated.h"

class USMonsterData;
class USSaveGame;
class UEnvQuery;
class UEnvQueryInstanceBlueprintWrapper;
class UCurveFloat;
class UDataTable;

/* DataTable Row for spawning monsters in game mode */
USTRUCT(BlueprintType)
struct FMonsterInfoRow : public  FTableRowBase
{
	GENERATED_BODY()

public:

	FMonsterInfoRow()
	{
		Weight = 1.0f;
		SpawnCost = 5.0f;
		KillReward = 20.0f;
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FPrimaryAssetId MonsterId;

	// Relative chance to pick this monster
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Weight;

	// Points required by gamemode to spawn this unit
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float SpawnCost;	

	// Amount of credits awarded to killer of this unit
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float KillReward;
};

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:

	UPROPERTY()
	FString SlotName;

	UPROPERTY()
	USSaveGame *CurrentSaveGame;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UDataTable *MonsterTable;
	
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	bool bAISpawnDisabled;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UEnvQuery *SpawnBotQuery;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UCurveFloat *DifficultyCurve;

	FTimerHandle TimerHandle_SpawnBots;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float SpawnTimerInterval;

	UFUNCTION()
	void SpawnBotTimerElapsed();

	UFUNCTION()
	void OnBotSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper *QueryInstance, EEnvQueryStatus::Type QueryStatus);

	void OnMonsterLoaded(FPrimaryAssetId LoadedId, FVector SpawnLocation);

	UFUNCTION()
	void RespawnPlayerElapsed(AController *Controller);
	
public:

	ASGameModeBase();

	void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	
	virtual void OnActorKilled(AActor *VictimActor, AActor *Killer);
	
	virtual void StartPlay() override;

	void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

	UFUNCTION(Exec)
	void KillAllAI();

	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	void WriteSaveGame();
	
	void LoadSaveGame();
};