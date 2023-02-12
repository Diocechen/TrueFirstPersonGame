#pragma once

#include "Struct.generated.h"

USTRUCT(BlueprintType)
struct FItemData { //讓每個物品具備基礎資訊
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite);
	TSubclassOf<class APickup_Item> ItemClass;
	//讓藍圖內可以選擇 APickup_item 類的選項

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite);
	UTexture2D* ItemImage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite);
	float ItemCost;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite);
	bool IsEquipable;

	UPROPERTY(BlueprintReadWrite);
	int32 ItemAmount = 1; //當有新物品加入時，從一開始算
};