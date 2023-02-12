// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickup_Item_Weapon_Axe.h"

void APickup_Item_Weapon_Axe::AttachWeapon(ATrueFirstPersonCharacter* character)
{
	Super::AttachWeapon(character);
	MeshComp->AddRelativeRotation(FRotator(0.0f, 0.0f, 180.0f));
	MeshComp->AddRelativeLocation(FVector(0.0f, 0.0f, 25.0f));
}
