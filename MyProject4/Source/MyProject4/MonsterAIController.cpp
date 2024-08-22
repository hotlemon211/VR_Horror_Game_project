// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAIController.h"

#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"

#include "Monster.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void AMonsterAIController::OnMoveCompleted(FAIRequestID requestID, const FPathFollowingResult& result)
{
	Super::OnMoveCompleted(requestID, result);

	// �÷��̾ ã�� ���
	if (!isMoveToPlayer && isMoveToRandom && !isWait)
	{
		UE_LOG(LogTemp, Log, TEXT("Random"));

		// �ּ� ��� �ð��� �ִ� ��� �ð� ���̿��� ���� ��� �ð� ����
		float waitTime = FMath::RandRange(minWaitTime, maxWaitTime);
		isWait = true;
		isMoveToPlayer = false;
		isMoveToRandom = false;

		// Ÿ�̸� ���� �α� �߰�
		UE_LOG(LogTemp, Warning, TEXT("Setting Timer for %f seconds"), waitTime);

		// ��� �ð� �Ŀ� StartSearchPlayer ȣ��
		GetWorld()->GetTimerManager().SetTimer(waitMoveTimer, this, &AMonsterAIController::ChangeWait, waitTime, false);
	}
}

void AMonsterAIController::ChangeWait()
{
	isWait = false;
	MoveTo(GetPawn()->GetTargetLocation());
}

void AMonsterAIController::BeginPlay()
{
	Super::BeginPlay();

	playerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	navArea = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this);

	StartSearchPlayer();
}

void AMonsterAIController::Tick(float deltaSeconds)
{
	bool isEnemyClostPlayer = IsEnemyClostPlayer();
	bool defaultIsMovePlayer = isMoveToPlayer;

	isMoveToPlayer = false;

	// �÷��̾ ���� ��ó�� �ִ� ���
	if (isEnemyClostPlayer && IsEnemyLookAtPlayer() || isEnemyClostPlayer && defaultIsMovePlayer)
	{
		isMoveToPlayer = true;
		isMoveToRandom = false;
		isWait = false;

		MoveToPlayer();
	}

	// �÷��̾ ���� ��ó�� ���� ���
	if (!isMoveToPlayer)
		StartSearchPlayer();

	LookAtLocation(toLocation);
}

void AMonsterAIController::StartSearchPlayer()
{
	if (isWait || isMoveToRandom)
		return;

	isCanAttackPlayer = true;
	isMoveToRandom = true;
	isWait = false;

	GenerateRandomSearchLocation();
	MoveTo(toLocation);
}

void AMonsterAIController::GenerateRandomSearchLocation()
{
	FNavLocation resultLocation;

	// 10000.0f ������ ������ ��ġ�� ����
	bool isSucceed = navArea->GetRandomReachablePointInRadius(GetPawn()->GetActorLocation(), 10000.0f, resultLocation);

	if (!isSucceed)
		return;

	UE_LOG(LogTemp, Error, TEXT("Random Location: %s"), *toLocation.ToString());

	// ������ ��ġ�� ����
	toLocation = resultLocation.Location;
}

void AMonsterAIController::MoveToPlayer()
{
	if (playerPawn == nullptr)
		return;

	toLocation = playerPawn->GetActorLocation();
	UE_LOG(LogTemp, Warning, TEXT("Player Location: %s"), *toLocation.ToString());

	MoveToLocation(toLocation);
}

void AMonsterAIController::LookAtLocation(FVector targetLocation)
{
	APawn* monster = GetPawn();
	if (monster == nullptr)
		return;

	// ������ �̵� ���� ���͸� ������
	FVector monsterVelocity = monster->GetVelocity();

	// �̵� ������ ������ ȸ������ ����
	if (monsterVelocity.IsNearlyZero())
		return;

	// Z��(���� ��)�� ������ �����ϰ�, ���� ���͸� ����ȭ
	FVector direction = monsterVelocity.GetSafeNormal();
	direction.Z = 0.0f;

	// ��ǥ ȸ�� ��(�̵� ����)�� ���
	FRotator targetRotation = FRotationMatrix::MakeFromX(direction).Rotator();
	FRotator currentRotation = monster->GetActorRotation();

	// ���� ȸ�� ������ ��ǥ ȸ�� ������ ���� �����Ͽ� ȸ��
	FRotator newRotation = FMath::RInterpTo(currentRotation, targetRotation, GetWorld()->GetDeltaSeconds(), rotationSpeed);
	monster->SetActorRotation(newRotation);
}

bool AMonsterAIController::IsEnemyClostPlayer()
{
	if (playerPawn && GetPawn())
	{
		// ���Ϳ� �÷��̾� ���� �Ÿ��� ���
		float distance = FVector::Dist(GetPawn()->GetActorLocation(), playerPawn->GetActorLocation());

		// �÷��̾ SearchRadius ���� �ȿ� �ִ��� Ȯ��
		return distance <= searchRadius;
	}

	// playerPawn�� ��ȿ���� ���� ��� false ��ȯ
	return false;
}

bool AMonsterAIController::IsEnemyLookAtPlayer()
{
	APawn* monster = GetPawn();

	// �÷��̾�� ���� ��� ��ȿ�ؾ� ��
	if (playerPawn == nullptr || monster == nullptr)
		return false;

	// ���� ��ġ�� �÷��̾� ��ġ�� ����
	FVector monsterLocation = monster->GetActorLocation();
	FVector playerLocation = playerPawn->GetActorLocation();

	// ���� �� ����� �÷��̾�� ���� ������ ����
	FVector monsterForward = monster->GetActorForwardVector();
	FVector directionToPlayer = (playerLocation - monsterLocation).GetSafeNormal();

	float dot = FVector::DotProduct(monsterForward, directionToPlayer);

	float viewAngleRad = FMath::DegreesToRadians(viewAngle / 2.0f);
	float angleBet = FMath::Acos(dot);

	// ������ �þ� ���� ���� �ִ��� Ȯ��
	return angleBet <= viewAngleRad;
}

void AMonsterAIController::AttackPlayer()
{
	isWait = false;
}