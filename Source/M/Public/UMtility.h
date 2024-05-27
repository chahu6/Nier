// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UMtility.generated.h"

/**
 * 
 */
USTRUCT()	
struct FMAnimStartEndFrame : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnyWhere)
	int StartFrame;
	
	UPROPERTY(EditAnyWhere)
	int EndFrame;

	UPROPERTY(EditAnyWhere)
	FName AnimName;
};

UCLASS()
class M_API UMUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	UFUNCTION(BlueprintCallable,Category = "M|Utility|Anim")
	static void RecordAnimByDataTable(const UAnimSequence* InSequence, UDataTable* DataTable);

private:
	static  void RecordAnim_Internal(UAnimSequence* TargetAnim,TMap<FName,TArray<FTransform>>& SourceBonesTransforms,int StartFrame, int EndFrame,int FrameSize);
};
