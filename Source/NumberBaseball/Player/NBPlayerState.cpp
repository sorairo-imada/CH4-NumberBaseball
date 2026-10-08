// NBPlayerState.cpp

#include "Player/NBPlayerState.h"

#include "Net/UnrealNetwork.h"
#include "Player/NBPlayerController.h"

ANBPlayerState::ANBPlayerState()
	: CurrentAttempts(0)
	, MaxAttempts(3)
{
	bReplicates = true;
}

void ANBPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, CurrentAttempts);
	DOREPLIFETIME(ThisClass, MaxAttempts);
}

void ANBPlayerState::OnRep_PlayerName()
{
	Super::OnRep_PlayerName();

	NotifyAttemptsChanged();
}

FString ANBPlayerState::GetAttemptsString() const
{
	return FString::Printf(TEXT("[%d / %d]"), CurrentAttempts, MaxAttempts);
}

void ANBPlayerState::IncreaseAttempts()
{
	if (HasAuthority() == false)
	{
		return;
	}

	CurrentAttempts = FMath::Min(CurrentAttempts + 1, MaxAttempts);
	NotifyAttemptsChanged();
}

void ANBPlayerState::ResetAttempts()
{
	if (HasAuthority() == false)
	{
		return;
	}

	CurrentAttempts = 0;
	NotifyAttemptsChanged();
}

void ANBPlayerState::OnRep_Attempts()
{
	NotifyAttemptsChanged();
}

void ANBPlayerState::NotifyAttemptsChanged() const
{
	// OnRep은 서버에서 호출되지 않으므로, 리슨 서버 호스트의 UI도 같은 경로로 갱신한다.
	ANBPlayerController* PlayerController = Cast<ANBPlayerController>(GetPlayerController());
	if (IsValid(PlayerController) == true && PlayerController->IsLocalController() == true)
	{
		PlayerController->RefreshAttempts();
	}
}
