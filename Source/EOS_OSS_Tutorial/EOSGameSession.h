// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameSession.h"
#include "Interfaces/OnlineSessionDelegates.h"
#include "EOSGameSession.generated.h"

/**
 * 
 */
UCLASS()
class EOS_OSS_TUTORIAL_API AEOSGameSession : public AGameSession
{
	GENERATED_BODY()

private:

	// Delegate to bind callback event for session creation. 
	FDelegateHandle CreateSessionDelegateHandle;

	// Delegate to bind callback event for register player. 
	FDelegateHandle RegisterPlayerDelegateHandle;

	// Delegate to bind callback event for start session. 
	FDelegateHandle StartSessionDelegateHandle;

	// Delegate to bind callback event for unregister player. 
	FDelegateHandle UnregisterPlayerDelegateHandle;

	// Delegate to bind callback event for end session. 
	FDelegateHandle EndSessionDelegateHandle;

	// Delegate to bind callback event for destroy session. 
	FDelegateHandle DestroySessionDelegateHandle; 

	// Used to keep track if the session exists or not. 
	bool bSessionExists = false;

	// Hardcoding the max number of players in a session. 
	const int MaxNumberOfPlayersInSession = 2;

	int NumberOfPlayersInSession = 0;

protected:

	virtual void BeginPlay() override;

	// Function to create an EOS session. 
	void CreateSession(FName KeyName = "KeyName", FString KeyValue= "KeyValue");

	virtual bool ProcessAutoLogin() override;

	virtual void RegisterPlayer(APlayerController* NewPlayer, const FUniqueNetIdRepl& UniqueId, bool bWasFromInvite) override;

	virtual void UnregisterPlayer(const APlayerController* ExitingPlayer) override;

	virtual void NotifyLogout(const APlayerController* PC) override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:

	void HandleCreateSessionCompleted(FName EOSSessionName, bool bWasSuccessful);

	void HandleRegisterPlayerCompleted(FName EOSSessionName, const TArray<FUniqueNetIdRef>& PlayerIds, bool bWasSuccesful);
	
	void StartSession();

	// Callback function. This function will run when start session compeletes.
	void HandleStartSessionCompleted(FName MySessionName, bool bWasSuccessful);

	void HandleUnregisterPlayerCompleted(FName EOSSessionName, const TArray<FUniqueNetIdRef>& PlayerIds, bool bWasSuccesful);
	
	void EndSession();
	
	void HandleEndSessionCompleted(FName EOSSessionName, bool bWasSuccessful);
	
	void DestroySession();
	
	void HandleDestroySessionCompleted(FName EOSSessionName, bool bWasSuccesful);
};
