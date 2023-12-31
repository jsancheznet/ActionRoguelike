// 2023 Jorge Sánchez, All rights reserved


#include "STargetDummy.h"

// Sets default values
ASTargetDummy::ASTargetDummy()
{
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("Static Mesh");
	RootComponent = MeshComp;
	
	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComponent");
	AttributeComp->OnHealthChanged.AddDynamic(this, &ASTargetDummy::OnHealthChanged);
}

void ASTargetDummy::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if(Delta < 0.0f)
	{
		MeshComp->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);	
	}
}