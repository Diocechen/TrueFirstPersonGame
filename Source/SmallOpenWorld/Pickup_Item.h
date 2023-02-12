// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyInterface.h"
#include "Struct.h"
#include "Pickup_Item.generated.h"

UCLASS()
class SMALLOPENWORLD_API APickup_Item : public AActor, public IMyInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickup_Item();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UStaticMeshComponent* MeshComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Data);
	FItemData ItemData;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Data);
	//bool bIsEquipable;

public:

	virtual void Interact(ATrueFirstPersonCharacter* character);

	virtual void UsedBy(ATrueFirstPersonCharacter* character);

	bool IsEquipable() const { return ItemData.IsEquipable; }
};
