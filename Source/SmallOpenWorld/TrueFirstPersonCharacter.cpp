// Fill out your copyright notice in the Description page of Project Settings.


#include "TrueFirstPersonCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "DrawDebugHelpers.h" //繪製 debugline 

//#include "LineTrace.h"
#include "Pickup_Item.h"
ATrueFirstPersonCharacter::ATrueFirstPersonCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	TurnRateGamepad = 50.f; //旋轉率

	bUseControllerRotationPitch = false; //固定住 Character 不然會很奇怪
	bUseControllerRotationYaw = true; //但是不鎖定 Z 軸 (YAW) 旋轉，不然動作會很奇怪
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true; //讓角色跟著方向移動
	//GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); //讓角色以這個速率旋轉

	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCamera->SetupAttachment(GetMesh(),"head");
	//將相機接上 Mesh 裡的 head 物件
	//此 Meah 為 CHaracter 內預設的
	FirstPersonCamera->bUsePawnControlRotation = true;

	//Fist collision
	FistCollision = CreateDefaultSubobject<USphereComponent>(TEXT("FistCollision"));
	FistCollision->SetupAttachment(GetMesh(), FName("RightHand"));

	SprintingSpeed = 2.0f;

	PlayerHealth = 50.0f;
	Stamina = 100.0f;

	IsSprinting = false;

	IsPunching = false;

	IsEquipSomethingInHand = false;

	//LineTraceComp = CreateDefaultSubobject<ALineTrace>(TEXT("LineTraceComp"));

}

// Called when the game starts or when spawned
void ATrueFirstPersonCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(StaminaHandle, this, &ATrueFirstPersonCharacter::RegenerateStamina, 3.0f, true);
	//這個 timer 會在每一秒呼叫 RegenerateStamina() 函式
	GetWorld()->GetTimerManager().SetTimer(SprintHandle, this, &ATrueFirstPersonCharacter::HandleStamina, 1.0f, true);
	//會使用 HandleStamina

	//AnimIns = GetMesh()->GetAnimInstance(); //獲得預設 Mesh 上面的動畫實例
}

// Called every frame
void ATrueFirstPersonCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATrueFirstPersonCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	//前後左右
	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &ATrueFirstPersonCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &ATrueFirstPersonCharacter::MoveRight);
	//滑鼠鏡頭
	PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &ATrueFirstPersonCharacter::TurnRight);
	PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &ATrueFirstPersonCharacter::LookUp);
	
	//跳 使用內建的jump函式
	//action 在點擊時呼叫一次
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ATrueFirstPersonCharacter::AttempToJump); //為了讓跳躍時減少耐力值
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	//小跑衝刺
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ATrueFirstPersonCharacter::SprintStart);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ATrueFirstPersonCharacter::SprintStop);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ATrueFirstPersonCharacter::InteractPressed);

	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &ATrueFirstPersonCharacter::NormalAttack);
}

void ATrueFirstPersonCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		Value *= 0.5f;
		//從 rotatot 獲得 vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ATrueFirstPersonCharacter::MoveRight(float Value)
{
	if (Value != 0.0f) {
		Value *= 0.5f;
		AddMovementInput(GetActorRightVector(), Value);
	}
}


