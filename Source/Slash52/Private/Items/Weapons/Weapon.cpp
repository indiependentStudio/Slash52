// 2023, indiependent.studio. All rights reserved.


#include "Items/Weapons/Weapon.h"

#include "Characters/SlashCharacter.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

void AWeapon::AttachMeshToSocket(USceneComponent* InParent, FName InSocketName)
{
	ItemMesh->AttachToComponent(InParent, FAttachmentTransformRules::SnapToTargetIncludingScale, InSocketName);
}

void AWeapon::Equip(USceneComponent* InParent, FName InSocketName)
{
	AttachMeshToSocket(InParent, InSocketName);
	ItemState = EItemState::EIS_Equipped;

	if (EquipSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, EquipSound, GetActorLocation()); 
	}

	if (Sphere)
	{
		Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AWeapon::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                              UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void AWeapon::OnSphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnSphereOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}
