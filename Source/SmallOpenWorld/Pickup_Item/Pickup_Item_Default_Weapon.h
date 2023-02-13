// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SmallOpenWorld\Pickup_Item.h"
#include "Pickup_Item_Default_Weapon.generated.h"

/**
 * 
 */
UCLASS()
class SMALLOPENWORLD_API APickup_Item_Default_Weapon : public APickup_Item
{
	GENERATED_BODY()
public:
	APickup_Item_Default_Weapon();

	virtual void AttachWeapon(ATrueFirstPersonCharacter* character);

	void DetachWeapon();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	float BaseDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	float BaseSpeed;
};
