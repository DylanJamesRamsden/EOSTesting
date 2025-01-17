// Fill out your copyright notice in the Description page of Project Settings.


#include "EOSPlayerController.h"

#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "OnlineSubsystemTypes.h"
#include "Interfaces/OnlineIdentityInterface.h"


AEOSPlayerController::AEOSPlayerController()
{
	
}

void AEOSPlayerController::BeginPlay()
{
	Super::BeginPlay();

    Login(); //Call login function 
}

void AEOSPlayerController::Login()
{

    /* @TODO This entire function doesn't make sense to me, if there is an auth type in the command line, it just auto logins but if there isn't
     * it runs an AccountPortal login. Defeats the point of passing in AccountPortal into the command line so should probably inverse it. */
    
	/*
    Tutorial 2: This function will access the EOS OSS via the OSS identity interface to log first into Epic Account Services, and then into Epic Game Services.
    It will bind a delegate to handle the callback event once login call succeeeds or fails. 
    All functions that access the OSS will have this structure: 1-Get OSS interface, 2-Bind delegate for callback and 3-Call OSS interface function (which will call the correspongin EOS OSS function)
    */
    IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld());
    IOnlineIdentityPtr Identity = Subsystem->GetIdentityInterface(); // This is the generic OSS interface that will access the EOS OSS.
 
    // If you're logged in, don't try to login again.
    // This can happen if your player travels to a dedicated server or different maps as BeginPlay() will be called each time.
    
    FUniqueNetIdPtr NetId = Identity->GetUniquePlayerId(0);
 
    if (NetId != nullptr && Identity->GetLoginStatus(0) == ELoginStatus::LoggedIn)
    {
        return; 
    }
    
    /* This binds a delegate so we can run our function when the callback completes. 0 represents the player number.
    You should parametrize this Login function and pass the parameter here for splitscreen. 
    */
    LoginDelegateHandle = Identity->AddOnLoginCompleteDelegate_Handle(0, FOnLoginCompleteDelegate::CreateUObject(this, &ThisClass::HandleLoginCompleted));
 
    // Grab command line parameters. If empty call hardcoded login function - Hardcoded login function useful for Play In Editor. 
    FString AuthType; 
    FParse::Value(FCommandLine::Get(), TEXT("AUTH_TYPE="), AuthType);

    // @TODO If the AuthType isn't empty, an AutoLogin is done. This doesn't seem right as we may way to pass in different AuthTypes
    // into the command line, but leaving as it
    if (!AuthType.IsEmpty()) //If parameter is NOT empty we can autologin.
    {
        /* 
        In most situations you will want to automatically log a player in using the parameters passed via CLI.
        For example, using the exchange code for the Epic Games Store.
        */
        UE_LOG(LogTemp, Log, TEXT("Logging into EOS...")); // Log to the UE logs that we are trying to log in. 
      
        if (!Identity->AutoLogin(0))
        {
            UE_LOG(LogTemp, Warning, TEXT("Failed to login... ")); // Log to the UE logs that we are trying to log in.
			// Clear our handle and reset the delegate.
			Identity->ClearOnLoginCompleteDelegate_Handle(0, LoginDelegateHandle);
            LoginDelegateHandle.Reset();
        }
    }
    else
    {
        /* 
        Fallback if the CLI parameters are empty.Useful for PIE.
        The type here could be developer if using the DevAuthTool, ExchangeCode if the game is launched via the Epic Games Launcher, etc...
        */
        // @TODO Account Portal
       //  FOnlineAccountCredentials Credentials("AccountPortal","", "");

        // @TODO Developer
        FOnlineAccountCredentials Credentials("Developer","127.0.0.1:8081", "Client1");
 
        UE_LOG(LogTemp, Log, TEXT("Logging into EOS...")); // Log to the UE logs that we are trying to log in. 
        
        if (!Identity->Login(0, Credentials))
        {
            UE_LOG(LogTemp, Warning, TEXT("Failed to login... ")); // Log to the UE logs that we are trying to log in.
			// Clear our handle and reset the delegate. 
            Identity->ClearOnLoginCompleteDelegate_Handle(0, LoginDelegateHandle);
            LoginDelegateHandle.Reset();			
        }        
    }
}

void AEOSPlayerController::HandleLoginCompleted(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId,
	const FString& Error)
{
    /*
    Tutorial 2: This function handles the callback from logging in. You should not proceed with any EOS features until this function is called.
    This function will remove the delegate that was bound in the Login() function.
    */
    IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld());
    IOnlineIdentityPtr Identity = Subsystem->GetIdentityInterface();
    if (bWasSuccessful)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Login successful!"));
        UE_LOG(LogTemp, Log, TEXT("Login callback completed!"));; 
    }
    else //Login failed
        {
        // If your game is online only, you may want to return an errror to the user and return to a menu that uses a different GameMode/PlayerController.
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Login failed! " + Error));
        UE_LOG(LogTemp, Warning, TEXT("EOS login failed.")); //Print sign in failure in logs as a warning.
        }
 
    Identity->ClearOnLoginCompleteDelegate_Handle(LocalUserNum, LoginDelegateHandle);
    LoginDelegateHandle.Reset();
}
