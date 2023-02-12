// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enemy.generated.h"

UCLASS()
class SMALLOPENWORLD_API AEnemy : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;



protected:

	UFUNCTION(BlueprintCallable)
	void TakeDamage(float Damage_Value);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State)
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State)
	bool GetHit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State)
	bool isDead;
};
