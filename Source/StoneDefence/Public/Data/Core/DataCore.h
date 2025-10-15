#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "DataCore.generated.h"



USTRUCT(BlueprintType)
struct FDataCore : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	FDataCore();

	virtual void Init();

	virtual bool IsValid() const;

	//名字
	UPROPERTY(EditDefaultsOnly, Category = "Attribute")
	FName Name;

	UPROPERTY(EditDefaultsOnly, Category = "Attribute")
	int32 ID;

	UPROPERTY(EditDefaultsOnly, Category = "Attribute")
	int32 Gold;

	//钻石 需要购买
	UPROPERTY(EditDefaultsOnly, Category = "Attribute")
	int32 Diamonds;

	//铜币 
	UPROPERTY(EditDefaultsOnly, Category = "Attribute")
	int32 Copper;
};