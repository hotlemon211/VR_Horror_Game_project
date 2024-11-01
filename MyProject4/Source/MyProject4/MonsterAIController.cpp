// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAIController.h"

#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"

#include "Monster.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void AMonsterAIController::BeginPlay()
{
	Super::BeginPlay();

	playerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	navArea = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this);
	movementComponent = GetPawn()->FindComponentByClass<UCharacterMovementComponent>();

	// 최대 이동 속력을 저장함
	maxSpeed = maxWalkSpeed;

	StartSearchPlayer();
}

void AMonsterAIController::Tick(float deltaSeconds)
{
	bool isEnemyClostPlayer = IsEnemyClostPlayer();
	bool defaultIsMovePlayer = isMoveToPlayer;

	isMoveToPlayer = false;

	// 플레이어가 몬스터 근처에 있는 경우
	if (isEnemyClostPlayer && IsEnemyLookAtPlayer() || isEnemyClostPlayer && defaultIsMovePlayer)
	{
		isMoveToPlayer = true;
		isMoveToRandom = false;
		isWait = false;
		maxSpeed = maxRunSpeed;

		MoveToPlayer();
	}

	// 플레이어가 몬스터 근처에 없는 경우
	if (!isMoveToPlayer)
		StartSearchPlayer();

	IncreaseMoveSpeed(deltaSeconds);
	LookAtLocation(toLocation);
}

void AMonsterAIController::OnMoveCompleted(FAIRequestID requestID, const FPathFollowingResult& result)
{
	Super::OnMoveCompleted(requestID, result);

	// 플레이어를 찾는 경우
	if (!isMoveToPlayer && isMoveToRandom && !isWait)
	{
		UE_LOG(LogTemp, Log, TEXT("Random"));

		// 최소 대기 시간과 최대 대기 시간 사이에서 랜덤 대기 시간 선택
		float waitTime = FMath::RandRange(minWaitTime, maxWaitTime);
		isWait = true;
		isMoveToPlayer = false;
		isMoveToRandom = false;

		// 타이머 설정 로그 추가
		UE_LOG(LogTemp, Warning, TEXT("Setting Timer for %f seconds"), waitTime);

		// 대기 시간 후에 StartSearchPlayer 호출
		GetWorld()->GetTimerManager().SetTimer(waitMoveTimer, this, &AMonsterAIController::ChangeWait, waitTime, false);
	}
}

void AMonsterAIController::ChangeWait()
{
	isWait = false;
	MoveTo(GetPawn()->GetTargetLocation());
}

void AMonsterAIController::StartSearchPlayer()
{
	if (isWait || isMoveToRandom)
		return;

	maxSpeed = maxWalkSpeed;

	isCanAttackPlayer = true;
	isMoveToRandom = true;
	isWait = false;

	GenerateRandomSearchLocation();
	StartMoveSpeed();
	MoveTo(toLocation);
}

void AMonsterAIController::GenerateRandomSearchLocation()
{
	FNavLocation resultLocation;

	// 10000.0f 범위의 랜덤한 위치를 정함
	bool isSucceed = navArea->GetRandomReachablePointInRadius(GetPawn()->GetActorLocation(), 10000.0f, resultLocation);

	if (!isSucceed)
		return;

	UE_LOG(LogTemp, Error, TEXT("Random Location: %s"), *toLocation.ToString());

	// 랜덤한 위치로 지정
	toLocation = resultLocation.Location;
}

void AMonsterAIController::MoveToPlayer()
{
	if (playerPawn == nullptr)
		return;

	FVector tempToLocation = toLocation;

	toLocation = playerPawn->GetActorLocation();
	FNavLocation closeMoveablePos;

	if (UNavigationSystemV1* navSystem = UNavigationSystemV1::GetCurrent(GetWorld()))
	{
		// 해당 위치로 이동할 수 있는 경우
		if (navSystem->ProjectPointToNavigation(toLocation, closeMoveablePos))
			toLocation = closeMoveablePos.Location;

		// 해당 위치로 이동할 수 없는 경우
		else
		{
			// 플레이어 위치에서 NavSystem 상 가장 가까운 영역으로 이동함
			if (navSystem->GetRandomReachablePointInRadius(toLocation, 500.0f, closeMoveablePos))
				toLocation = closeMoveablePos.Location;

			// 원래 이동할려던 위치로 이동함
			else
				toLocation = tempToLocation;
		}
	}

	MoveToLocation(toLocation);
}

void AMonsterAIController::StartMoveSpeed()
{
	movementComponent->MaxWalkSpeed = 0.0f;
}

void AMonsterAIController::IncreaseMoveSpeed(float deltaSecond)
{
	if (movementComponent->MaxWalkSpeed >= maxSpeed)
		return;

	float accelerationRate = maxSpeed / increaseToMaxSpeedTime; // 초당 증가할 속도
	float currentSpeed = movementComponent->MaxWalkSpeed;		// 현재 이동 속도

	currentSpeed = FMath::Min(currentSpeed + accelerationRate * deltaSecond, maxSpeed);
	movementComponent->MaxWalkSpeed = currentSpeed;
}

void AMonsterAIController::LookAtLocation(FVector targetLocation)
{
	APawn* monster = GetPawn();
	if (monster == nullptr)
		return;

	// 몬스터의 이동 방향 벡터를 가져옴
	FVector monsterVelocity = monster->GetVelocity();

	// 이동 방향이 없으면 회전하지 않음
	if (monsterVelocity.IsNearlyZero())
		return;

	// Z축(상하 축)의 영향을 제거하고, 방향 벡터를 정규화
	FVector direction = monsterVelocity.GetSafeNormal();
	direction.Z = 0.0f;

	// 목표 회전 값(이동 방향)을 계산
	FRotator targetRotation = FRotationMatrix::MakeFromX(direction).Rotator();
	FRotator currentRotation = monster->GetActorRotation();

	// 현재 회전 값에서 목표 회전 값으로 선형 보간하여 회전
	FRotator newRotation = FMath::RInterpTo(currentRotation, targetRotation, GetWorld()->GetDeltaSeconds(), rotationSpeed);
	monster->SetActorRotation(newRotation);
}

bool AMonsterAIController::IsEnemyClostPlayer()
{
	if (playerPawn && GetPawn())
	{
		// 몬스터와 플레이어 간의 거리를 계산
		float distance = FVector::Dist(GetPawn()->GetActorLocation(), playerPawn->GetActorLocation());

		// 플레이어가 SearchRadius 범위 안에 있는지 확인
		return distance <= searchRadius;
	}

	// playerPawn이 유효하지 않은 경우 false 반환
	return false;
}

bool AMonsterAIController::IsEnemyLookAtPlayer()
{
	APawn* monster = GetPawn();

	// 플레이어와 몬스터 모두 유효해야 함
	if (playerPawn == nullptr || monster == nullptr)
		return false;

	// 몬스터 위치와 플레이어 위치를 구함
	FVector monsterLocation = monster->GetActorLocation();
	FVector playerLocation = playerPawn->GetActorLocation();

	// 몬스터 앞 방향과 플레이어와 몬스터 방향을 구함
	FVector monsterForward = monster->GetActorForwardVector();
	FVector directionToPlayer = (playerLocation - monsterLocation).GetSafeNormal();

	float dot = FVector::DotProduct(monsterForward, directionToPlayer);

	float viewAngleRad = FMath::DegreesToRadians(viewAngle / 2.0f);
	float angleBet = FMath::Acos(dot);

	// 각도가 시야 각도 내에 있는지 확인
	return angleBet <= viewAngleRad;
}

void AMonsterAIController::AttackPlayer()
{
	isWait = false;
}