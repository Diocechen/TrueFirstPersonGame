// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickup_Item.h"

#include "TrueFirstPersonCharacter.h"

#include "Components/StaticMeshComponent.h"
// Sets default values
APickup_Item::APickup_Item()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;

	ItemData.ItemClass = StaticClass(); //返回一個代表這個 class 的對象
	ItemData.IsEquipable = false;

	//bIsEquipable = false;

}

// Called when the game starts or when spawned
void APickup_Item::BeginPlay()
{
	Super::BeginPlay();
}

void APickup_Item::Interact(ATrueFirstPersonCharacter* character)
{
	if (character) {
		character->AddItemToArrayAndInventory(ItemData);
		Destroy();
	}
}

void APickup_Item::UsedBy(ATrueFirstPersonCharacter* character)
{
	UE_LOG(LogTemp, Warning, TEXT("USING ITEM %s"), *GetName());
}




