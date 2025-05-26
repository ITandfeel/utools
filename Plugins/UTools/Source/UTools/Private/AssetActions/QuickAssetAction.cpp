// Copyright IT&feel, CA. All Rights Reserved.

#include "AssetActions/QuickAssetAction.h"

#include "AssetToolsModule.h"
#include "DebugHeader.h"
#include "EditorUtilityLibrary.h"
#include "EditorAssetLibrary.h"
#include "ObjectTools.h"
#include "AssetRegistry/AssetRegistryModule.h"

void UQuickAssetAction::DuplicateAssets(const int32 NumCopies)
{
	if (NumCopies <= 0)
	{
		ShowMessage(
			FText::FromString(TEXT("Invalid number of copies")),
			FText::FromString(TEXT("Warning")),
			EAppMsgType::Ok,
			true
		);

		return;
	}

	int32 NumSelectedAssets = 0;
	for (TArray<FAssetData> SelectedAssetsData = UEditorUtilityLibrary::GetSelectedAssetData();
	     const FAssetData& AssetData : SelectedAssetsData)
	{
		for (int32 i = 0; i < NumCopies; i++)
		{
			FString SourceAssetPath = AssetData.GetSoftObjectPath().ToString();
			FString DuplicatedAssetName = AssetData.AssetName.ToString() + FString::Printf(TEXT("_%d"), i);
			FString NewAssetPath = FPaths::Combine(AssetData.PackagePath.ToString(), DuplicatedAssetName);

			Print(FString::Printf(TEXT("AssetPath: %s"), *SourceAssetPath), FColor::Green);
			Print(FString::Printf(TEXT("AssetName: %s"), *DuplicatedAssetName), FColor::Green);

			if (UEditorAssetLibrary::DuplicateAsset(SourceAssetPath, NewAssetPath))
			{
				UEditorAssetLibrary::SaveAsset(NewAssetPath, false);
				++NumSelectedAssets;
			}
		}

		if (NumSelectedAssets > 0)
		{
			ShowNotifyInfo(FString::Printf(TEXT("Duplicated %d assets"), NumSelectedAssets));
		}
	}
}

void UQuickAssetAction::AddPrefix()
{
	int32 Counter = 0;

	for (TArray<UObject*> SelectedAssets = UEditorUtilityLibrary::GetSelectedAssets(); UObject* Asset : SelectedAssets)
	{
		if (nullptr == Asset) { continue; }

		const FString* PrefixFound = PrefixMap.Find(Asset->GetClass());

		if (nullptr == PrefixFound || PrefixFound->IsEmpty())
		{
			Print(FString::Printf(TEXT("No prefix found for %s"), *Asset->GetClass()->GetName()), FColor::Red);
			continue;
		}

		FString OldName = Asset->GetName();

		if (OldName.StartsWith(*PrefixFound))
		{
			Print(FString::Printf(TEXT("Prefix already exists for %s"), *OldName), FColor::Red);
			continue;
		}

		if (Asset->IsA<UMaterialInstanceConstant>())
		{
			OldName.RemoveFromStart(TEXT("M_"));
			OldName.RemoveFromEnd(TEXT("_Inst"));
		}

		const FString NewNameWithPrefix = *PrefixFound + OldName;
		UEditorUtilityLibrary::RenameAsset(Asset, NewNameWithPrefix);
		++Counter;
	}

	if (Counter > 0)
	{
		ShowNotifyInfo(FString::Printf(TEXT("Added prefix to %d assets"), Counter));
	}
}

void UQuickAssetAction::RemoveUnusedAssets()
{
	TArray<FAssetData> SelectedAssetsData = UEditorUtilityLibrary::GetSelectedAssetData();
	TArray<FAssetData> UnusedAssetsData;

	FixUpRedirects();

	for (const FAssetData& AssetData : SelectedAssetsData)
	{
		if (TArray<FString> References = UEditorAssetLibrary::FindPackageReferencersForAsset(
			AssetData.GetSoftObjectPath().ToString()); 0 == References.Num())
		{
			UnusedAssetsData.Add(AssetData);
		}
	}

	if (0 == UnusedAssetsData.Num())
	{
		ShowMessage(
			FText::FromString(TEXT("No unused assets found")),
			FText::FromString(TEXT("Info")),
			EAppMsgType::Ok,
			false
		);

		return;
	}

	const int32 NumDeletedAssets = ObjectTools::DeleteAssets(UnusedAssetsData, true);
	if (0 == NumDeletedAssets) { return; }

	ShowNotifyInfo(FString::Printf(TEXT("Removed %d unused assets"), NumDeletedAssets));
}

void UQuickAssetAction::FixUpRedirects()
{
	TArray<UObjectRedirector*> RedirectorsToFixArray;

	const FAssetRegistryModule& AssetRegistryModule = FModuleManager::Get().LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");

	FARFilter Filter;
	Filter.bRecursivePaths = true;
	Filter.PackagePaths.Emplace(TEXT("/Game"));
	Filter.ClassPaths.Emplace(UObjectRedirector::StaticClass()->GetFName());
	
	TArray<FAssetData> OutRedirectors;
	AssetRegistryModule.Get().GetAssets(Filter, OutRedirectors);

	for (const FAssetData& AssetData : OutRedirectors)
	{
		if (UObjectRedirector* RedirectorToFix = Cast<UObjectRedirector>(AssetData.GetAsset()))
		{
			RedirectorsToFixArray.Add(RedirectorToFix);
		}
	}

	const FAssetToolsModule& AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools");
	AssetToolsModule.Get().FixupReferencers(RedirectorsToFixArray);
}
