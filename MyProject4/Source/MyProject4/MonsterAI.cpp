// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAI.h"

#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"

#include "MonsterAI.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

void AMonsterAI::BeginPlay()
{
	Super::BeginPlay();

	playerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	navArea = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this);

	isSearchForPlayer = true;
	isCanAttackPlayer = true;
	isMoveToPlayer = false;

	GenerateRandomSearchLocation();
	SearchForPlayer();
}

void AMonsterAI::Tick(float DeltaSeconds)
{
}

void AMonsterAI::GenerateRandomSearchLocation()
{
	//randomLocation = navArea->GetRandomReachablePointInRadius(this, GetPawn()->GetActorLocation(), 10000.0f);
}

void AMonsterAI::SearchForPlayer()
{
	MoveToLocation(randomLocation);
}

void AMonsterAI::MoveToPlayer()
{
}

void AMonsterAI::StartChasingPlayer()
{
}

bool AMonsterAI::IsEnemyCloseToPlayer()
{
	return false;
}

void AMonsterAI::AttackPlayer()
{
}

void AMonsterAI::OnMoveCompleted(FAIRequestID requestID, const FPathFollowingResult& result)
{
}

void AMonsterAI::OnDetectPlayerBeginOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex)
{
}

void AMonsterAI::OnDetectPlayerEndOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex)
{
}
