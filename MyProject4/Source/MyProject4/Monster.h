// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Monster.generated.h"

UCLASS()
class MYPROJECT4_API AMonster : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMonster();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere)
	class USphereComponent* detectPlayerCollisionSphere;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sound")
	class UAudioComponent* audioComponent;

	UPROPERTY(visibleAnywhere, BlueprintReadOnly, Category = "Sound")
	class USoundAttenuation* soundAttenuation;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	class USphereComponent* GetDetectPlayerCollisionSphere();

private:
	

	bool isPlayStep1;
};
