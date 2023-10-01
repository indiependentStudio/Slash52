// 2023, indiependent.studio. All rights reserved.


#include "Items/Treasure.h"

#include "Characters/SlashCharacter.h"
#include "Kismet/GameplayStatics.h"

void ATreasure::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ASlashCharacter* SlashCharacter = Cast<ASlashCharacter>(OtherActor))
	{
		if (PickupSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, PickupSound, GetActorLocation());
		}

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(1, 10.f, FColor::Orange,
				FString::Printf(TEXT("Gold: %i"), Gold));
		}
		
		Destroy();
	}
}
