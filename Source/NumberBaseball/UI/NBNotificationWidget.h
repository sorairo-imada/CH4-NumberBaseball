// NBNotificationWidget.h

#pragma once

#include "Blueprint/UserWidget.h"
#include "NBNotificationWidget.generated.h"

class UTextBlock;

/**
 * 승리/무승부 결과를 화면에 띄우는 공지 위젯.
 */
UCLASS()
class NUMBERBASEBALL_API UNBNotificationWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// 빈 문자열이면 위젯을 숨긴다.
	void ShowNotification(const FString& InNotification);

protected:
	virtual void NativeConstruct() override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UTextBlock> NotificationText;

};
