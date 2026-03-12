// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AttachmentManager.generated.h"

class AAttachmentTestActor;

UCLASS(Blueprintable)
class ATTACHMENT_API AAttachmentManager : public AActor
{
	GENERATED_BODY()

public:
	AAttachmentManager();

	UFUNCTION(BlueprintCallable, Category = "Attachment Test")
	void InitializeDefaultChain();

	UFUNCTION(BlueprintCallable, Category = "Attachment Test")
	void SwapParentAndChild();

	UFUNCTION(BlueprintCallable, Category = "Attachment Test")
	void ChangeLeafAttachment();

	UFUNCTION(BlueprintCallable, Category = "Attachment Test")
	void LogAttachmentState();

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Attachment Test")
	TObjectPtr<AAttachmentTestActor> Root;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Attachment Test")
	TObjectPtr<AAttachmentTestActor> Parent;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Attachment Test")
	TObjectPtr<AAttachmentTestActor> Child;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Attachment Test")
	TObjectPtr<AAttachmentTestActor> Leaf;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attachment Test")
	bool bKeepWorldTransform = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attachment Test")
	bool bLogOnScreen = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attachment Test")
	float OnScreenLogDuration = 5.0f;
};
