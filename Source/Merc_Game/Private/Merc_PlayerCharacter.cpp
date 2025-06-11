// Fill out your copyright notice in the Description page of Project Settings.


#include "Merc_PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "InputActionValue.h"
#include "Actors/Merc_Gun.h"
#include "Components/CharacterStateComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "UI/Merc_PlayerHUDWidget.h"



// Sets default values
AMerc_PlayerCharacter::AMerc_PlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
	CharacterStateComp = CreateDefaultSubobject<UCharacterStateComponent>(TEXT("Character State Component"));

}

// Called when the game starts or when spawned
void AMerc_PlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	InitWeapons();

	CurrentHealth = MaxHealth;

	DefaultCameraArmLength = CameraBoom->TargetArmLength; // The camera follows at this distance behind the character	
	DefaultCameraFOV = FollowCamera->FieldOfView;
	DefaultWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;

	// In BeginPlay
	if (PlayerHUDClass)
	{
		PlayerHUD = CreateWidget<UMerc_PlayerHUDWidget>(GetWorld(), PlayerHUDClass);
		if (PlayerHUD)
		{
			PlayerHUD->AddToViewport();
			PlayerHUD->UpdateAmmo(Gun->GetCurrentAmmo(), Gun->GetMaxAmmo());
			PlayerHUD->SetWeaponIcon(Gun->GetGunIcon());
			//PlayerHUD->UpdateHealth(CurrentHealth, MaxHealth);
			//PlayerHUD->UpdateGrenades(CurrentGrenades); // optional
		}
	}

}

// Called every frame
void AMerc_PlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMerc_PlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {

		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMerc_PlayerCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMerc_PlayerCharacter::Look);

		// Shoot
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Started, this, &AMerc_PlayerCharacter::Shoot);
		// Shoot
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Completed, this, &AMerc_PlayerCharacter::StopShooting);

		// Aim
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Started, this, &AMerc_PlayerCharacter::AimStart);
		// Aim
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Completed, this, &AMerc_PlayerCharacter::AimEnd);

		// Reload
		EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Triggered, this, &AMerc_PlayerCharacter::Reload);

		// Sprint
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &AMerc_PlayerCharacter::SprintStart);
		// Sprint
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AMerc_PlayerCharacter::SprintEnd);

		// Scroll Up
		EnhancedInputComponent->BindAction(ScrollUpAction, ETriggerEvent::Started, this, &AMerc_PlayerCharacter::ScrollUpWeapon);
		// Scroll Down
		EnhancedInputComponent->BindAction(ScrollDownAction, ETriggerEvent::Started, this, &AMerc_PlayerCharacter::ScrollDownWeapon);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AMerc_PlayerCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AMerc_PlayerCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

float AMerc_PlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	//DamageToApply = FMath::Min(CurrentHealth, DamageToApply);
	//CurrentHealth -= DamageToApply;
	//UE_LOG(LogTemp, Warning, TEXT("Current Health: '%f'"), CurrentHealth);

	//if (IsDead())
	//{
	//	ASimpleShooterGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ASimpleShooterGameModeBase>();
	//	if (GameMode != nullptr)
	//	{
	//		GameMode->PawnKilled(this);
	//	}
	//	DetachFromControllerPendingDestroy(); // ***** Maybe Change these things later. Not Sure, maybe.
	//	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//}

	return DamageToApply;

}

void AMerc_PlayerCharacter::Shoot()
{
	Gun->StartFiring();
}

void AMerc_PlayerCharacter::StopShooting()
{
	if(Gun->GetFireMode() == EFireMode::FullAuto)
		Gun->StopFiring();
}

void AMerc_PlayerCharacter::AimStart()
{
	UE_LOG(LogTemp, Log, TEXT("Aiming Started!!!!"));

	CameraBoom->TargetArmLength = AimingCameraArmLength;
	FollowCamera->FieldOfView = AimingCameraFOV;

	CharacterStateComp->SetState(ECharacterStates::Aim);
	SprintEnd();
}

void AMerc_PlayerCharacter::AimEnd()
{
	UE_LOG(LogTemp, Warning, TEXT("Aiming Stopped!!!!"));

	CameraBoom->TargetArmLength = DefaultCameraArmLength;
	FollowCamera->FieldOfView = DefaultCameraFOV;
	CharacterStateComp->SetState(ECharacterStates::None);
}

void AMerc_PlayerCharacter::Reload()
{
	Gun->StartReload();
}

