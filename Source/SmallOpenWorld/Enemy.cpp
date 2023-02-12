// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Health = 100.0f;
	GetHit = false;
	isDead = false;
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemy::TakeDamage(float Damage_Value)
{
	if (Health > 0.0f && !GetHit) {
		GetHit = true;
		Health -= Damage_Value;
		UE_LOG(LogTemp, Warning, TEXT("696969%f"), Health);
	}

	if(Health <= 0.0f){
		Health = 0.0f;
		isDead = true;
	}
}

