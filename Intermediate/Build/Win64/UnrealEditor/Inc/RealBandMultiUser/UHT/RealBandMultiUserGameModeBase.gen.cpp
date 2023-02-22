// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "RealBandMultiUser/RealBandMultiUserGameModeBase.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeRealBandMultiUserGameModeBase() {}
// Cross Module References
	ENGINE_API UClass* Z_Construct_UClass_AGameModeBase();
	REALBANDMULTIUSER_API UClass* Z_Construct_UClass_ARealBandMultiUserGameModeBase();
	REALBANDMULTIUSER_API UClass* Z_Construct_UClass_ARealBandMultiUserGameModeBase_NoRegister();
	UPackage* Z_Construct_UPackage__Script_RealBandMultiUser();
// End Cross Module References
	void ARealBandMultiUserGameModeBase::StaticRegisterNativesARealBandMultiUserGameModeBase()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(ARealBandMultiUserGameModeBase);
	UClass* Z_Construct_UClass_ARealBandMultiUserGameModeBase_NoRegister()
	{
		return ARealBandMultiUserGameModeBase::StaticClass();
	}
	struct Z_Construct_UClass_ARealBandMultiUserGameModeBase_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_ARealBandMultiUserGameModeBase_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_AGameModeBase,
		(UObject* (*)())Z_Construct_UPackage__Script_RealBandMultiUser,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ARealBandMultiUserGameModeBase_Statics::Class_MetaDataParams[] = {
		{ "Comment", "/**\n * \n */" },
		{ "HideCategories", "Info Rendering MovementReplication Replication Actor Input Movement Collision Rendering HLOD WorldPartition DataLayers Transformation" },
		{ "IncludePath", "RealBandMultiUserGameModeBase.h" },
		{ "ModuleRelativePath", "RealBandMultiUserGameModeBase.h" },
		{ "ShowCategories", "Input|MouseInput Input|TouchInput" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_ARealBandMultiUserGameModeBase_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<ARealBandMultiUserGameModeBase>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_ARealBandMultiUserGameModeBase_Statics::ClassParams = {
		&ARealBandMultiUserGameModeBase::StaticClass,
		"Game",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		nullptr,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		0,
		0,
		0x009002ACu,
		METADATA_PARAMS(Z_Construct_UClass_ARealBandMultiUserGameModeBase_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_ARealBandMultiUserGameModeBase_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_ARealBandMultiUserGameModeBase()
	{
		if (!Z_Registration_Info_UClass_ARealBandMultiUserGameModeBase.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_ARealBandMultiUserGameModeBase.OuterSingleton, Z_Construct_UClass_ARealBandMultiUserGameModeBase_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_ARealBandMultiUserGameModeBase.OuterSingleton;
	}
	template<> REALBANDMULTIUSER_API UClass* StaticClass<ARealBandMultiUserGameModeBase>()
	{
		return ARealBandMultiUserGameModeBase::StaticClass();
	}
	DEFINE_VTABLE_PTR_HELPER_CTOR(ARealBandMultiUserGameModeBase);
	ARealBandMultiUserGameModeBase::~ARealBandMultiUserGameModeBase() {}
	struct Z_CompiledInDeferFile_FID_Projects_RealBandMultiUser_5_1_Source_RealBandMultiUser_RealBandMultiUserGameModeBase_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Projects_RealBandMultiUser_5_1_Source_RealBandMultiUser_RealBandMultiUserGameModeBase_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_ARealBandMultiUserGameModeBase, ARealBandMultiUserGameModeBase::StaticClass, TEXT("ARealBandMultiUserGameModeBase"), &Z_Registration_Info_UClass_ARealBandMultiUserGameModeBase, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(ARealBandMultiUserGameModeBase), 3083726758U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Projects_RealBandMultiUser_5_1_Source_RealBandMultiUser_RealBandMultiUserGameModeBase_h_376582190(TEXT("/Script/RealBandMultiUser"),
		Z_CompiledInDeferFile_FID_Projects_RealBandMultiUser_5_1_Source_RealBandMultiUser_RealBandMultiUserGameModeBase_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Projects_RealBandMultiUser_5_1_Source_RealBandMultiUser_RealBandMultiUserGameModeBase_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
