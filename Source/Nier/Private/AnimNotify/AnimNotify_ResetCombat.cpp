// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/AnimNotify_ResetCombat.h"
#include "Interfaces/PlayerInterface.h"

void UAnimNotify_ResetCombat::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (IPlayerInterface* PlayerInterface = Cast<IPlayerInterface>(MeshComp->GetOwner()))
	{
		PlayerInterface->Execute_ResetCombat(MeshComp->GetOwner());
	}
}
