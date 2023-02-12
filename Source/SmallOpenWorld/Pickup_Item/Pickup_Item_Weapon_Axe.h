// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup_Item_Default_Weapon.h"
#include "Pickup_Item_Weapon_Axe.generated.h"

/**
 * 
 */
UCLASS()
class SMALLOPENWORLD_API APickup_Item_Weapon_Axe : public APickup_Item_Default_Weapon
{
	GENERATED_BODY()
public:
	virtual void AttachWeapon(ATrueFirstPersonCharacter* character) override;
};
