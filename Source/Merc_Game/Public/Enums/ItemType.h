#pragma once

#include "ItemType.generated.h"

// This is a shared enum for any buyable/interactable items in your game
UENUM(BlueprintType)
enum class EItemType : uint8
{
	Weapon,
	Ammo,
	Perk,
	Door,
	Upgrade
};