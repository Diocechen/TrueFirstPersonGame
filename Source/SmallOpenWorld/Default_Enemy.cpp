// Fill out your copyright notice in the Description page of Project Settings.


#include "Default_Enemy.h"

// Sets default values
ADefault_Enemy::ADefault_Enemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Health = 100.0;
	GetHit = false;
	isDead = false;
}

// Called when the game starts or when spawned
void ADefault_Enemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADefault_Enemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADefault_Enemy::TakeDamage(float Damage_Value)
{
	if (Health > 0.0f && !GetHit) {
		GetHit = true;
		Health -= Damage_Value;
		UE_LOG(LogTemp, Warning, TEXT("%f"), Health);
	}

	if (Health <= 0.0f) {
		Health = 0.0f;
		isDead = true;
	}
}


