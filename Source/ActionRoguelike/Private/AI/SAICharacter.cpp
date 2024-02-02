// 2023 Jorge Sánchez, All rights reserved

#include "AI/SAICharacter.h"

#include "AIController.h"
#include "AIHelpers.h"
#include "BrainComponent.h"
#include "SActionComponent.h"
#include "SAttributeComponent.h"
#include "Scharacter.h"
#include "SPlayerState.h"
#include "SWorldUserWidget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/PawnSensingComponent.h"

ASAICharacter::ASAICharacter()
{
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComp");
	AttribComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");
	ActionComp = CreateDefaultSubobject<USActionComponent>("ActionComp");

	// Ensures we receive a controlled when spawned in the level by our gamemode
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// Disabled on capsule to let projectiles pass through capsule and hit mesh instead
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);

	// Enabled on mesh to react to incoming projectiles
	GetMesh()->SetGenerateOverlapEvents(true);

	TimeToHitParamName = "TimeToHit";
	TargetActorKey = "TargetActor";

	CreditWorthAmount = 10;	
}

void ASAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	PawnSensingComp->OnSeePawn.AddDynamic(this, &ASAICharacter::OnPawnSeen);
	AttribComp->OnHealthChanged.AddDynamic(this, &ASAICharacter::OnHealthChanged);
}

void ASAICharacter::OnPawnSeen(APawn* Pawn)
{
	// Ignore if target already set
	if(GetTargetActor() != Pawn)
	{
		SetTargetActor(Pawn);
		SpawnSpottedWidget();
	}

	// DrawDebugString(GetWorld(), GetActorLocation(), "PLAYER SPOTTED", nullptr, FColor::White, 0.5f, true);
}

void ASAICharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if(Delta < 0.0f)
	{
		if(InstigatorActor != this)
		{
			// if(GetTargetActor() != InstigatorActor)
			// {
			// 	SpawnSpottedWidget();
			// }
			
			SetTargetActor(InstigatorActor);
		}

		if(ActiveHealthBar == nullptr)
		{
			ActiveHealthBar =  CreateWidget<USWorldUserWidget>(GetWorld(), HealthBarWidgetClass);
			if(ActiveHealthBar)
			{
				ActiveHealthBar->AttachedActor = this;
				ActiveHealthBar->AddToViewport();
			}			
		}
		
		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName, GetWorld()->TimeSeconds);

		// Died
		if(NewHealth <= 0.0f)
		{
			// Stop BT
			AAIController *AIC = Cast<AAIController>(GetController());
			if(AIC)
			{
				AIC->GetBrainComponent()->StopLogic("Killed");
			}
			
			// Ragdoll
			GetMesh()->SetAllBodiesSimulatePhysics(true);
			GetMesh()->SetCollisionProfileName("Ragdoll");

			// When the minion dies, the capsule has collision on the place where it died, colliding with the player.
			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			GetCharacterMovement()->DisableMovement();

			// Set lifespan (How long until we destroy the actor)
			SetLifeSpan(10.0f);
		}
	}
}

AActor* ASAICharacter::GetTargetActor() const
{
	AAIController *AIC = Cast<AAIController>(GetController());
	if(AIC)
	{
		return Cast<AActor>(AIC->GetBlackboardComponent()->GetValueAsObject(TargetActorKey));
	}

	return nullptr;
}

void ASAICharacter::SpawnSpottedWidget()
{
	USWorldUserWidget *NewWidget = CreateWidget<USWorldUserWidget>(GetWorld(), SpottedWidgetClass);
	if(NewWidget)
	{
		NewWidget->AttachedActor = this;
		// Index of 10 (or anything higher than default of 0) places this on top of any other widget.
		// May end up begind the minion health bar otherwise.
		NewWidget->AddToViewport(10);
	}	
}

void ASAICharacter::SetTargetActor(AActor* NewTarget)
{
	AAIController *AIController = Cast<AAIController>(GetController());
	if(AIController)
	{
		AIController->GetBlackboardComponent()->SetValueAsObject(TargetActorKey, NewTarget);
	}
}