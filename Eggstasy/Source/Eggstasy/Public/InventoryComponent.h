// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EGGSTASY_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()


public:
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> Items;

    UFUNCTION(BlueprintCallable)
    void AddItem(const FString& Item);

    UFUNCTION(BlueprintCallable)
    bool HasItem(const FString& Item) const;
};
