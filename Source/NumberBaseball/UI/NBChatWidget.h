// NBChatWidget.h

#pragma once

#include "Blueprint/UserWidget.h"
#include "NBChatWidget.generated.h"

class UEditableTextBox;
class UScrollBox;
class UTextBlock;

/**
 * 채팅 로그, 숫자 입력창, 시도 횟수를 보여주는 위젯.
 */
UCLASS()
class NUMBERBASEBALL_API UNBChatWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void AddChatMessage(const FString& InMessage);

	void SetAttemptsText(const FString& InPlayerName, const FString& InAttempts);

	TSharedPtr<SWidget> GetInputFocusWidget() const;

protected:
	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

private:
	UFUNCTION()
	void OnGuessInputCommitted(const FText& InText, ETextCommit::Type InCommitMethod);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UScrollBox> ChatLogScrollBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UEditableTextBox> GuessInputTextBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UTextBlock> AttemptsText;

private:
	UPROPERTY(EditDefaultsOnly, Category = "NumberBaseball", Meta = (ClampMin = 1))
	int32 ChatMessageFontSize = 18;

	UPROPERTY(EditDefaultsOnly, Category = "NumberBaseball", Meta = (ClampMin = 1))
	int32 MaxChatMessageCount = 100;

};
