#pragma once

#include "Struct.generated.h"

USTRUCT(BlueprintType)
struct FItemData { //���C�Ӫ��~��ư�¦��T
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite);
	TSubclassOf<class APickup_Item> ItemClass;
	//���ŹϤ��i�H��� APickup_item �����ﶵ

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite);
	UTexture2D* ItemImage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite);
	float ItemCost;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite);
	bool IsEquipable;

	UPROPERTY(BlueprintReadWrite);
	int32 ItemAmount = 1; //���s���~�[�J�ɡA�q�@�}�l��
};