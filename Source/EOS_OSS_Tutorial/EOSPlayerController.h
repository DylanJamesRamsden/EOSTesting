// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EOSPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class EOS_OSS_TUTORIAL_API AEOSPlayerController : public APlayerController
{
	GENERATED_BODY()

private:

	//Delegate to bind callback event for login. 
	FDelegateHandle LoginDelegateHandle;

public:

	AEOSPlayerController();

protected:

	// Function called when play begins
	virtual void BeginPlay();
 
	//Function to sign into EOS Game Services
	void Login();
 
	//Callback function. This function is ran when signing into EOS Game Services completes. 
	void HandleLoginCompleted(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error);
};
