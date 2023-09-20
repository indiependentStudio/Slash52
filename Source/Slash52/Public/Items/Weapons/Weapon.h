// 2023, indiependent.studio. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Weapon.generated.h"

/**
 * 
 */
UCLASS()
class SLASH52_API AWeapon : public AItem
{
	GENERATED_BODY()

public:
	void AttachMeshToSocket(USceneComponent* InParent, FName InSocketName);
	void Equip(USceneComponent* InParent, FName InSocketName);
	
protected:
	// No UFUNCTION when overriding
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent,AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void OnSphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

private:
	UPROPERTY(EditAnywhere, Category="Weapon Properties")
	TObjectPtr<USoundBase> EquipSound;
};
