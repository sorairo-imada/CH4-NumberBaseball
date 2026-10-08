// NBPlayerController.h

#pragma once

#include "GameFramework/PlayerController.h"
#include "NBPlayerController.generated.h"

class UNBChatWidget;
class UNBNotificationWidget;

/**
 * 클라이언트 입력을 서버로 올리고(Server RPC), 서버가 내려준 결과를 위젯에 뿌린다.
 */
UCLASS()
class NUMBERBASEBALL_API ANBPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	virtual void OnRep_PlayerState() override;

	UFUNCTION(Server, Reliable)
	void ServerRPCSubmitGuess(const FString& InGuess);

	// 입력 오류처럼 본인에게만 보여줄 안내 문구.
	UFUNCTION(Client, Reliable)
	void ClientRPCPrintSystemMessage(const FString& InMessage);

	void AddChatMessage(const FString& InMessage);

	void RefreshAttempts();

	void ShowNotification(const FString& InNotification);

private:
	UUserWidget* CreateGameWidget(TSubclassOf<UUserWidget> InWidgetClass, UClass* InBaseClass, const TCHAR* InDefaultClassPath, int32 InZOrder);

private:
	// 비워두면 /Game/NumberBaseball/UI/WBP_NBChat 을 찾아 쓴다.
	UPROPERTY(EditDefaultsOnly, Category = "NumberBaseball")
	TSubclassOf<UNBChatWidget> ChatWidgetClass;

	UPROPERTY()
	TObjectPtr<UNBChatWidget> ChatWidgetInstance;

	// 비워두면 /Game/NumberBaseball/UI/WBP_NBNotification 을 찾아 쓴다.
	UPROPERTY(EditDefaultsOnly, Category = "NumberBaseball")
	TSubclassOf<UNBNotificationWidget> NotificationWidgetClass;

	UPROPERTY()
	TObjectPtr<UNBNotificationWidget> NotificationWidgetInstance;

};
