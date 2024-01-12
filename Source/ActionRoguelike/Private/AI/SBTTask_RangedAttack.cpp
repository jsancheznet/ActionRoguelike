// 2023 Jorge Sánchez, All rights reserved


#include "AI/SBTTask_RangedAttack.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"

EBTNodeResult::Type USBTTask_RangedAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController *MyController =  OwnerComp.GetAIOwner();
	if(ensure(MyController))
	{
		ACharacter *MyPawn = Cast<ACharacter>(MyController->GetPawn());
		if(MyPawn == nullptr)
		{
			return EBTNodeResult::Failed;
		}
		
		FVector MuzzleLocation = MyPawn->GetMesh()->GetSocketLocation("Muzzle_01");
		
		AActor *TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("TargetActor"));

		if(TargetActor == nullptr)
		{
			return EBTNodeResult::Failed;
		}

		FVector Direction = TargetActor->GetActorLocation() - MuzzleLocation;
		FRotator MuzzleRotation = Direction.Rotation();

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		AActor *NewProjectile = GetWorld()->SpawnActor<AActor>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);

		return NewProjectile ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
	}

	return EBTNodeResult::Failed;
}