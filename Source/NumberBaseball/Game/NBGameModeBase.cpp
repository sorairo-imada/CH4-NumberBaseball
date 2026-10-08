// NBGameModeBase.cpp

#include "Game/NBGameModeBase.h"

#include "Game/NBGameStateBase.h"
#include "Library/NBBaseballLibrary.h"
#include "NumberBaseball.h"
#include "Player/NBPlayerController.h"
#include "Player/NBPlayerState.h"
#include "TimerManager.h"

ANBGameModeBase::ANBGameModeBase()
	: SecretNumberLength(3)
	, ResetDelay(5.f)
	, bIsGameInProgress(false)
	, JoinedPlayerCount(0)
{
	GameStateClass = ANBGameStateBase::StaticClass();
	PlayerControllerClass = ANBPlayerController::StaticClass();
	PlayerStateClass = ANBPlayerState::StaticClass();
	DefaultPawnClass = nullptr;
}

void ANBGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	StartNewGame();
}

void ANBGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	ANBPlayerController* NBPlayerController = Cast<ANBPlayerController>(NewPlayer);
	if (IsValid(NBPlayerController) == false || IsValid(NBPlayerController->PlayerState) == false)
	{
		return;
	}

	const FString PlayerName = FString::Printf(TEXT("Player%d"), ++JoinedPlayerCount);
	NBPlayerController->PlayerState->SetPlayerName(PlayerName);

	BroadcastChatMessage(FString::Printf(TEXT("%s 님이 입장했습니다."), *PlayerName));
	NBPlayerController->ClientRPCPrintSystemMessage(FString::Printf(TEXT("1~9 사이의 중복 없는 %d자리 숫자를 입력하세요."), SecretNumberLength));
}

void ANBGameModeBase::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	// 기회가 남은 유일한 플레이어가 나가면 남은 사람들은 끝나지 않는 게임에 갇힌다.
	if (bIsGameInProgress == true && HasEveryPlayerUsedAllAttempts() == true)
	{
		FinishGame(FString::Printf(TEXT("무승부! 정답은 %s 입니다."), *SecretNumber));
	}
}

void ANBGameModeBase::HandleGuess(ANBPlayerController* InGuessingPlayerController, const FString& InGuess)
{
	if (IsValid(InGuessingPlayerController) == false)
	{
		return;
	}

	ANBPlayerState* NBPlayerState = InGuessingPlayerController->GetPlayerState<ANBPlayerState>();
	if (IsValid(NBPlayerState) == false)
	{
		return;
	}

	if (bIsGameInProgress == false)
	{
		InGuessingPlayerController->ClientRPCPrintSystemMessage(TEXT("곧 새 게임이 시작됩니다. 잠시만 기다려주세요."));
		return;
	}

	if (NBPlayerState->HasRemainingAttempts() == false)
	{
		InGuessingPlayerController->ClientRPCPrintSystemMessage(TEXT("기회를 모두 사용했습니다. 다른 플레이어를 기다려주세요."));
		return;
	}

	const FString Guess = InGuess.TrimStartAndEnd();
	const ENBGuessError GuessError = UNBBaseballLibrary::ValidateGuess(Guess, SecretNumberLength);
	if (GuessError != ENBGuessError::None)
	{
		const FString ErrorMessage = UNBBaseballLibrary::GetGuessErrorMessage(GuessError, SecretNumberLength);
		InGuessingPlayerController->ClientRPCPrintSystemMessage(FString::Printf(TEXT("%s 다시 입력하세요."), *ErrorMessage));
		return;
	}

	NBPlayerState->IncreaseAttempts();

	const FNBJudgeResult JudgeResult = UNBBaseballLibrary::JudgeGuess(SecretNumber, Guess);
	BroadcastChatMessage(FString::Printf(TEXT("%s: %s → %s %s"),
		*NBPlayerState->GetPlayerName(),
		*Guess,
		*UNBBaseballLibrary::MakeJudgeResultString(JudgeResult),
		*NBPlayerState->GetAttemptsString()));

	JudgeGame(NBPlayerState, JudgeResult.StrikeCount);
}

void ANBGameModeBase::StartNewGame()
{
	SecretNumber = UNBBaseballLibrary::GenerateSecretNumber(SecretNumberLength);
	bIsGameInProgress = true;

	UE_LOG(LogNumberBaseball, Log, TEXT("SecretNumber: %s"), *SecretNumber);
}

void ANBGameModeBase::JudgeGame(const ANBPlayerState* InGuessingPlayerState, int32 InStrikeCount)
{
	if (InStrikeCount == SecretNumberLength)
	{
		FinishGame(FString::Printf(TEXT("%s 승리! 정답은 %s 입니다."), *InGuessingPlayerState->GetPlayerName(), *SecretNumber));
	}
	else if (HasEveryPlayerUsedAllAttempts() == true)
	{
		FinishGame(FString::Printf(TEXT("무승부! 정답은 %s 입니다."), *SecretNumber));
	}
}

bool ANBGameModeBase::HasEveryPlayerUsedAllAttempts() const
{
	if (IsValid(GameState) == false || GameState->PlayerArray.IsEmpty() == true)
	{
		return false;
	}

	for (const APlayerState* PlayerState : GameState->PlayerArray)
	{
		const ANBPlayerState* NBPlayerState = Cast<ANBPlayerState>(PlayerState);
		if (IsValid(NBPlayerState) == true && NBPlayerState->HasRemainingAttempts() == true)
		{
			return false;
		}
	}

	return true;
}

void ANBGameModeBase::FinishGame(const FString& InNotification)
{
	// 리셋 전까지 들어오는 입력은 HandleGuess()에서 거절된다.
	bIsGameInProgress = false;

	ANBGameStateBase* NBGameState = GetGameState<ANBGameStateBase>();
	if (IsValid(NBGameState) == true)
	{
		NBGameState->SetNotification(InNotification);
	}

	if (ResetDelay > 0.f)
	{
		GetWorldTimerManager().SetTimer(ResetTimerHandle, this, &ThisClass::ResetGame, ResetDelay, false);
	}
	else
	{
		ResetGame();
	}
}

void ANBGameModeBase::ResetGame()
{
	ANBGameStateBase* NBGameState = GetGameState<ANBGameStateBase>();
	if (IsValid(NBGameState) == true)
	{
		NBGameState->SetNotification(FString());

		for (APlayerState* PlayerState : NBGameState->PlayerArray)
		{
			ANBPlayerState* NBPlayerState = Cast<ANBPlayerState>(PlayerState);
			if (IsValid(NBPlayerState) == true)
			{
				NBPlayerState->ResetAttempts();
			}
		}
	}

	StartNewGame();

	BroadcastChatMessage(TEXT("새 게임이 시작되었습니다. 숫자를 입력하세요!"));
}

void ANBGameModeBase::BroadcastChatMessage(const FString& InMessage) const
{
	ANBGameStateBase* NBGameState = GetGameState<ANBGameStateBase>();
	if (IsValid(NBGameState) == true)
	{
		NBGameState->MulticastRPCBroadcastChatMessage(InMessage);
	}
}
