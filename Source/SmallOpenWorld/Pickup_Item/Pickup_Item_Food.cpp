// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickup_Item_Food.h"
#include "SmallOpenWorld/TrueFirstPersonCharacter.h"

APickup_Item_Food::APickup_Item_Food()
{
	HealthCureAmount = 20.0f;

	ItemData.ItemClass = StaticClass();
}

void APickup_Item_Food::UsedBy(ATrueFirstPersonCharacter* character)
{
	if (character) {
		character->HealCharacter(HealthCureAmount);
	}
}
