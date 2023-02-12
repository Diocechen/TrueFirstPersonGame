// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

//#include "TrueFirstPersonCharacter.h"
#include "MyInterface.generated.h"

class ATrueFirstPersonCharacter;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMyInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SMALLOPENWORLD_API IMyInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void Interact(ATrueFirstPersonCharacter* character) = 0; //C++ 內的純虛擬函式語法，就是 interface

	virtual void UsedBy(ATrueFirstPersonCharacter* character) {}
};
