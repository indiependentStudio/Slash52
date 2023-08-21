// 2023, indiependent.studio. All rights reserved.


#include "Items/Item.h"

#include "Slash52/DebugMacros.h"

AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AItem::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	FVector StartLocation = GetActorLocation();
	if (World)
	{
		FVector ForwardVector = GetActorForwardVector();
		DRAW_SPHERE(StartLocation)
		DRAW_VECTOR(StartLocation, StartLocation + ForwardVector * 100)
	}
}

void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
