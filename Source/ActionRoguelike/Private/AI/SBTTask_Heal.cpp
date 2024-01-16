// 2023 Jorge Sánchez, All rights reserved

#include "AI/SBTTask_Heal.h"

#include "AIController.h"
#include "SAttributeComponent.h"
#include "AI/SAICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type USBTTask_Heal::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent *BlackboardComponent = OwnerComp.GetBlackboardComponent();
	if(BlackboardComponent)
	{
		AAIController *AIController = OwnerComp.GetAIOwner();
		if(AIController)
		{
			ASAICharacter *AICharacter = Cast<ASAICharacter>(AIController->GetCharacter());
			if(AICharacter)
			{
				USAttributeComponent *AttribComp =  Cast<USAttributeComponent>(AICharacter->GetComponentByClass(USAttributeComponent::StaticClass()));
				if(AttribComp)
				{
					AttribComp->ApplyHealthChange(AttribComp->GetMaxHealth());
					return EBTNodeResult::Succeeded;
				}
			}
		}
	}
	
	return EBTNodeResult::Failed;	
}