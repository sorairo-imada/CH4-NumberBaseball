// NBBaseballLibrary.cpp

#include "Library/NBBaseballLibrary.h"

FString UNBBaseballLibrary::GenerateSecretNumber(int32 InLength)
{
	TArray<int32> Candidates;
	for (int32 i = 1; i <= 9; ++i)
	{
		Candidates.Add(i);
	}

	InLength = FMath::Clamp(InLength, 1, Candidates.Num());

	FString SecretNumber;
	for (int32 i = 0; i < InLength; ++i)
	{
		// 뽑은 숫자는 후보에서 제거해 중복을 원천 차단한다.
		const int32 Index = FMath::RandRange(0, Candidates.Num() - 1);
		SecretNumber.AppendInt(Candidates[Index]);
		Candidates.RemoveAt(Index);
	}

	return SecretNumber;
}

ENBGuessError UNBBaseballLibrary::ValidateGuess(const FString& InGuess, int32 InLength)
{
	for (const TCHAR Char : InGuess)
	{
		// FChar::IsDigit()은 전각 숫자 등도 통과시키므로 범위를 직접 비교한다.
		if (Char < TEXT('0') || Char > TEXT('9'))
		{
			return ENBGuessError::NotDigit;
		}
	}

	if (InGuess.Len() != InLength)
	{
		return ENBGuessError::InvalidLength;
	}

	TSet<TCHAR> UniqueDigits;
	for (const TCHAR Char : InGuess)
	{
		if (Char == TEXT('0'))
		{
			return ENBGuessError::OutOfRange;
		}

		UniqueDigits.Add(Char);
	}

	if (UniqueDigits.Num() != InGuess.Len())
	{
		return ENBGuessError::Duplicated;
	}

	return ENBGuessError::None;
}

FNBJudgeResult UNBBaseballLibrary::JudgeGuess(const FString& InSecretNumber, const FString& InGuess)
{
	FNBJudgeResult JudgeResult;

	const int32 Length = FMath::Min(InSecretNumber.Len(), InGuess.Len());
	for (int32 i = 0; i < Length; ++i)
	{
		if (InSecretNumber[i] == InGuess[i])
		{
			++JudgeResult.StrikeCount;
		}
		else
		{
			int32 FoundIndex = INDEX_NONE;
			if (InSecretNumber.FindChar(InGuess[i], FoundIndex) == true)
			{
				++JudgeResult.BallCount;
			}
		}
	}

	return JudgeResult;
}

FString UNBBaseballLibrary::MakeJudgeResultString(const FNBJudgeResult& InJudgeResult)
{
	if (InJudgeResult.StrikeCount == 0 && InJudgeResult.BallCount == 0)
	{
		return TEXT("OUT");
	}

	return FString::Printf(TEXT("%dS%dB"), InJudgeResult.StrikeCount, InJudgeResult.BallCount);
}

FString UNBBaseballLibrary::GetGuessErrorMessage(ENBGuessError InGuessError, int32 InLength)
{
	switch (InGuessError)
	{
	case ENBGuessError::NotDigit:
		return TEXT("문자가 포함되어 있습니다. 숫자만 입력해주세요.");
	case ENBGuessError::InvalidLength:
		return FString::Printf(TEXT("%d자리 숫자를 입력해주세요."), InLength);
	case ENBGuessError::OutOfRange:
		return TEXT("1~9 사이의 숫자만 입력해주세요.");
	case ENBGuessError::Duplicated:
		return TEXT("중복되지 않은 숫자를 입력해주세요.");
	default:
		return FString();
	}
}
