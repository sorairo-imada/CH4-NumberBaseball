// NBGameStateBase.h

#pragma once

#include "GameFramework/GameStateBase.h"
#include "NBGameStateBase.generated.h"

/**
 * 모든 클라이언트가 함께 봐야 하는 것(채팅 로그, 게임 결과 공지)을 전달한다.
 */
UCLASS()
class NUMBERBASEBALL_API ANBGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPCBroadcastChatMessage(const FString& InMessage);

	// 빈 문자열이면 공지를 숨긴다. 서버 전용.
	void SetNotification(const FString& InNotification);

	const FString& GetNotification() const { return Notification; }

private:
	UFUNCTION()
	void OnRep_Notification();

private:
	// RPC가 아닌 프로퍼티로 복제해서 뒤늦게 접속한 플레이어도 현재 공지를 볼 수 있게 한다.
	UPROPERTY(ReplicatedUsing = OnRep_Notification)
	FString Notification;

};
