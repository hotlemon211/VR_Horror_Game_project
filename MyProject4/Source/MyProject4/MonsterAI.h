// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MonsterAI.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT4_API AMonsterAI : public AAIController
{
	GENERATED_BODY()

protected:
	void BeginPlay() override;
	void Tick(float DeltaSeconds) override;

private:
	APawn* playerPawn;

	class AEnemyCharacter* controlledEnemy;
	class UNavigationSystemV1* navArea;

	FVector randomLocation = FVector();

	bool isMoveToPlayer;
	bool isSearchForPlayer;
	bool isCanAttackPlayer;

	float playerStoppingDistance = 60.0f;
	float playerAttackDistacne = 200.0f;

	void GenerateRandomSearchLocation();
	void SearchForPlayer();

	void MoveToPlayer();
	void StartChasingPlayer();
	bool IsEnemyCloseToPlayer();
	void AttackPlayer();

	FTimerHandle searchTimerHandle;
	FTimerHandle attackTimerHandle;

public:
	void OnMoveCompleted(FAIRequestID requestID, const FPathFollowingResult& result) override;

	UFUNCTION()
	void OnDetectPlayerBeginOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, class UPrimitiveComponent* otherComp, int32 otherBodyIndex);

	UFUNCTION()
	void OnDetectPlayerEndOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, class UPrimitiveComponent* otherComp, int32 otherBodyIndex);
};
