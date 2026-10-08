// NBGameModeBase.h

#pragma once

#include "GameFramework/GameModeBase.h"
#include "NBGameModeBase.generated.h"

class ANBPlayerController;
class ANBPlayerState;

/**
 * 숫자 야구의 심판. 정답은 서버에만 존재하며 복제하지 않는다.
 */
UCLASS()
class NUMBERBASEBALL_API ANBGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ANBGameModeBase();

	virtual void BeginPlay() override;

	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void Logout(AController* Exiting) override;

	// 채팅창에 입력된 모든 문자열은 정답 시도로 취급한다.
	void HandleGuess(ANBPlayerController* InGuessingPlayerController, const FString& InGuess);

private:
	void StartNewGame();

	// 승리/무승부 여부를 판단하고, 게임이 끝났다면 공지 후 리셋을 예약한다.
	void JudgeGame(const ANBPlayerState* InGuessingPlayerState, int32 InStrikeCount);

	bool HasEveryPlayerUsedAllAttempts() const;

	void FinishGame(const FString& InNotification);

	void ResetGame();

	void BroadcastChatMessage(const FString& InMessage) const;

private:
	UPROPERTY(EditDefaultsOnly, Category = "NumberBaseball", Meta = (ClampMin = 1, ClampMax = 9))
	int32 SecretNumberLength;

	// 결과 공지를 띄운 뒤 다음 게임을 시작하기까지의 대기 시간(초).
	UPROPERTY(EditDefaultsOnly, Category = "NumberBaseball", Meta = (ClampMin = 0))
	float ResetDelay;

	FString SecretNumber;

	bool bIsGameInProgress;

	int32 JoinedPlayerCount;

	FTimerHandle ResetTimerHandle;

};
