// 2023, indiependent.studio. All rights reserved.


#include "HUD/HealthBarComponent.h"

#include "Components/ProgressBar.h"
#include "HUD/HealthBar.h"

void UHealthBarComponent::SetHealthPercent(float Percent)
{
	if (HealthBarWidget == nullptr)
	{
		HealthBarWidget = Cast<UHealthBar>(GetUserWidgetObject());
	}
	else if (HealthBarWidget->HealthBar)
	{
		/*
		 * HealthBarWidget is a UHealthBar which has a pointer to HealthBar which is a ProgressBar.
		 * Pasting the declaration from HealthBar.h for reference, but check it didn't change before copying.
		 ** UPROPERTY(meta=(BindWidget))
		 ** TObjectPtr<UProgressBar> HealthBar;
		 */
		HealthBarWidget->HealthBar->SetPercent(Percent);
	}
}
