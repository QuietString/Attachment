// Copyright Epic Games, Inc. All Rights Reserved.

#include "AttachmentManager.h"

#include "AttachmentTestActor.h"
#include "Components/SceneComponent.h"
#include "Engine/Engine.h"

namespace
{
	static FString GetActorLabelSafe(const AActor* Actor)
	{
		if (!Actor)
		{
			return TEXT("None");
		}
#if WITH_EDITOR
		return Actor->GetActorNameOrLabel();
#else
		return Actor->GetName();
#endif
	}

	static AActor* GetAttachParentActor(const AActor* Actor)
	{
		if (!Actor)
		{
			return nullptr;
		}

		const USceneComponent* RootComp = Actor->GetRootComponent();
		if (!RootComp)
		{
			return nullptr;
		}

		const USceneComponent* ParentComp = RootComp->GetAttachParent();
		return ParentComp ? ParentComp->GetOwner() : nullptr;
	}

	static void GetAttachChildrenActors(const AActor* Actor, TArray<AActor*>& OutActors)
	{
		OutActors.Reset();
		if (!Actor)
		{
			return;
		}

		const USceneComponent* RootComp = Actor->GetRootComponent();
		if (!RootComp)
		{
			return;
		}

		for (const USceneComponent* ChildComp : RootComp->GetAttachChildren())
		{
			if (ChildComp && ChildComp->GetOwner())
			{
				OutActors.Add(ChildComp->GetOwner());
			}
		}
	}

	static FString BuildActorAttachmentInfo(const AActor* Actor)
	{
		const FString ActorLabel = GetActorLabelSafe(Actor);
		const FString ParentLabel = GetActorLabelSafe(GetAttachParentActor(Actor));

		TArray<AActor*> ChildActors;
		GetAttachChildrenActors(Actor, ChildActors);

		FString ChildrenList;
		for (int32 Index = 0; Index < ChildActors.Num(); ++Index)
		{
			if (Index > 0)
			{
				ChildrenList += TEXT(", ");
			}
			ChildrenList += GetActorLabelSafe(ChildActors[Index]);
		}

		return FString::Printf(TEXT("%s | Parent=%s | Children=[%s]"), *ActorLabel, *ParentLabel, *ChildrenList);
	}
}

AAttachmentManager::AAttachmentManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AAttachmentManager::InitializeDefaultChain()
{
	if (!Root || !Parent || !Child || !Leaf)
	{
		UE_LOG(LogTemp, Warning, TEXT("InitializeDefaultChain: missing actor reference(s)."));
		return;
	}

	const FAttachmentTransformRules Rules = bKeepWorldTransform
		? FAttachmentTransformRules::KeepWorldTransform
		: FAttachmentTransformRules::KeepRelativeTransform;

	Parent->AttachToActor(Root, Rules);
	Child->AttachToActor(Parent, Rules);
	Leaf->AttachToActor(Child, Rules);

	LogAttachmentState();
}

void AAttachmentManager::SwapParentAndChild()
{
	if (!Root || !Parent || !Child)
	{
		UE_LOG(LogTemp, Warning, TEXT("SwapParentAndChild: missing actor reference(s)."));
		return;
	}

	const FAttachmentTransformRules Rules = bKeepWorldTransform
		? FAttachmentTransformRules::KeepWorldTransform
		: FAttachmentTransformRules::KeepRelativeTransform;

	// Step 1: both attach to Root
	Child->AttachToActor(Root, Rules);
	Parent->AttachToActor(Root, Rules);

	// Step 2: Parent becomes child of Child
	Parent->AttachToActor(Child, Rules);

	Swap(Parent, Child);

	LogAttachmentState();
}

void AAttachmentManager::ChangeLeafAttachment()
{
	if (!Leaf || !Child)
	{
		UE_LOG(LogTemp, Warning, TEXT("ChangeLeafAttachment: missing actor reference(s)."));
		return;
	}

	const USceneComponent* LeafRoot = Leaf->GetRootComponent();
	if (!LeafRoot)
	{
		UE_LOG(LogTemp, Warning, TEXT("ChangeLeafAttachment: Leaf has no root component."));
		return;
	}

	const USceneComponent* CurrentParent = LeafRoot->GetAttachParent();
	const USceneComponent* NextAncestor = CurrentParent ? CurrentParent->GetAttachParent() : nullptr;
	AActor* NewParentActor = NextAncestor ? NextAncestor->GetOwner() : nullptr;

	if (!NewParentActor)
	{
		NewParentActor = Child;
	}

	const FAttachmentTransformRules Rules = bKeepWorldTransform
		? FAttachmentTransformRules::KeepWorldTransform
		: FAttachmentTransformRules::KeepRelativeTransform;

	Leaf->AttachToActor(NewParentActor, Rules);

	LogAttachmentState();
}

void AAttachmentManager::LogAttachmentState()
{
	const FString RootInfo = BuildActorAttachmentInfo(Root);
	const FString ParentInfo = BuildActorAttachmentInfo(Parent);
	const FString ChildInfo = BuildActorAttachmentInfo(Child);
	const FString LeafInfo = BuildActorAttachmentInfo(Leaf);

	const FString Combined = FString::Printf(TEXT("Attachment State:\n%s\n%s\n%s\n%s"), *RootInfo, *ParentInfo, *ChildInfo, *LeafInfo);

	UE_LOG(LogTemp, Log, TEXT("%s"), *Combined);

	if (bLogOnScreen && GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, OnScreenLogDuration, FColor::Cyan, Combined);
	}
}
