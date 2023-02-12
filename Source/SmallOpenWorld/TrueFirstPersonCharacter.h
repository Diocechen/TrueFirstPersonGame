// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Components/SphereComponent.h"
#include "Pickup_Item/Pickup_Item_Default_Weapon.h"
#include "Struct.h"
#include "TrueFirstPersonCharacter.generated.h"


class APickup_Item; //�ϥγo�ؤ�k�ޤJ class �ϥ� include �|�����D
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

	FTimerHandle StaminaHandle; //�B�z�@�O�ȫ�_
	FTimerHandle SprintHandle; //�B�z�@�O�Ȯ���

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

	UFUNCTION(BlueprintImplementableEvent) //�b�ŹϳЫؤ@��event���G���Ωw�q�b .cpp �ɸ̭�
	void AddItemToInventoryWidget(FItemData ItemData);

	UFUNCTION(BlueprintImplementableEvent) 
	void UpdateInventoryWidget(const TArray<FItemData>& NewInventoryArray, int RemoveThisFromGrid); //��s��ܪ�����
	//const �ݸɥR
	//�O�o�O�Ǧa�}

	//UFUNCTION(BlueprintImplementableEvent)
	//void DetachWeaponInHand();

	UFUNCTION(BlueprintCallable) //�b�ŹϳQ�I�s
	void UseItem(TSubclassOf<APickup_Item> ItemSubclass, int IndexOfThisItem);
	//TSubclassOf �i�H���Ϋغc���O�N��ϥ����O

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
