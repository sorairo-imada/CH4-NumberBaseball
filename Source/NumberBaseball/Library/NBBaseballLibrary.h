// NBBaseballLibrary.h

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "NBBaseballLibrary.generated.h"

UENUM(BlueprintType)
enum class ENBGuessError : uint8
{
	None,
	NotDigit,
	InvalidLength,
	OutOfRange,
	Duplicated,
};

USTRUCT(BlueprintType)
struct FNBJudgeResult
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	int32 StrikeCount = 0;

	UPROPERTY(BlueprintReadOnly)
	int32 BallCount = 0;
};

/**
 * 숫자 야구 규칙만 담당하는 순수 함수 모음. 네트워크/월드 상태에 의존하지 않는다.
 */
UCLASS()
class NUMBERBASEBALL_API UNBBaseballLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// 1~9 사이의 서로 다른 숫자 InLength개로 이루어진 정답 문자열을 만든다.
	UFUNCTION(BlueprintCallable, Category = "NumberBaseball")
	static FString GenerateSecretNumber(int32 InLength = 3);

	UFUNCTION(BlueprintPure, Category = "NumberBaseball")
	static ENBGuessError ValidateGuess(const FString& InGuess, int32 InLength = 3);

	// InGuess는 ValidateGuess()를 통과한 값이어야 한다.
	UFUNCTION(BlueprintPure, Category = "NumberBaseball")
	static FNBJudgeResult JudgeGuess(const FString& InSecretNumber, const FString& InGuess);

	// "1S1B", "3S0B", "OUT" 형태의 문자열로 변환한다.
	UFUNCTION(BlueprintPure, Category = "NumberBaseball")
	static FString MakeJudgeResultString(const FNBJudgeResult& InJudgeResult);

	UFUNCTION(BlueprintPure, Category = "NumberBaseball")
	static FString GetGuessErrorMessage(ENBGuessError InGuessError, int32 InLength = 3);

};
