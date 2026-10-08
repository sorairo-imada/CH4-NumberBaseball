// NBNotificationWidget.cpp

#include "UI/NBNotificationWidget.h"

#include "Components/TextBlock.h"

void UNBNotificationWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ShowNotification(FString());
}

void UNBNotificationWidget::ShowNotification(const FString& InNotification)
{
	NotificationText->SetText(FText::FromString(InNotification));

	SetVisibility(InNotification.IsEmpty() == true ? ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible);
}
