// 2023, indiependent.studio. All rights reserved.


#include "Components/AttributeComponent.h"

UAttributeComponent::UAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

float UAttributeComponent::GetHealthPercent()
{
	return Health / MaxHealth;
}

void UAttributeComponent::ReceiveDamage(float Damage)
{
	Health = FMath::Clamp(Health - Damage, 0, MaxHealth);
}

void UAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
