// Copyright IT&feel, CA. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AssetActionUtility.h"
#include "WidgetBlueprint.h"
#include "Blueprint/UserWidget.h"
#include "Curves/CurveLinearColor.h"
#include "Curves/CurveVector.h"
#include "Engine/UserDefinedStruct.h"
#include "Materials/MaterialFunctionInstance.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Materials/MaterialParameterCollection.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundCue.h"
#include "NiagaraSystem.h"
#include "NiagaraEmitter.h"
#include "Rig/IKRigDefinition.h"
#include "Retargeter/IKRetargeter.h"
#include "ControlRig.h"
#include "FoliageType_Actor.h"
#include "FoliageType_InstancedStaticMesh.h"
#include "GeometryCollection/GeometryCollectionObject.h"
#include "PhysicsEngine/PhysicsAsset.h"
#include "InputMappingContext.h"
#include "Sound/SoundAttenuation.h"
#include "MetasoundSource.h"

#include "QuickAssetAction.generated.h"

/**
 *  Quick Asset Actions for duplicating and renaming assets
 */
UCLASS()
class UTOOLS_API UQuickAssetAction : public UAssetActionUtility
{
	GENERATED_BODY()

public:
	UFUNCTION(CallInEditor)
	static void DuplicateAssets(int32 NumCopies);

	UFUNCTION(CallInEditor)
	void AddPrefix();

	UFUNCTION(CallInEditor)
	static void RemoveUnusedAssets();

private:
	TMap<UClass*, FString> PrefixMap =
	{
		// Blueprints
		{UBlueprint::StaticClass(), TEXT("BP_")},
		{UBlueprintGeneratedClass::StaticClass(), TEXT("BPGC_")},
		{UBlueprintFunctionLibrary::StaticClass(), TEXT("BFL_")},
		{UUserWidget::StaticClass(), TEXT("WBP_")},
		{UWidgetBlueprint::StaticClass(), TEXT("WBP_")},
		{UInterface::StaticClass(), TEXT("I_")},

		// Materials
		{UMaterial::StaticClass(), TEXT("M_")},
		{UMaterialInstanceConstant::StaticClass(), TEXT("MI_")},
		{UMaterialFunction::StaticClass(), TEXT("MF_")},
		{UMaterialFunctionInterface::StaticClass(), TEXT("MFI_")},
		{UMaterialFunctionInstance::StaticClass(), TEXT("MFI_")},
		{UMaterialParameterCollection::StaticClass(), TEXT("MPC_")},

		// Textures
		{UTexture::StaticClass(), TEXT("T_")},
		{UTexture2D::StaticClass(), TEXT("T_")},

		// Sounds
		{USoundWave::StaticClass(), TEXT("SW_")},
		{USoundCue::StaticClass(), TEXT("SC_")},

		// Meshes
		{UStaticMesh::StaticClass(), TEXT("SM_")},
		{USkeletalMesh::StaticClass(), TEXT("SKM_")},

		// Skeleton
		{USkeleton::StaticClass(), TEXT("SK_")},

		// PhysicalAsset
		{UPhysicsAsset::StaticClass(), TEXT("PA_")},

		// Animations
		{UAnimSequence::StaticClass(), TEXT("AS_")},
		{UAnimBlueprint::StaticClass(), TEXT("ABP_")},
		{UAnimMontage::StaticClass(), TEXT("AM_")},

		// Particle and Effects
		{UParticleSystem::StaticClass(), TEXT("PS_")},
		{UNiagaraSystem::StaticClass(), TEXT("NS_")},
		{UNiagaraEmitter::StaticClass(), TEXT("NE_")},

		// Curves
		{UCurveFloat::StaticClass(), TEXT("CF_")},
		{UCurveLinearColor::StaticClass(), TEXT("CLC_")},
		{UCurveVector::StaticClass(), TEXT("CV_")},
		{UCurveTable::StaticClass(), TEXT("CT_")},

		// Data
		{UDataTable::StaticClass(), TEXT("DT_")},
		{UUserDefinedStruct::StaticClass(), TEXT("UDS_")},
		{UEnum::StaticClass(), TEXT("E_")},

		// IK Rig
		{UIKRigDefinition::StaticClass(), TEXT("IK_")},

		// IK Retargeter
		{UIKRetargeter::StaticClass(), TEXT("RTG_")},

		// Control Rig
		{UControlRig::StaticClass(), TEXT("CR_")},

		// Geometry Collection
		{UGeometryCollection::StaticClass(), TEXT("GC_")},

		// Input
		{UInputMappingContext::StaticClass(), TEXT("IMC_")},
		{UInputAction::StaticClass(), TEXT("IA_")},

		// Sound
		{USoundAttenuation::StaticClass(), TEXT("SA_")},
		{UMetaSoundSource::StaticClass(), TEXT("MS_")},

		// Foliage		
		{UFoliageType_Actor::StaticClass(), TEXT("AF_")},
		{UFoliageType_InstancedStaticMesh::StaticClass(), TEXT("SMF_")}
	};

	static void FixUpRedirects();
};
