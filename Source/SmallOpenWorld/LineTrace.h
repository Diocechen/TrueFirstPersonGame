// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LineTrace.generated.h"

UCLASS()
class SMALLOPENWORLD_API ALineTrace : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALineTrace();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	AActor* LineTrace(FVector Start, FVector End);
};
