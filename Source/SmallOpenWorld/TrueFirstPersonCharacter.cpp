// Fill out your copyright notice in the Description page of Project Settings.


#include "TrueFirstPersonCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "DrawDebugHelpers.h" //ø�s debugline 

//#include "LineTrace.h"
#include "Pickup_Item.h"
ATrueFirstPersonCharacter::ATrueFirstPersonCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	TurnRateGamepad = 50.f; //����v

	bUseControllerRotationPitch = false; //�T�w�� Character ���M�|�ܩ_��
	bUseControllerRotationYaw = true; //���O����w Z �b (YAW) ����A���M�ʧ@�|�ܩ_��
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true; //�������ۤ�V����
	//GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); //������H�o�ӳt�v����

	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCamera->SetupAttachment(GetMesh(),"head");
	//�N�۾����W Mesh �̪� head ����
	//�� Meah �� CHaracter ���w�]��
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
	//�o�� timer �|�b�C�@��I�s RegenerateStamina() �禡
	GetWorld()->GetTimerManager().SetTimer(SprintHandle, this, &ATrueFirstPersonCharacter::HandleStamina, 1.0f, true);
	//�|�ϥ� HandleStamina

	//AnimIns = GetMesh()->GetAnimInstance(); //��o�w�] Mesh �W�����ʵe���
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
	//�e�ᥪ�k
	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &ATrueFirstPersonCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &ATrueFirstPersonCharacter::MoveRight);
	//�ƹ����Y
	PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &ATrueFirstPersonCharacter::TurnRight);
	PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &ATrueFirstPersonCharacter::LookUp);
	
	//�� �ϥΤ��ت�jump�禡
	//action �b�I���ɩI�s�@��
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ATrueFirstPersonCharacter::AttempToJump); //���F�����D�ɴ�֭@�O��
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	//�p�]�Ĩ�
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
		//�q rotatot ��o vector
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
			GetWorld()->GetTimerManager().PauseTimer(StaminaHandle); //�b�]�ɰ���^�_��O
		}
		else if (Stamina <= 0.0f) {
			GetWorld()->GetTimerManager().UnPauseTimer(StaminaHandle); 
			//�b�@�O�ȵ���0.0�ɱj���b�]�A�ö}�l��_��O
			//�j���b HandleStamina() �B�z
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

void ATrueFirstPersonCharacter::InteractPressed() //�򪫫~�ΤH������
{
	FVector Loc;
	FRotator Rot;
	GetController()->GetPlayerViewPoint(Loc, Rot);

	FVector Start = Loc + Rot.Vector() * 40.0f;
	FVector End = Start + Rot.Vector() * 170.0f;

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	if (GetWorld()->LineTraceSingleByChannel(HitResult,Start,End,ECC_Visibility,Params)) { //�b�o�ӥ@�ɳгy�X�g�u
		if (IMyInterface* Interface = Cast<IMyInterface>(HitResult.GetActor())) {
			Interface->Interact(this);
		}
	}
}

void ATrueFirstPersonCharacter::AddItemToArrayAndInventory(FItemData ItemData)
{
	bool IsNewItem = true;
	for (FItemData& oldItem : InventoryArray) { //�M���x�s���~��TArray
		if (oldItem.ItemClass == ItemData.ItemClass && oldItem.ItemAmount < 4 && !ItemData.IsEquipable) { //�p�G�s�W�����~�b InventoryArray �̭�������  
			oldItem.ItemAmount++; //�W�[���Ӫ��~���ƶq
			IsNewItem = false;
			UE_LOG(LogTemp, Warning, TEXT("ITEM AMOUNT %d"), oldItem.ItemAmount);
			break;
		}
	}
	if (IsNewItem) { //�p�G�O�s�����~
		InventoryArray.Add(ItemData);
		UE_LOG(LogTemp, Warning, TEXT("%d"), InventoryArray.Num());
		if (InventoryArray.Num()) {
			AddItemToInventoryWidget(InventoryArray[InventoryArray.Num()-1]);
		}
	}else { //�p�G���O�s���~�N��s inventory_UI ���s���ƾڧe�{�X��
		UpdateInventoryWidget(InventoryArray, 0); //�ǤJ�w�g��s��TArray
	}
}

void ATrueFirstPersonCharacter::UseItem(TSubclassOf<APickup_Item> ItemSubclass, int IndexOfThisItem) //�b W_Inventory_Item �Q�I�s
{
	if (ItemSubclass) {
		if (APickup_Item* item = ItemSubclass.GetDefaultObject()) { //��^�w�]�n����H
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
				UpdateInventoryWidget(InventoryArray, IndexOfThisItem); //���O�o��s����
			}
		}
	}
}

void ATrueFirstPersonCharacter::RegenerateStamina() //�H�ۮɶ���_��O��
{
	if (Stamina >= 100.0f) {
		Stamina = 100.0f;
	}
	else {
		Stamina += 3;
	}
}
void ATrueFirstPersonCharacter::HandleStamina() //�ϥ� SprintHandle �C1.0��|����
{
	if (IsSprinting) {
		LowerStamina(2.0f);
		if (Stamina <= 0.0f) {
			SprintStop(); //��O�ȵ���s�ɱj���
		}
	}
}

void ATrueFirstPersonCharacter::AttempToJump()
{
	if (Stamina > 10.0f && !GetCharacterMovement()->IsFalling()) { //�קK�s���I���ɦ�����O��
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

void ATrueFirstPersonCharacter::LowerStamina(float Value) //�i��Y�Ӱʧ@�Ӯ�����O
{
	Stamina -= Value;
	if (Stamina <= 0.0f) {
		Stamina = 0;
	}
}





