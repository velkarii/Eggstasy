#include "PlayerCharacter.h"
#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "GM_Eggstasy.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCharacterMovement()->MaxWalkSpeed = 350.f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 350.f;
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->bDoCollisionTest = false;

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(CameraBoom);
	Camera->bUsePawnControlRotation = false;


	TargetOffset = FVector::ZeroVector;
	DefaultBoomOffset = FVector(0.f, 0.f, 0.f);
	CameraBoomOffset = FVector(15.f, 0.f, 0.f);
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
		Input->BindAction(InteractAction, ETriggerEvent::Started, this, &APlayerCharacter::Interact);
		Input->BindAction(PauseAction, ETriggerEvent::Started, this, &APlayerCharacter::Pause);
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

void APlayerCharacter::Interact()
{
	FVector TraceStart = GetActorLocation();
	float MaxInteractRange = 268.f;
	TryInteract(TraceStart, MaxInteractRange);
}

void APlayerCharacter::Pause()
{
	if (auto gameModeRef = Cast<AGM_Eggstasy>(GetWorld()->GetAuthGameMode()))
	{
		gameModeRef->OpenPauseMenu();
	}
}

bool APlayerCharacter::TryInteract(FVector TraceStart, float MaxInteractRange)
{
	FHitResult LocalHit;
	FVector TraceEnd = TraceStart + (GetActorForwardVector() * MaxInteractRange);
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	bool bHasHit = GetWorld()->LineTraceSingleByChannel(LocalHit, TraceStart, TraceEnd, ECC_GameTraceChannel1, Params);

	if (bHasHit)
	{
		AActor* HitActor = LocalHit.GetActor();

		OnInteractableHit(HitActor);
	}
	return bHasHit;
}

void APlayerCharacter::UpdateSocketOffset(float DeltaTime)
{
	FVector CurrentOffset = CameraBoom->SocketOffset;
	FVector NewOffset = FMath::VInterpTo(CurrentOffset, TargetOffset, DeltaTime, 2.f);
	CameraBoom->SocketOffset = NewOffset;
}