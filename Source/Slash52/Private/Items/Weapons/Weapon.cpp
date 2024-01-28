// 2023, indiependent.studio. All rights reserved.


#include "Items/Weapons/Weapon.h"

#include "NiagaraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Interfaces/HitInterface.h"
#include "Kismet/GameplayStatics.h"

AWeapon::AWeapon()
{
	WeaponBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Weapon Box"));
	WeaponBox->SetupAttachment(GetRootComponent());
	WeaponBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	WeaponBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	BoxTraceStart = CreateDefaultSubobject<USceneComponent>(TEXT("Start"));
	BoxTraceStart->SetupAttachment(GetRootComponent());

	BoxTraceEnd = CreateDefaultSubobject<USceneComponent>(TEXT("End"));
	BoxTraceEnd->SetupAttachment(GetRootComponent());
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	WeaponBox->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnBoxOverlap);
}

void AWeapon::AttachMeshToSocket(USceneComponent* InParent, FName InSocketName)
{
	ItemMesh->AttachToComponent(InParent, FAttachmentTransformRules::SnapToTargetIncludingScale, InSocketName);
}

void AWeapon::PlayEquipSound()
{
	if (EquipSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, EquipSound, GetActorLocation());
	}
}

void AWeapon::DestroySphereCollision()
{
	if (Sphere)
	{
		//Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Sphere->DestroyComponent();
	}
}

void AWeapon::DeactivateEmbers()
{
	if (EmbersEffect)
	{
		EmbersEffect->Deactivate();
	}
}

// called by SlashCharacter when picking up Weapon
void AWeapon::Equip(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator)
{
	ItemState = EItemState::EIS_Equipped;
	SetOwner(NewOwner);
	SetInstigator(NewInstigator); // used to specify who is doing damage with this weapon
	AttachMeshToSocket(InParent, InSocketName);
	DestroySphereCollision();
	PlayEquipSound();
	DeactivateEmbers();
}

void AWeapon::ExecuteGetHit(FHitResult BoxHit)
{
	if (Cast<IHitInterface>(BoxHit.GetActor()))
	{
		IHitInterface::Execute_GetHit(BoxHit.GetActor(), BoxHit.ImpactPoint, GetOwner());
	}
}

bool AWeapon::ActorIsSameType(AActor* OtherActor)
{
	return GetOwner()->ActorHasTag(FName("Enemy")) && OtherActor->ActorHasTag(FName("Enemy"));
}

void AWeapon::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                           int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Ignore hits on other enemies and self - when we overlap
	if (ActorIsSameType(OtherActor)) return;
		
	FHitResult BoxHit;
	BoxTrace(BoxHit);

	if (BoxHit.GetActor())
	{
		// Ignore hits on other enemies and self - when we hit
		if (ActorIsSameType(BoxHit.GetActor())) return;
		
		UGameplayStatics::ApplyDamage(
			BoxHit.GetActor(),
			Damage,
			GetInstigatorController(),
			this,
			UDamageType::StaticClass()
		);

		ExecuteGetHit(BoxHit);
		
		// BP function that drives our custom physics fields - used in course for the normal fields here too
		CreateFields(BoxHit.ImpactPoint);
	}
}

void AWeapon::BoxTrace(FHitResult& BoxHit)
{
	const FVector Start = BoxTraceStart->GetComponentLocation();
	const FVector End = BoxTraceEnd->GetComponentLocation();

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);

	for (AActor* Actor : IgnoreActors)
	{
		ActorsToIgnore.AddUnique(Actor);
	}

	UKismetSystemLibrary::BoxTraceSingle(
		this,
		Start,
		End,
		BoxTraceExtent,
		BoxTraceStart->GetComponentRotation(),
		TraceTypeQuery1,
		false,
		ActorsToIgnore,
		bShowBoxDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None,
		BoxHit,
		true
	);
	// Make sure we only register one hit on an enemy per swing
	IgnoreActors.AddUnique(BoxHit.GetActor());
}
