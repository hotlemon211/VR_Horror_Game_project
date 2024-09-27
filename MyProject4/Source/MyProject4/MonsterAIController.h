// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MonsterAIController.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT4_API AMonsterAIController : public AAIController
{
	GENERATED_BODY()

public:
	// ==============================================================
	//							Public Method
	// ==============================================================

	void OnMoveCompleted(FAIRequestID requestID, const FPathFollowingResult& result) override;

	// ==============================================================
	//							Public Variable
	// ==============================================================

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float searchRadius = 3000.0f;				// 플레이어 인식 가능한 거리

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float playerStoppingDistance = 60.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float playerAttackDistance = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float maxWalkSpeed = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float maxRunSpeed = 400.0f;

	UPROPERTY(EditAnywhere, Category = "AI")
	float viewAngle = 120.0f;					// 몬스터 시야 각도

	UPROPERTY(EditAnywhere, Category = "AI")
	float minWaitTime = 1.0f;					// 최소 대기 시간 (초)

	UPROPERTY(EditAnywhere, Category = "AI")
	float maxWaitTime = 3.0f;					// 최대 대기 시간 (초)

	UPROPERTY(EditAnywhere, Category = "AI")
	float rotationSpeed = 5.0f; // 회전 속도 (초당 도)
	
protected:
	void BeginPlay() override;
	void Tick(float deltaSeconds);

private:
	// ==============================================================
	//							Private Method
	// ==============================================================

	void StartSearchPlayer();
	void GenerateRandomSearchLocation();

	void MoveToPlayer();
	void StartMoveSpeed();
	void IncreaseMoveSpeed(float deltaSecond);
	void LookAtLocation(FVector targetLocation);

	bool IsEnemyClostPlayer();
	bool IsEnemyLookAtPlayer();

	void AttackPlayer();

	void ChangeWait();

	// ==============================================================
	//							Private Variable
	// ==============================================================

	APawn* playerPawn;										// 플레이어 Pawn

	class UNavigationSystemV1* navArea;						// 네비게이션 시스템
	class UCharacterMovementComponent* movementComponent;	// 이동 관련 컴포넌트
		
	FTimerHandle attackTimerHandle;							// 공격 대기 타임머
	FTimerHandle waitMoveTimer;								// 다음 이동 타임머

	FVector toLocation = FVector();							// 몬스터가 이동할 위치

	float maxSpeed;											// 몬스터의 최대 이동 속도
	float increaseToMaxSpeedTime = 0.5f;					// 최대 속력까지 증가하는데 걸리는 시간
	
	bool isWait;
	bool isMoveToPlayer;
	bool isMoveToRandom;
	bool isCanAttackPlayer;
};
