// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "BackUp/Private/Config/RealBandConfig.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeRealBandConfig() {}
// Cross Module References
	BACKUP_API UClass* Z_Construct_UClass_URealBandConfig();
	BACKUP_API UClass* Z_Construct_UClass_URealBandConfig_NoRegister();
	COREUOBJECT_API UClass* Z_Construct_UClass_UObject();
	UPackage* Z_Construct_UPackage__Script_BackUp();
// End Cross Module References
	void URealBandConfig::StaticRegisterNativesURealBandConfig()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(URealBandConfig);
	UClass* Z_Construct_UClass_URealBandConfig_NoRegister()
	{
		return URealBandConfig::StaticClass();
	}
	struct Z_Construct_UClass_URealBandConfig_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_URealBandConfig_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UObject,
		(UObject* (*)())Z_Construct_UPackage__Script_BackUp,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_URealBandConfig_Statics::Class_MetaDataParams[] = {
		{ "Comment", "/**\n * \n */" },
		{ "IncludePath", "Config/RealBandConfig.h" },
		{ "ModuleRelativePath", "Private/Config/RealBandConfig.h" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_URealBandConfig_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<URealBandConfig>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_URealBandConfig_Statics::ClassParams = {
		&URealBandConfig::StaticClass,
		"Editor",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		nullptr,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		0,
		0,
		0x000000A0u,
		METADATA_PARAMS(Z_Construct_UClass_URealBandConfig_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_URealBandConfig_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_URealBandConfig()
	{
		if (!Z_Registration_Info_UClass_URealBandConfig.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_URealBandConfig.OuterSingleton, Z_Construct_UClass_URealBandConfig_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_URealBandConfig.OuterSingleton;
	}
	template<> BACKUP_API UClass* StaticClass<URealBandConfig>()
	{
		return URealBandConfig::StaticClass();
	}
	DEFINE_VTABLE_PTR_HELPER_CTOR(URealBandConfig);
	struct Z_CompiledInDeferFile_FID_Projects_RealBandMultiUser_5_1_Plugins_BackUp_Source_BackUp_Private_Config_RealBandConfig_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Projects_RealBandMultiUser_5_1_Plugins_BackUp_Source_BackUp_Private_Config_RealBandConfig_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_URealBandConfig, URealBandConfig::StaticClass, TEXT("URealBandConfig"), &Z_Registration_Info_UClass_URealBandConfig, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(URealBandConfig), 2662331589U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Projects_RealBandMultiUser_5_1_Plugins_BackUp_Source_BackUp_Private_Config_RealBandConfig_h_2592327283(TEXT("/Script/BackUp"),
		Z_CompiledInDeferFile_FID_Projects_RealBandMultiUser_5_1_Plugins_BackUp_Source_BackUp_Private_Config_RealBandConfig_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Projects_RealBandMultiUser_5_1_Plugins_BackUp_Source_BackUp_Private_Config_RealBandConfig_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
