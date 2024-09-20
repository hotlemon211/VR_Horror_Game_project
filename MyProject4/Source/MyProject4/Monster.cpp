// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster.h"
#include "PlayerCharacter.h"

#include "Animation/AnimInstance.h"
#include "Components/AudioComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Sound/SoundBase.h"

// Sets default values
AMonster::AMonster()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	detectPlayerCollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Collision Sphere"));
	detectPlayerCollisionSphere->SetupAttachment(RootComponent);

	isPlayStep1 = false;
	
	audioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio Component"));
	audioComponent->bAutoActivate = false;
	audioComponent->bOverrideAttenuation = true;
	
	soundAttenuation = CreateDefaultSubobject<USoundAttenuation>(TEXT("Sound Attenuation"));
	
	FSoundAttenuationSettings attenuationSettings;
	attenuationSettings.AttenuationShape = EAttenuationShape::Sphere;
	attenuationSettings.AttenuationShapeExtents = FVector(50.0f);
	attenuationSettings.FalloffDistance = 1500.0f;
	
	soundAttenuation->Attenuation = attenuationSettings;
	audioComponent->AttenuationSettings = soundAttenuation;
}

// Called when the game starts or when spawned
void AMonster::BeginPlay()
{
	Super::BeginPlay();

	UCharacterMovementComponent* movementComponent = GetCharacterMovement();

	movementComponent->MaxWalkSpeed = 200.0f;
}

// Called every frame
void AMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

USphereComponent* AMonster::GetDetectPlayerCollisionSphere()
{
	return detectPlayerCollisionSphere;
}