// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class MYPROJECT4_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = Camemra)
	class USpringArmComponent* cameraBoom;

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = Camemra)
	class UCameraComponent* followCamera;

protected:
	void MoveForward(float value);
	void MoveRight(float value);

	void MouseX(float value);
	void MouseY(float value);

	FRotator cameraRotation = FRotator();
	FVector moveDirection = FVector();
};
