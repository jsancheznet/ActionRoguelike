// 2023 Jorge Sánchez, All rights reserved

#include "AI/SAIController.h"

void ASAIController::BeginPlay()
{
	Super::BeginPlay();

	if(ensureMsgf(BehaviorTree, TEXT("BehaviorTree is nullptr! please assign BehaviorTree in your AI Controller.")))
	{
		RunBehaviorTree(BehaviorTree);
	}
}