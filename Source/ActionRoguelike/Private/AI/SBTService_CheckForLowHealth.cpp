// 2023 Jorge Sánchez, All rights reserved

#include "AI/SBTService_CheckForLowHealth.h"

#include "AIController.h"
#include "SAttributeComponent.h"
#include "AI/SAICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

void USBTService_CheckForLowHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn *AIPawn = OwnerComp.GetAIOwner()->GetPawn();
	UBlackboardComponent *BlackboardComponent = OwnerComp.GetBlackboardComponent();	
	if(ensure(AIPawn) && ensure(BlackboardComponent))
	{
		USAttributeComponent *AttribComp =  Cast<USAttributeComponent>(AIPawn->GetComponentByClass(USAttributeComponent::StaticClass()));
		if(AttribComp)
		{
			bool bIsLowHealth = AttribComp->IsLowHealth();
			BlackboardComponent->SetValueAsBool(IsLowHealthKey.SelectedKeyName, bIsLowHealth);
		}		
	}
}