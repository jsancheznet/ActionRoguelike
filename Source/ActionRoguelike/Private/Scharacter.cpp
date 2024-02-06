// Fill out your copyright notice in the Description page of Project Settings.


#include "Scharacter.h"

#include "SActionComponent.h"
#include "SInteractionComponent.h"
#include "Camera/CameraComponent.h"
#include "SAttributeComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"

AScharacter::AScharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);

	InteractionComponent = CreateDefaultSubobject<USInteractionComponent>("InteractionComponent");

	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");

	ActionComponent = CreateDefaultSubobject<USActionComponent>("ActionComponent");

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	TimeToHitParamName = "TimeToHit";
}

void AScharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	AttributeComp->OnHealthChanged.AddDynamic(this, &AScharacter::OnHealthChanged);

	// Set material static values
	GetMesh()->SetScalarParameterValueOnMaterials("HitFlashSpeed", 4.0f);
	GetMesh()->SetVectorParameterValueOnMaterials("HitFlashColor", FVector(1.0f, 0.0f, 1.0f));
}
void AScharacter::BeginPlay()
{
	Super::BeginPlay();
}

FVector AScharacter::GetPawnViewLocation() const
{
	return CameraComp->GetComponentLocation();
}

void AScharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// -- Rotation Visualization -- //
	const float DrawScale = 100.0f;
	const float Thickness = 5.0f;

	FVector LineStart = GetActorLocation();
	// Offset to the right of pawn
	LineStart += GetActorRightVector() * 100.0f;
	// Set line end in direction of the actor's forward
	FVector ActorDirection_LineEnd = LineStart + (GetActorForwardVector() * 100.0f);
	// Draw Actor's Direction
	DrawDebugDirectionalArrow(GetWorld(), LineStart, ActorDirection_LineEnd, DrawScale, FColor::Yellow, false, 0.0f, 0, Thickness);

	FVector ControllerDirection_LineEnd = LineStart + (GetControlRotation().Vector() * 100.0f);
	// Draw 'Controller' Rotation ('PlayerController' that 'possessed' this character)
	DrawDebugDirectionalArrow(GetWorld(), LineStart, ControllerDirection_LineEnd, DrawScale, FColor::Green, false, 0.0f, 0, Thickness);

	// FString MyRage = FString::Printf(TEXT("Rage: %d"), AttributeComp->GetRage());
	// GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red, *MyRage);

}

// Called to bind functionality to input
void AScharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AScharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AScharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &AScharacter::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &AScharacter::LookUp);
	
	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &AScharacter::PrimaryAttack);
	PlayerInputComponent->BindAction("BlackHoleAttack", IE_Pressed, this, &AScharacter::BlackHoleAttack);
	PlayerInputComponent->BindAction("TeleportAttack", IE_Pressed, this, &AScharacter::DashAttack);	
	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &AScharacter::PrimaryInteract);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AScharacter::SprintStart);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AScharacter::SprintStop);
	
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AScharacter::Jump);
}

void AScharacter::HealSelf(float Amount)
{
	AttributeComp->ApplyHealthChange(this, Amount);
}

void AScharacter::MoveForward(float Value)
{
	FRotator Rotation = GetControlRotation();
	Rotation.Pitch = 0.0f;
	Rotation.Roll = 0.0f;
	AddMovementInput(Rotation.Vector(), Value);
}

void AScharacter::MoveRight(float Value)
{
	FRotator ControlRotation = GetControlRotation();
	ControlRotation.Pitch = 0.0f;
	ControlRotation.Roll = 0.0f;
    FVector RightVector = UKismetMathLibrary::GetRightVector(ControlRotation);	
	AddMovementInput(RightVector, Value);
}

void AScharacter::SprintStart()
{
	ActionComponent->StartActionByName(this, "Sprint");
}

void AScharacter::SprintStop()
{
	ActionComponent->StopActionByName(this, "Sprint");
}

void AScharacter::Turn(float Value)
{
	AddControllerYawInput(Value);
}

void AScharacter::LookUp(float Value)
{
	AddControllerPitchInput(-Value);
}

void AScharacter::Jump()
{
	Super::Jump();
}

void AScharacter::PrimaryAttack()
{
	ActionComponent->StartActionByName(this, "PrimaryAttack");
}

void AScharacter::BlackHoleAttack()
{
	ActionComponent->StartActionByName(this, "Blackhole");
}

void AScharacter::DashAttack()
{
	ActionComponent->StartActionByName(this, "Dash");
}

void AScharacter::PrimaryInteract()
{
	if(InteractionComponent)
	{
		InteractionComponent->PrimaryInteract();	
	}
}

void AScharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if(Delta < 0.0f)
	{
		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName, GetWorld()->TimeSeconds);

		AttributeComp->AddRageFromDamage(Delta);
	}

	// Dead
	if(NewHealth <= 0.0f && Delta < 0.0f)
	{
		APlayerController *PC = Cast<APlayerController>(GetController());
		DisableInput(PC);

		SetLifeSpan(5.0f);
	}
}