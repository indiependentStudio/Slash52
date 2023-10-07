// 2023, indiependent.studio. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "HealthBarComponent.generated.h"

class UHealthBar;
/**
 * 
 */
UCLASS()
class SLASH52_API UHealthBarComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	void SetHealthPercent(float Percent);

private:
	UPROPERTY()
	TObjectPtr<UHealthBar> HealthBarWidget;
};
