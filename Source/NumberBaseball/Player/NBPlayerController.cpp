// NBPlayerController.cpp

#include "Player/NBPlayerController.h"

#include "Blueprint/UserWidget.h"
#include "Game/NBGameModeBase.h"
#include "Game/NBGameStateBase.h"
#include "NumberBaseball.h"
#include "Player/NBPlayerState.h"
#include "UI/NBChatWidget.h"
#include "UI/NBNotificationWidget.h"

void ANBPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController() == false)
	{
		return;
	}

	ChatWidgetInstance = Cast<UNBChatWidget>(CreateGameWidget(ChatWidgetClass, UNBChatWidget::StaticClass(), TEXT("/Game/NumberBaseball/UI/WBP_NBChat.WBP_NBChat_C"), 0));
	NotificationWidgetInstance = Cast<UNBNotificationWidget>(CreateGameWidget(NotificationWidgetClass, UNBNotificationWidget::StaticClass(), TEXT("/Game/NumberBaseball/UI/WBP_NBNotification.WBP_NBNotification_C"), 1));

	FInputModeUIOnly Mode;
	if (IsValid(ChatWidgetInstance) == true)
	{
		Mode.SetWidgetToFocus(ChatWidgetInstance->GetInputFocusWidget());
	}
	SetInputMode(Mode);

	bShowMouseCursor = true;

	RefreshAttempts();

	// 게임 도중에 접속한 경우를 위해 현재 공지 상태를 한 번 읽어온다.
	const ANBGameStateBase* NBGameState = GetWorld()->GetGameState<ANBGameStateBase>();
	if (IsValid(NBGameState) == true)
	{
		ShowNotification(NBGameState->GetNotification());
	}
}

void ANBPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	RefreshAttempts();
}

void ANBPlayerController::ServerRPCSubmitGuess_Implementation(const FString& InGuess)
{
	ANBGameModeBase* NBGameMode = GetWorld()->GetAuthGameMode<ANBGameModeBase>();
	if (IsValid(NBGameMode) == true)
	{
		NBGameMode->HandleGuess(this, InGuess);
	}
}

void ANBPlayerController::ClientRPCPrintSystemMessage_Implementation(const FString& InMessage)
{
	AddChatMessage(FString::Printf(TEXT("[안내] %s"), *InMessage));
}

void ANBPlayerController::AddChatMessage(const FString& InMessage)
{
	UE_LOG(LogNumberBaseball, Log, TEXT("[Chat] %s"), *InMessage);

	if (IsValid(ChatWidgetInstance) == true)
	{
		ChatWidgetInstance->AddChatMessage(InMessage);
	}
}

void ANBPlayerController::RefreshAttempts()
{
	const ANBPlayerState* NBPlayerState = GetPlayerState<ANBPlayerState>();
	if (IsValid(ChatWidgetInstance) == true && IsValid(NBPlayerState) == true)
	{
		ChatWidgetInstance->SetAttemptsText(NBPlayerState->GetPlayerName(), NBPlayerState->GetAttemptsString());
	}
}

void ANBPlayerController::ShowNotification(const FString& InNotification)
{
	if (InNotification.IsEmpty() == false)
	{
		UE_LOG(LogNumberBaseball, Log, TEXT("[Notification] %s"), *InNotification);
	}

	if (IsValid(NotificationWidgetInstance) == true)
	{
		NotificationWidgetInstance->ShowNotification(InNotification);
	}
}

UUserWidget* ANBPlayerController::CreateGameWidget(TSubclassOf<UUserWidget> InWidgetClass, UClass* InBaseClass, const TCHAR* InDefaultClassPath, int32 InZOrder)
{
	UClass* WidgetClass = InWidgetClass.Get();
	if (IsValid(WidgetClass) == false)
	{
		WidgetClass = StaticLoadClass(InBaseClass, nullptr, InDefaultClassPath, nullptr, LOAD_NoWarn);
	}

	if (IsValid(WidgetClass) == false)
	{
		UE_LOG(LogNumberBaseball, Error, TEXT("Widget class not found: %s"), InDefaultClassPath);
		return nullptr;
	}

	UUserWidget* WidgetInstance = CreateWidget<UUserWidget>(this, WidgetClass);
	if (IsValid(WidgetInstance) == true)
	{
		WidgetInstance->AddToViewport(InZOrder);
	}

	return WidgetInstance;
}
