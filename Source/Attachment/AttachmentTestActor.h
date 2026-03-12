// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AttachmentTestActor.generated.h"

class UStaticMeshComponent;
class UMaterialInterface;

UCLASS(Blueprintable)
class ATTACHMENT_API AAttachmentTestActor : public AActor
{
	GENERATED_BODY()

public:
	AAttachmentTestActor();

	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attachment Test")
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attachment Test")
	bool bIsLeaf = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attachment Test")
	TObjectPtr<UMaterialInterface> MaterialOverride;
};
