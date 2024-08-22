// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "Components/StaticMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/World.h"

#include "Kismet/GamePlayStatics.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	cameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Spring Arm"));
	cameraBoom->SetupAttachment(RootComponent);
	cameraBoom->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	cameraBoom->SetRelativeRotation(FRotator(0.0f, -20.0f, 350.0f));
	cameraBoom->TargetArmLength = 550.0f;

	followCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Follow Camera"));
	followCamera->SetupAttachment(cameraBoom, USpringArmComponent::SocketName);

	cameraBoom->bUsePawnControlRotation = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->JumpZVelocity = 650.0f;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &APlayerCharacter::MouseX);
	PlayerInputComponent->BindAxis("LookUp", this, &APlayerCharacter::MouseY);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
}

void APlayerCharacter::MoveForward(float value)
{
	cameraRotation = GetControlRotation();
	cameraRotation.Pitch = 0.0f;
	moveDirection = cameraRotation.Vector();

	GetCharacterMovement()->AddInputVector(moveDirection * value);
}

void APlayerCharacter::MoveRight(float value)
{
	cameraRotation = GetControlRotation();
	cameraRotation.Pitch = 0.0f;
	moveDirection = cameraRotation.RotateVector(FVector::RightVector);

	GetCharacterMovement()->AddInputVector(moveDirection * value);
}

void APlayerCharacter::MouseX(float value)
{
	float input = value * 100.0f * GetWorld()->GetDeltaSeconds();
	AddControllerYawInput(input);
}

void APlayerCharacter::MouseY(float value)
{
	float input = value * 100.0f * GetWorld()->GetDeltaSeconds();
	AddControllerPitchInput(input);
}