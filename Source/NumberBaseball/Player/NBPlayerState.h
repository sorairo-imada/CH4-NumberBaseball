// NBPlayerState.h

#pragma once

#include "GameFramework/PlayerState.h"
#include "NBPlayerState.generated.h"

/**
 * 플레이어별 시도 횟수를 보관하고 복제한다. 값 변경은 서버에서만 한다.
 */
UCLASS()
class NUMBERBASEBALL_API ANBPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ANBPlayerState();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void OnRep_PlayerName() override;

	int32 GetCurrentAttempts() const { return CurrentAttempts; }

	int32 GetMaxAttempts() const { return MaxAttempts; }

	bool HasRemainingAttempts() const { return CurrentAttempts < MaxAttempts; }

	// "[현재 시도 횟수 / 최대 시도 횟수]"
	FString GetAttemptsString() const;

	void IncreaseAttempts();

	void ResetAttempts();

private:
	UFUNCTION()
	void OnRep_Attempts();

	void NotifyAttemptsChanged() const;

private:
	UPROPERTY(ReplicatedUsing = OnRep_Attempts)
	int32 CurrentAttempts;

	UPROPERTY(ReplicatedUsing = OnRep_Attempts, EditDefaultsOnly, Category = "NumberBaseball")
	int32 MaxAttempts;

};
