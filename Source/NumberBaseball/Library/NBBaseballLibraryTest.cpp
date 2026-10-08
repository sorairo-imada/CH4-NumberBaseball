// NBBaseballLibraryTest.cpp

#include "Library/NBBaseballLibrary.h"
#include "Misc/AutomationTest.h"

#if WITH_DEV_AUTOMATION_TESTS

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FNBBaseballLibraryTest, "NumberBaseball.Library", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FNBBaseballLibraryTest::RunTest(const FString& Parameters)
{
	for (int32 i = 0; i < 1000; ++i)
	{
		const FString SecretNumber = UNBBaseballLibrary::GenerateSecretNumber(3);
		if (UNBBaseballLibrary::ValidateGuess(SecretNumber, 3) != ENBGuessError::None)
		{
			AddError(FString::Printf(TEXT("Generated an invalid secret number: %s"), *SecretNumber));
			break;
		}
	}

	TestEqual(TEXT("386"), UNBBaseballLibrary::ValidateGuess(TEXT("386"), 3), ENBGuessError::None);
	TestEqual(TEXT("12"), UNBBaseballLibrary::ValidateGuess(TEXT("12"), 3), ENBGuessError::InvalidLength);
	TestEqual(TEXT("1234"), UNBBaseballLibrary::ValidateGuess(TEXT("1234"), 3), ENBGuessError::InvalidLength);
	TestEqual(TEXT("empty"), UNBBaseballLibrary::ValidateGuess(TEXT(""), 3), ENBGuessError::InvalidLength);
	TestEqual(TEXT("abc"), UNBBaseballLibrary::ValidateGuess(TEXT("abc"), 3), ENBGuessError::NotDigit);
	TestEqual(TEXT("1a3"), UNBBaseballLibrary::ValidateGuess(TEXT("1a3"), 3), ENBGuessError::NotDigit);
	TestEqual(TEXT("-12"), UNBBaseballLibrary::ValidateGuess(TEXT("-12"), 3), ENBGuessError::NotDigit);
	TestEqual(TEXT("1 2"), UNBBaseballLibrary::ValidateGuess(TEXT("1 2"), 3), ENBGuessError::NotDigit);
	TestEqual(TEXT("112"), UNBBaseballLibrary::ValidateGuess(TEXT("112"), 3), ENBGuessError::Duplicated);
	TestEqual(TEXT("102"), UNBBaseballLibrary::ValidateGuess(TEXT("102"), 3), ENBGuessError::OutOfRange);

	auto Judge = [](const TCHAR* InGuess)
	{
		return UNBBaseballLibrary::MakeJudgeResultString(UNBBaseballLibrary::JudgeGuess(TEXT("386"), InGuess));
	};

	TestEqual(TEXT("127"), Judge(TEXT("127")), FString(TEXT("OUT")));
	TestEqual(TEXT("167"), Judge(TEXT("167")), FString(TEXT("0S1B")));
	TestEqual(TEXT("123"), Judge(TEXT("123")), FString(TEXT("0S1B")));
	TestEqual(TEXT("368"), Judge(TEXT("368")), FString(TEXT("1S2B")));
	TestEqual(TEXT("638"), Judge(TEXT("638")), FString(TEXT("0S3B")));
	TestEqual(TEXT("386"), Judge(TEXT("386")), FString(TEXT("3S0B")));

	return true;
}

#endif
