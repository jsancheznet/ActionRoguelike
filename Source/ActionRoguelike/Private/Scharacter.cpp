// Fill out your copyright notice in the Description page of Project Settings.


#include "Scharacter.h"

#include "SInteractionComponent.h"
#include "Camera/CameraComponent.h"
#include "SAttributeComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AScharacter::AScharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);

	InteractionComponent = CreateDefaultSubobject<USInteractionComponent>("InteractionComponent");

	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void AScharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	AttributeComp->OnHealthChanged.AddDynamic(this, &AScharacter::OnHealthChanged);
}
void AScharacter::BeginPlay()
{
	Super::BeginPlay();
	
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
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AScharacter::Jump);
}

void AScharacter::SpawnProjectileClass(TSubclassOf<AActor> ProjectileClass)
{
	if(ensure(ProjectileClass))
	{
		FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");
		FVector Target = GetProjectileTarget();
		FRotator ProjectileRotator = UKismetMathLibrary::FindLookAtRotation(HandLocation, Target);
		FActorSpawnParameters SpawnParams = {};
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = this;
		FTransform SpawnTransform = FTransform(ProjectileRotator, HandLocation);
		GetWorld()->SpawnActor(ProjectileClass, &SpawnTransform, SpawnParams);
	}
}

FVector AScharacter::GetProjectileTarget()
{
	FVector Result = {};
	
	// Line Trace from the Camera to World to get the desired impact point of the projectile, if the line trace fails to hit something, use the end location of the trace
	FVector Start = CameraComp->GetComponentLocation();
	FVector End = CameraComp->GetComponentLocation() + (GetControlRotation().Vector() * 5000.0f);	
	FCollisionObjectQueryParams QueryParams = {};
	QueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	QueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
	QueryParams.AddObjectTypesToQuery(ECC_Pawn);
	FHitResult HitResult = {};
	
	// NOTE(Jorge): Tom uses SweepSingleByObjectType  with a sphere of 20.0f radius to make it easier for the player to hit targets	
	bool bLineTraceHit = GetWorld()->LineTraceSingleByObjectType(HitResult, Start, End, QueryParams); 

	if(bLineTraceHit)
	{
		Result = HitResult.ImpactPoint;
	}
	else
	{
		Result = End;
	}
	
	return Result;
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
	PlayAnimMontage(AttackAnim);
	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &AScharacter::PrimaryAttack_TimeElapsed, 0.2f);
}

void AScharacter::BlackHoleAttack()
{
	PlayAnimMontage(AttackAnim);
	GetWorldTimerManager().SetTimer(TimerHandle_BlackHoleAttack, this, &AScharacter::BlackHoleAttack_TimeElapsed, 0.2f);	
}

void AScharacter::DashAttack()
{
	PlayAnimMontage(AttackAnim);
	GetWorldTimerManager().SetTimer(TimerHandle_DashAttack, this, &AScharacter::DashAttack_TimeElapsed, 0.2f);	
}

void AScharacter::PrimaryInteract()
{
	if(InteractionComponent)
	{
		InteractionComponent->PrimaryInteract();	
	}
}

void AScharacter::PrimaryAttack_TimeElapsed()
{
	SpawnProjectileClass(MagicProjectileClass);
}

void AScharacter::BlackHoleAttack_TimeElapsed()
{
	SpawnProjectileClass(BlackHoleProjectileClass);
}

void AScharacter::DashAttack_TimeElapsed()
{
	SpawnProjectileClass(DashProjectileClass);
}

void AScharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if(NewHealth <= 0.0f && Delta < 0.0f)
	{
		APlayerController *PC = Cast<APlayerController>(GetController());
		DisableInput(PC);
	}
}