void ATrueFirstPersonCharacter::TurnRight(float Rate)
{
	AddControllerYawInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void ATrueFirstPersonCharacter::LookUp(float Rate)
{
	AddControllerPitchInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void ATrueFirstPersonCharacter::SprintStart()
{
	if (GetVelocity() != FVector::ZeroVector) {
		if (Stamina > 10) {
			IsSprinting = true;
			GetCharacterMovement()->MaxWalkSpeed *= SprintingSpeed;
			GetWorld()->GetTimerManager().PauseTimer(StaminaHandle); //奔跑時停止回復體力
		}
		else if (Stamina <= 0.0f) {
			GetWorld()->GetTimerManager().UnPauseTimer(StaminaHandle); 
			//在耐力值等於0.0時強制停止奔跑，並開始恢復體力
			//強制停止在 HandleStamina() 處理
		}
	}
}

void ATrueFirstPersonCharacter::SprintStop()
{
	if (IsSprinting) {
		IsSprinting = false;
		GetCharacterMovement()->MaxWalkSpeed /= SprintingSpeed;
		GetWorld()->GetTimerManager().UnPauseTimer(StaminaHandle);
	}
}

void ATrueFirstPersonCharacter::DamageCharacter(float DamageAmount)
{
	PlayerHealth -= DamageAmount;
	if (PlayerHealth <= 0) {
		Destroy();
	}
}

void ATrueFirstPersonCharacter::HealCharacter(float HealAmount)
{
	PlayerHealth += HealAmount;
	if (PlayerHealth >= 100) {
		PlayerHealth = 100;
	}
}

void ATrueFirstPersonCharacter::InteractPressed() //跟物品或人物互動
{
	FVector Loc;
	FRotator Rot;
	GetController()->GetPlayerViewPoint(Loc, Rot);

	FVector Start = Loc + Rot.Vector() * 40.0f;
	FVector End = Start + Rot.Vector() * 170.0f;

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	if (GetWorld()->LineTraceSingleByChannel(HitResult,Start,End,ECC_Visibility,Params)) { //在這個世界創造出射線
		if (IMyInterface* Interface = Cast<IMyInterface>(HitResult.GetActor())) {
			Interface->Interact(this);
		}
	}
}

void ATrueFirstPersonCharacter::AddItemToArrayAndInventory(FItemData ItemData)
{
	bool IsNewItem = true;
	for (FItemData& oldItem : InventoryArray) { //遍歷儲存物品的TArray
		if (oldItem.ItemClass == ItemData.ItemClass && oldItem.ItemAmount < 4 && !ItemData.IsEquipable) { //如果新增的物品在 InventoryArray 裡面有重複  
			oldItem.ItemAmount++; //增加那個物品的數量
			IsNewItem = false;
			UE_LOG(LogTemp, Warning, TEXT("ITEM AMOUNT %d"), oldItem.ItemAmount);
			break;
		}
	}
	if (IsNewItem) { //如果是新的物品
		InventoryArray.Add(ItemData);
		UE_LOG(LogTemp, Warning, TEXT("%d"), InventoryArray.Num());
		if (InventoryArray.Num()) {
			AddItemToInventoryWidget(InventoryArray[InventoryArray.Num()-1]);
		}
	}else { //如果不是新物品就刷新 inventory_UI 讓新的數據呈現出來
		UpdateInventoryWidget(InventoryArray, 0); //傳入已經更新的TArray
	}
}

void ATrueFirstPersonCharacter::UseItem(TSubclassOf<APickup_Item> ItemSubclass, int IndexOfThisItem) //在 W_Inventory_Item 被呼叫
{
	if (ItemSubclass) {
		if (APickup_Item* item = ItemSubclass.GetDefaultObject()) { //返回預設好的對象
			if (item->IsEquipable()) {
				if (IsEquipSomethingInHand) {
					currentWeapon->DetachWeapon();
					IsEquipSomethingInHand = false;
				}
				else {
					IsEquipSomethingInHand = true;

					FActorSpawnParameters params;
					params.Owner = this;

					currentWeapon = GetWorld()->SpawnActor<APickup_Item_Default_Weapon>(ItemSubclass, params);
					currentWeapon->AttachWeapon(this);

				}
			}
			else {
				item->UsedBy(this);
				InventoryArray[IndexOfThisItem].ItemAmount--;
				if (InventoryArray[IndexOfThisItem].ItemAmount <= 0) {
					InventoryArray.RemoveAt(IndexOfThisItem);
				}
				UE_LOG(LogTemp, Warning, TEXT("%d"), InventoryArray.Num());
				UpdateInventoryWidget(InventoryArray, IndexOfThisItem); //減完後記得更新頁面
			}
		}
	}
}

void ATrueFirstPersonCharacter::RegenerateStamina() //隨著時間恢復體力值
{
	if (Stamina >= 100.0f) {
		Stamina = 100.0f;
	}
	else {
		Stamina += 3;
	}
}
void ATrueFirstPersonCharacter::HandleStamina() //使用 SprintHandle 每1.0秒會執行
{
	if (IsSprinting) {
		LowerStamina(2.0f);
		if (Stamina <= 0.0f) {
			SprintStop(); //體力值等於零時強制停止
		}
	}
}

void ATrueFirstPersonCharacter::AttempToJump()
{
	if (Stamina > 10.0f && !GetCharacterMovement()->IsFalling()) { //避免連續點擊時扣除體力值
		Stamina -= 10.0f;
		Jump();
	}
}
void ATrueFirstPersonCharacter::NormalAttack()
{
	//PlayAnimMontage(NormalAttackMontage, 1.0f, FName("NormalAttack"));
	if (!IsPunching) {
		IsPunching = true;
	}
}

void ATrueFirstPersonCharacter::LowerStamina(float Value) //進行某個動作而消耗體力
{
	Stamina -= Value;
	if (Stamina <= 0.0f) {
		Stamina = 0;
	}
}





