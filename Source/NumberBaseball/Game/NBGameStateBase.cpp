// NBGameStateBase.cpp

#include "Game/NBGameStateBase.h"

#include "Engine/Engine.h"
#include "Net/UnrealNetwork.h"
#include "Player/NBPlayerController.h"

namespace
{
	// 데디케이티드 서버에서는 nullptr.
	ANBPlayerController* GetLocalNBPlayerController(const UObject* InWorldContextObject)
	{
		if (GEngine == nullptr)
		{
			return nullptr;
		}

		return Cast<ANBPlayerController>(GEngine->GetFirstLocalPlayerController(InWorldContextObject->GetWorld()));
	}
}

void ANBGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, Notification);
}

void ANBGameStateBase::MulticastRPCBroadcastChatMessage_Implementation(const FString& InMessage)
{
	ANBPlayerController* PlayerController = GetLocalNBPlayerController(this);
	if (IsValid(PlayerController) == true)
	{
		PlayerController->AddChatMessage(InMessage);
	}
}

void ANBGameStateBase::SetNotification(const FString& InNotification)
{
	if (HasAuthority() == false)
	{
		return;
	}

	Notification = InNotification;

	// 리슨 서버 호스트는 OnRep이 호출되지 않으므로 직접 호출한다.
	OnRep_Notification();
}

void ANBGameStateBase::OnRep_Notification()
{
	ANBPlayerController* PlayerController = GetLocalNBPlayerController(this);
	if (IsValid(PlayerController) == true)
	{
		PlayerController->ShowNotification(Notification);
	}
}
