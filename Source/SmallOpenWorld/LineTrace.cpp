// Fill out your copyright notice in the Description page of Project Settings.


#include "LineTrace.h"

ALineTrace::ALineTrace()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALineTrace::BeginPlay()
{
	Super::BeginPlay();
	
}

AActor* ALineTrace::LineTrace(FVector Start, FVector End)
{
	FHitResult Hit;
	FCollisionObjectQueryParams CollisionObjectParams; //不是很懂
	FCollisionQueryParams CollisionParams; //跟碰撞相關的參數
	//CollisionParams.AddIgnoredActor(Ignore); //忽略發出射線的Actor

	DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 3.0f); //發出射線

	if (GetWorld()->LineTraceSingleByObjectType(Hit, Start, End, CollisionObjectParams, CollisionParams)) {
		return Hit.GetActor();
	}
	else {
		return nullptr;
	}

}


