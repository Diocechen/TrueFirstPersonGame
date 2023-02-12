// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickup_Item_Default_Weapon.h"
#include "SmallOpenWorld/TrueFirstPersonCharacter.h"

APickup_Item_Default_Weapon::APickup_Item_Default_Weapon() 
{
	ItemData.IsEquipable = true;
}

void APickup_Item_Default_Weapon::AttachWeapon(ATrueFirstPersonCharacter* character)
{
	MeshComp->AttachToComponent(character->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("RightHandSocket"));
}

void APickup_Item_Default_Weapon::DetachWeapon()
{
	MeshComp->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
	Destroy();
}