void AMerc_PlayerCharacter::SprintStart()
{
	if (CharacterStateComp->GetState() == ECharacterStates::Aim)
		return;

	UE_LOG(LogTemp, Log, TEXT("Sprinting Started!!!!"));
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	// Disable controller yaw rotation
	bUseControllerRotationYaw = false;

	// Enable movement-based rotation
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void AMerc_PlayerCharacter::SprintEnd()
{
	UE_LOG(LogTemp, Warning, TEXT("Sprinting Stopped!!!!"));
	GetCharacterMovement()->MaxWalkSpeed = DefaultWalkSpeed;
	// Re-enable controller rotation yaw
	bUseControllerRotationYaw = true;

	// Prevent movement component from rotating the character
	GetCharacterMovement()->bOrientRotationToMovement = false;

}

void AMerc_PlayerCharacter::ScrollUpWeapon()
{
	int32 NewIndex = (CurrentWeaponIndex + 1) % Weapons.Num();
	UE_LOG(LogTemp, Log, TEXT("ScrollUpWeapon: Attempting to switch to Index %d"), NewIndex);
	SwitchWeapon(NewIndex);
}

void AMerc_PlayerCharacter::ScrollDownWeapon()
{
	int32 NewIndex = (CurrentWeaponIndex - 1 + Weapons.Num()) % Weapons.Num();
	UE_LOG(LogTemp, Log, TEXT("ScrollDownWeapon: Attempting to switch to Index %d"), NewIndex);
	SwitchWeapon(NewIndex);
}

void AMerc_PlayerCharacter::InitWeapons()
{
	for (TSubclassOf<AMerc_Gun> WeaponClass : WeaponClasses)
	{
		if (WeaponClass)
		{
			AMerc_Gun* NewGun = GetWorld()->SpawnActor<AMerc_Gun>(WeaponClass);
			NewGun->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("WeaponSocket"));
			NewGun->SetOwner(this);
			NewGun->SetActorHiddenInGame(true); // Hide all but the current one
			Weapons.Add(NewGun);
		}
	}

	// Show only first weapon
	if (Weapons.IsValidIndex(0))
	{
		Weapons[0]->SetActorHiddenInGame(false);
		CurrentWeaponIndex = 0;
		Gun = Weapons[0]; // Set the active Gun reference here

		if(Gun)
			Gun->OnAmmoChanged.AddDynamic(this, &AMerc_PlayerCharacter::OnAmmoChanged);
	}
}

void AMerc_PlayerCharacter::SwitchWeapon(int32 NewIndex)
{
	if (NewIndex == CurrentWeaponIndex || !Weapons.IsValidIndex(NewIndex))
	{
		UE_LOG(LogTemp, Warning, TEXT("SwitchWeapon: Ignored - Invalid index or same weapon. NewIndex: %d, Current: %d"), NewIndex, CurrentWeaponIndex);
		return;
	}

	if (Gun)
	{
		Gun->OnAmmoChanged.RemoveDynamic(this, &AMerc_PlayerCharacter::OnAmmoChanged);
		Gun->CancelReload();
	}

	UE_LOG(LogTemp, Log, TEXT("Switching Weapon: From Index %d to Index %d"), CurrentWeaponIndex, NewIndex);

	Weapons[CurrentWeaponIndex]->SetActorHiddenInGame(true);
	CurrentWeaponIndex = NewIndex;
	Weapons[CurrentWeaponIndex]->SetActorHiddenInGame(false);
	Gun = Weapons[CurrentWeaponIndex];

	if (Gun)
	{
		UE_LOG(LogTemp, Log, TEXT("Current Weapon: %s"), *Gun->GetName());
		Gun->OnAmmoChanged.AddDynamic(this, &AMerc_PlayerCharacter::OnAmmoChanged);

		// Update HUD immediately on switch
		OnAmmoChanged(Gun->GetCurrentAmmo(), Gun->GetMaxAmmo());

		if (PlayerHUD)
			PlayerHUD->SetWeaponIcon(Gun->GetGunIcon());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("SwitchWeapon: Gun pointer is NULL after switching!"));
	}
}


bool AMerc_PlayerCharacter::IsDead() const
{
	return CurrentHealth <= 0;
}

void AMerc_PlayerCharacter::AI_Shoot()
{
	Gun->StartFiring();

}

float AMerc_PlayerCharacter::GetHealthPercent() const
{
	return CurrentHealth / MaxHealth;
}

void AMerc_PlayerCharacter::OnAmmoChanged(int32 CurrentAmmo, int32 MaxAmmo)
{
	if (PlayerHUD)
		PlayerHUD->UpdateAmmo(CurrentAmmo, MaxAmmo);
}
