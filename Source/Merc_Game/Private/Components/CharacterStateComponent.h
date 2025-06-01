// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterStateComponent.generated.h"

UENUM(BlueprintType)
enum class ECharacterStates : uint8
{
	None, //UMETA(DisplayName = "Option 1"),
	Aim, //UMETA(DisplayName = "Option 2"),
	Sprint, //UMETA(DisplayName = "Option 3"),
	Death, //UMETA(DisplayName = "Option 4"),
	Disabled, //UMETA(DisplayName = "Option 5"),
	Parry,
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UCharacterStateComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCharacterStateComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, Category = State, meta = (AllowPrivateAccess = "true"))
	ECharacterStates CurrentState;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "State")
	void SetState(ECharacterStates NewState);

	UFUNCTION(BlueprintPure, Category = "State")
	ECharacterStates GetState() const;
	bool IsCurrentStateEqualToAny(const TArray<ECharacterStates>& StatesToCheck);

	void ResetState();
		
};
