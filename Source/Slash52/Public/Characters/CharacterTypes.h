#pragma once

UENUM(BlueprintType)
// Enumeration for Weapon equip states
enum class ECharacterState : uint8
{
	ECS_Unequipped UMETA(DisplayName="Weapon not equipped"),
	ECS_EquippedOneHandedWeapon UMETA(DisplayName="Equipped 1-handed weapon"),
	ECS_EquippedTwoHandedWeapon UMETA(DisplayName="Equipped 2-handed weapon")
};

