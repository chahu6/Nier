#include "UMtility.h"

#include "AnimationUtils.h"
#include "AssetToolsModule.h"
#include "GameplayTagsManager.h"
#include "ObjectTools.h"
#include "Animation/AnimSequenceHelpers.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Factories/AnimSequenceFactory.h"
#include "GameFramework/HUD.h"
#include "UObject/SavePackage.h"

#define LOCTEXT_NAMESPACE "Utility"

void UMUtility::RecordAnim_Internal(UAnimSequence* TargetAnim,
                                    TMap<FName, TArray<FTransform>>& SourceBonesTransforms, int StartFrame,
                                    int EndFrame, int FrameSize)
{
	if (SourceBonesTransforms.Num() <= 0)
	{
		return;
	}
	IAnimationDataController& DestController = TargetAnim->GetController();
	for (auto& item : SourceBonesTransforms)
	{
		TArray<FVector3d> PosKeys;
		PosKeys.SetNum(FrameSize);
		TArray<FQuat4d> RotKeys;
		RotKeys.SetNum(FrameSize);
		TArray<FVector3d> ScaleKeys;
		ScaleKeys.SetNum(FrameSize);

		for (int32 TransformIndex = StartFrame, i = 0; TransformIndex <= EndFrame; ++TransformIndex, ++i)
		{
			PosKeys[i] = item.Value[TransformIndex].GetLocation();
			RotKeys[i] = item.Value[TransformIndex].GetRotation();
			ScaleKeys[i] = item.Value[TransformIndex].GetScale3D();
		}
		DestController.AddBoneCurve(item.Key, false);
		DestController.SetBoneTrackKeys(item.Key, PosKeys, RotKeys, ScaleKeys, false);
	}
	DestController.SetNumberOfFrames(FrameSize - 1, false);
}

void UMUtility::RecordAnimByDataTable(const UAnimSequence* InSequence, UDataTable* DataTable)
{
	TArray<FName> RowNames = DataTable->GetRowNames();
	FString ContextString;
	
	const IAnimationDataModel* Model = InSequence->GetDataModel();
	int32 FrameNumber=Model->GetNumberOfFrames();

	//get track name
	TArray<FName> TrackNames;
	TrackNames.Reserve(Model->GetNumBoneTracks());
	Model->GetBoneTrackNames(TrackNames);

	//get all bone transform With Name;
	TMap<FName, TArray<FTransform>> SourceBonesTransforms;
	TArray<FTransform> BoneTransforms;
	BoneTransforms.Reserve(InSequence->GetNumberOfSampledKeys());
	for (const FName& TrackName : TrackNames)
	{
		BoneTransforms.Reset();
		Model->GetBoneTrackTransforms(TrackName, BoneTransforms);
		SourceBonesTransforms.Add(TrackName, BoneTransforms);
	}

	for (FName& Name : RowNames)
	{
		FMAnimStartEndFrame* ClipData = DataTable->FindRow<FMAnimStartEndFrame>(Name, ContextString);
		if(! ClipData)
		{
			UE_LOG(LogTemp,Error,TEXT("Provided incorrect type RecordAnimByDataTable , need struct of %s"),*DataTable->GetClass()->GetName())
			return;
		}
		
		int32 StartFrame = FMath::Clamp(ClipData->StartFrame, 0,FrameNumber );
		int32 EndFrame = FMath::Clamp(ClipData->EndFrame, 0, FrameNumber);

		//
		int32 FrameSize = EndFrame - StartFrame + 1;
		if (FrameSize <= 0)
		{
			UE_LOG(LogTemp,Error,TEXT("Can't Clip Anim With Zero Frame : %s"),*ClipData->AnimName.ToString())
			continue;
		}
		
		//crete Sequence;
		UAnimSequence* DestSeq = nullptr;
		FString SequenceName = ClipData->AnimName.ToString();
		
		SequenceName = ObjectTools::SanitizeObjectName(SequenceName);
		FString ParentPath = FString::Printf(
			TEXT("%s/%s"), *FPackageName::GetLongPackagePath(*InSequence->GetPackage()->GetName()), *SequenceName);

		UObject* ParentPackage = CreatePackage(*ParentPath);
		UObject* Object = LoadObject<UObject>(ParentPackage, *SequenceName, nullptr, (LOAD_Quiet | LOAD_NoWarn),
		                                      nullptr);
		DestSeq = Cast<UAnimSequence>(Object);
		// if object with same name exists, warn user
		if (Object && !DestSeq)
		{
			UE_LOG(LogTemp, Error, TEXT("Asset with same name exists. Can't overwrite another asset , Asset Name: %s "),
			       *SequenceName);
			continue;
		}

		// If not, create new one now.
		const bool bCreateAsset = !DestSeq;
		if (bCreateAsset)
		{
			DestSeq = NewObject<UAnimSequence>(ParentPackage, *SequenceName, RF_Public | RF_Standalone);
		}
		if (bCreateAsset)
		{
			// Notify the asset registry
			FAssetRegistryModule::AssetCreated(DestSeq);
		}
		
		if (!DestSeq->BoneCompressionSettings)
		{
			DestSeq->BoneCompressionSettings = FAnimationUtils::GetDefaultAnimationRecorderBoneCompressionSettings();
		}
		DestSeq->SetSkeleton(InSequence->GetSkeleton());
		IAnimationDataController& DestController = DestSeq->GetController();
		DestController.SetModel(DestSeq->GetDataModelInterface());
		DestController.OpenBracket(LOCTEXT("RecordAnimation", "Record Animation"));
		DestController.InitializeModel();

		RecordAnim_Internal(DestSeq, SourceBonesTransforms, StartFrame, EndFrame, FrameSize);

		DestController.NotifyPopulated();
		DestController.CloseBracket();
		// ReSharper disable once CppExpressionWithoutSideEffects
		DestSeq->MarkPackageDirty();

		UPackage* const Package = DestSeq->GetOutermost();
		FString const PackageName = Package->GetName();
		FString const PackageFileName = FPackageName::LongPackageNameToFilename(
			PackageName, FPackageName::GetAssetPackageExtension());

		FSavePackageArgs SaveArgs;
		SaveArgs.TopLevelFlags = RF_Standalone;
		SaveArgs.SaveFlags = SAVE_NoError;
		UPackage::SavePackage(Package, nullptr, *PackageFileName, SaveArgs);

		UE_LOG(LogAnimation, Log, TEXT("Animation Recorder saved %s "), *PackageName);
	}
}

#undef LOCTEXT_NAMESPACE
