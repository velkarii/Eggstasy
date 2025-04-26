#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputMappingContext.h"
#include "InputActionValue.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class EGGSTASY_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	UInputMappingContext* InputMapping;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	UInputAction* SprintAction;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	UInputAction* InteractAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USpringArmComponent* CameraBoom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector DefaultBoomOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector CameraBoomOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector TargetOffset;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void UpdateSocketOffset(float DeltaTime);
	void SprintStart();
	void SprintEnd();
	void Interact();

public:
	UFUNCTION(BlueprintCallable, Category = "Interact")
	bool TryInteract(FVector TraceStart, float MaxInteractRange);

	UFUNCTION(BlueprintImplementableEvent, Category = "Interact")
	void OnInteractableHit(AActor* HitActor);
};