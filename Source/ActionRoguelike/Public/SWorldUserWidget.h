// 2023 Jorge Sánchez, All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SWorldUserWidget.generated.h"

class USizeBox;

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API USWorldUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = "UI")
	FVector WorldOffset;
	
	UPROPERTY(BlueprintReadWrite, Category = "UI",  meta = (ExposeOnSpawn = true))
	AActor *AttachedActor;

protected:

	UPROPERTY(meta = (BindWidget))
	USizeBox *ParentSizeBox; 
	
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};