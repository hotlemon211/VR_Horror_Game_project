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
	float searchRadius = 3000.0f;				// �÷��̾� �ν� ������ �Ÿ�

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float playerStoppingDistance = 60.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float playerAttackDistance = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float maxWalkSpeed = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float maxRunSpeed = 400.0f;

	UPROPERTY(EditAnywhere, Category = "AI")
	float viewAngle = 120.0f;					// ���� �þ� ����

	UPROPERTY(EditAnywhere, Category = "AI")
	float minWaitTime = 1.0f;					// �ּ� ��� �ð� (��)

	UPROPERTY(EditAnywhere, Category = "AI")
	float maxWaitTime = 3.0f;					// �ִ� ��� �ð� (��)

	UPROPERTY(EditAnywhere, Category = "AI")
	float rotationSpeed = 5.0f; // ȸ�� �ӵ� (�ʴ� ��)
	
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

	APawn* playerPawn;										// �÷��̾� Pawn

	class UNavigationSystemV1* navArea;						// �׺���̼� �ý���
	class UCharacterMovementComponent* movementComponent;	// �̵� ���� ������Ʈ
		
	FTimerHandle attackTimerHandle;							// ���� ��� Ÿ�Ӹ�
	FTimerHandle waitMoveTimer;								// ���� �̵� Ÿ�Ӹ�

	FVector toLocation = FVector();							// ���Ͱ� �̵��� ��ġ

	float maxSpeed;											// ������ �ִ� �̵� �ӵ�
	float increaseToMaxSpeedTime = 0.5f;					// �ִ� �ӷ±��� �����ϴµ� �ɸ��� �ð�
	
	bool isWait;
	bool isMoveToPlayer;
	bool isMoveToRandom;
	bool isCanAttackPlayer;
};
