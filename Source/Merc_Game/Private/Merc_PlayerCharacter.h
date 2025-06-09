// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Merc_PlayerCharacter.generated.h"

class AMerc_Gun;
class UCameraComponent;
class UCharacterStateComponent;
class UInputMappingContext;
class UInputAction;
class UMerc_PlayerHUDWidget;
class USpringArmComponent;
struct FInputActionValue;

UCLASS()
class AMerc_PlayerCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	/** Shoot Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ShootAction;

	/** Aim Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* AimAction;

	/** Reload Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ReloadAction;
	
	/** Sprint Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SprintAction;

	/** Scroll Up Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ScrollUpAction;

	/** Scroll Down Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ScrollDownAction;


public:
	// Sets default values for this character's properties
	AMerc_PlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);


public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }


protected:

	void Shoot();
	void StopShooting();
	void AimStart();
	void AimEnd();
	void Reload();
	void SprintStart();
	void SprintEnd();
	void ScrollUpWeapon();
	void ScrollDownWeapon();
	void InitWeapons();
	void SwitchWeapon(int32 NewIndex);

public:

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintPure)
	bool IsDead() const;

	void AI_Shoot();

	UFUNCTION(BlueprintPure)
	float GetHealthPercent() const;

	UFUNCTION()
	void OnAmmoChanged(int32 CurrentAmmo, int32 MaxAmmo);

private:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AMerc_Gun> GunClass;

	AMerc_Gun* Gun;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	float MaxHealth = 100.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	float CurrentHealth;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera Settings", meta = (AllowPrivateAccess = "true"))
	float DefaultCameraArmLength;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera Settings", meta = (AllowPrivateAccess = "true"))
	float AimingCameraArmLength;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera Settings", meta = (AllowPrivateAccess = "true"))
	float DefaultCameraFOV;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera Settings", meta = (AllowPrivateAccess = "true"))
	float AimingCameraFOV;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float DefaultWalkSpeed;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float SprintSpeed;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float WalkAimSpeed;

	UCharacterStateComponent* CharacterStateComp;

	UPROPERTY(EditDefaultsOnly, Category = "Weapons")
	TArray<TSubclassOf<AMerc_Gun>> WeaponClasses;
	UPROPERTY()
	TArray<AMerc_Gun*> Weapons;
	int32 CurrentWeaponIndex = 0;

	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<UMerc_PlayerHUDWidget> PlayerHUDClass;

	UMerc_PlayerHUDWidget* PlayerHUD;

};
