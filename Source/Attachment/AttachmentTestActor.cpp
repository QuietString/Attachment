// Copyright Epic Games, Inc. All Rights Reserved.

#include "AttachmentTestActor.h"

#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

AAttachmentTestActor::AAttachmentTestActor()
{
	bReplicates = true;
	SetReplicateMovement(true);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
	Mesh->SetIsReplicated(true);
	Mesh->SetMobility(EComponentMobility::Movable);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (CubeMesh.Succeeded())
	{
		Mesh->SetStaticMesh(CubeMesh.Object);
	}
}

void AAttachmentTestActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (MaterialOverride)
	{
		Mesh->SetMaterial(0, MaterialOverride);
	}
}
