// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GM_Eggstasy.generated.h"

/**
 * 
 */
UCLASS()
class EGGSTASY_API AGM_Eggstasy : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AGM_Eggstasy();

	UFUNCTION(BlueprintImplementableEvent)
	void OpenPauseMenu();
};
