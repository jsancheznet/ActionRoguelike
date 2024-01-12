// 2023 Jorge Sánchez, All rights reserved

#include "AI/SAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

void ASAIController::BeginPlay()
{
	Super::BeginPlay();

	if(ensureMsgf(BehaviorTree, TEXT("BehaviorTree is nullptr! please assign BehaviorTree in your AI Controller.")))
	{
		RunBehaviorTree(BehaviorTree);
	}

	// APawn *PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	// if(PlayerPawn)
	// {
	// 	GetBlackboardComponent()->SetValueAsVector("MoveToLocation", PlayerPawn->GetActorLocation());			
	// 	GetBlackboardComponent()->SetValueAsObject("TargetActor", PlayerPawn);			
	// }
}