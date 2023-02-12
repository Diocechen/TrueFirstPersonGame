// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SmallOpenWorld\Pickup_Item.h"
#include "Pickup_Item_Food.generated.h"

class ATrueFirstPersonCharacter;
UCLASS()
class SMALLOPENWORLD_API APickup_Item_Food : public APickup_Item
{
	GENERATED_BODY()
public:
	APickup_Item_Food();

	virtual void UsedBy(ATrueFirstPersonCharacter* character) override;

protected:
	UPROPERTY(EditAnywhere)
	float HealthCureAmount;
};
