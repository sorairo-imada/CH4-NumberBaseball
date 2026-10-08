// NBChatWidget.cpp

#include "UI/NBChatWidget.h"

#include "Blueprint/WidgetTree.h"
#include "Components/EditableTextBox.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Player/NBPlayerController.h"

void UNBChatWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Enter로 제출한 뒤에도 바로 다음 숫자를 칠 수 있게 포커스를 유지한다.
	GuessInputTextBox->SetClearKeyboardFocusOnCommit(false);

	if (GuessInputTextBox->OnTextCommitted.IsAlreadyBound(this, &ThisClass::OnGuessInputCommitted) == false)
	{
		GuessInputTextBox->OnTextCommitted.AddDynamic(this, &ThisClass::OnGuessInputCommitted);
	}
}

void UNBChatWidget::NativeDestruct()
{
	GuessInputTextBox->OnTextCommitted.RemoveDynamic(this, &ThisClass::OnGuessInputCommitted);

	Super::NativeDestruct();
}

void UNBChatWidget::AddChatMessage(const FString& InMessage)
{
	UTextBlock* MessageText = WidgetTree->ConstructWidget<UTextBlock>();
	MessageText->SetText(FText::FromString(InMessage));
	MessageText->SetAutoWrapText(true);

	FSlateFontInfo FontInfo = MessageText->GetFont();
	FontInfo.Size = ChatMessageFontSize;
	MessageText->SetFont(FontInfo);

	ChatLogScrollBox->AddChild(MessageText);

	while (ChatLogScrollBox->GetChildrenCount() > MaxChatMessageCount)
	{
		ChatLogScrollBox->RemoveChildAt(0);
	}

	ChatLogScrollBox->ScrollToEnd();
}

void UNBChatWidget::SetAttemptsText(const FString& InPlayerName, const FString& InAttempts)
{
	AttemptsText->SetText(FText::FromString(FString::Printf(TEXT("%s  시도 횟수 %s"), *InPlayerName, *InAttempts)));
}

TSharedPtr<SWidget> UNBChatWidget::GetInputFocusWidget() const
{
	return GuessInputTextBox->GetCachedWidget();
}

void UNBChatWidget::OnGuessInputCommitted(const FText& InText, ETextCommit::Type InCommitMethod)
{
	if (InCommitMethod != ETextCommit::OnEnter || InText.IsEmptyOrWhitespace() == true)
	{
		return;
	}

	ANBPlayerController* PlayerController = Cast<ANBPlayerController>(GetOwningPlayer());
	if (IsValid(PlayerController) == true)
	{
		PlayerController->ServerRPCSubmitGuess(InText.ToString());
	}

	GuessInputTextBox->SetText(FText::GetEmpty());
}
