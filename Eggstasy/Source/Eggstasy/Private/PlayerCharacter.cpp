// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCharacterMovement()->MaxWalkSpeed = 350.f;
	
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	CameraBoom->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(CameraBoom);
	Camera->bUsePawnControlRotation = true;

	TargetOffset = FVector::ZeroVector;
	DefaultBoomOffset = FVector(0.f, 0.f, 0.f);
	CameraBoomOffset = FVector(20.f, 0.f, 0.f);

}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputMapping, 0);
		}
	}
	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateSocketOffset(DeltaTime);

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* Input = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		Input->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
		Input->BindAction(SprintAction, ETriggerEvent::Triggered, this, &APlayerCharacter::SprintStart);
		Input->BindAction(SprintAction, ETriggerEvent::Completed, this, &APlayerCharacter::SprintEnd);
	}

}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(ForwardDirection, MovementVector.Y);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(RightDirection, MovementVector.X);

}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();
	AddControllerYawInput(MovementVector.X);
	AddControllerPitchInput(MovementVector.Y);
}

void APlayerCharacter::SprintStart()
{
	TargetOffset = CameraBoomOffset;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
}

void APlayerCharacter::SprintEnd()
{
	TargetOffset = DefaultBoomOffset;
	GetCharacterMovement()->MaxWalkSpeed = 350.f;
}

void APlayerCharacter::UpdateSocketOffset(float DeltaTime)
{
	FVector CurrentOffset = CameraBoom->SocketOffset;
	FVector NewOffset = FMath::VInterpTo(CurrentOffset, TargetOffset, DeltaTime, 2.f);
	CameraBoom->SocketOffset = NewOffset;
}