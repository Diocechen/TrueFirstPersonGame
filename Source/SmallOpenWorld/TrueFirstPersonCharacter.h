// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Components/SphereComponent.h"
#include "Pickup_Item/Pickup_Item_Default_Weapon.h"
#include "Struct.h"
#include "TrueFirstPersonCharacter.generated.h"


class APickup_Item; //使用這種方法引入 class 使用 include 會有問題
//class APickup_Item_Default_Weapon;

UCLASS()
class SMALLOPENWORLD_API ATrueFirstPersonCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FirstPersonCamera;

public:
	// Sets default values for this character's properties
	ATrueFirstPersonCharacter();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input)
	float TurnRateGamepad;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = State)
	float PlayerHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PlayerMovement)
	float SprintingSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = State)
	float Stamina;

	FTimerHandle StaminaHandle; //處理耐力值恢復
	FTimerHandle SprintHandle; //處理耐力值消耗

	bool IsSprinting;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State)
	bool IsPunching;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = State)
	bool IsEquipSomethingInHand;

	//class ALineTrace* LineTraceComp;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	//class UAnimMontage* NormalAttackMontage;

	//class UAnimInstance* AnimIns;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	UFUNCTION(BlueprintImplementableEvent) //在藍圖創建一個event似乎不用定義在 .cpp 檔裡面
	void AddItemToInventoryWidget(FItemData ItemData);

	UFUNCTION(BlueprintImplementableEvent) 
	void UpdateInventoryWidget(const TArray<FItemData>& NewInventoryArray, int RemoveThisFromGrid); //更新顯示的介面
	//const 待補充
	//記得是傳地址

	//UFUNCTION(BlueprintImplementableEvent)
	//void DetachWeaponInHand();

	UFUNCTION(BlueprintCallable) //在藍圖被呼叫
	void UseItem(TSubclassOf<APickup_Item> ItemSubclass, int IndexOfThisItem);
	//TSubclassOf 可以不用建構類別就能使用類別

	UPROPERTY(BlueprintReadWrite)
	TArray<FItemData> InventoryArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USphereComponent* FistCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	APickup_Item_Default_Weapon* currentWeapon;

	void MoveForward(float Value);

	void MoveRight(float Value);

	void TurnRight(float Rate);

	void LookUp(float Rate);

	void SprintStart();

	void SprintStop();

	void InteractPressed();

	void NormalAttack();

public:
	void DamageCharacter(float DamageAmount);

	void HealCharacter(float HealAmount);

	void RegenerateStamina();

	void HandleStamina();
	void LowerStamina(float Value);

	void AttempToJump();

	void AddItemToArrayAndInventory(FItemData ItemData);
};
