// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class BackUp : ModuleRules
{
	public BackUp(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
				Path.Combine(EngineDirectory, @"Plugins/Developer/Concert/ConcertApp/MultiUserClient/Source/MultiUserClient/Public"),
                Path.Combine(EngineDirectory, @"Plugins/Developer/Concert/ConcertSync/ConcertSyncClient/Source/ConcertSyncClient/Public"),
                Path.Combine(EngineDirectory, @"Plugins/Messaging/UdpMessaging/Source/UdpMessaging/Public/Shared")
            }
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
				Path.Combine(EngineDirectory, @"Plugins/Developer/Concert/ConcertApp/MultiUserClient/Source/MultiUserClient/Public"),
                Path.Combine(EngineDirectory, @"Plugins/Messaging/UdpMessaging/Source/UdpMessaging/Public/Shared")
            }
			);


        PrivateIncludePathModuleNames.AddRange(
                new string[]
                {
                    "UdpMessaging",
                }
            );

        PublicIncludePathModuleNames.AddRange(
                new string[]
                {
                    "UdpMessaging",
                }
            );

        PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
                "CoreUObject",
                "EditorSubsystem",
                "Engine",
                "Blutility",
				"Concert",
                "ConcertSyncCore",
                "ConcertSharedSlate",
                "ConcertSyncClient",
                "ConcertTransport",
                "MultiUserClient",
                "MultiUserServer",
                "EditorWidgets",
                "PropertyEditor",
				"Messaging",
                "UdpMessaging"
                //"WindowsTargetPlatform",
                //"DesktopPlatform"
				
				// ... add other public dependencies that you statically link with here ...
			}
            );
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Projects",
				"InputCore",
				"EditorFramework",
				"UnrealEd",
				"ToolMenus",
                "Concert",
                "ConcertSyncCore",
                "ConcertSharedSlate",
                "ConcertSyncClient",
                "ConcertTransport",
                "EditorWidgets",
                "MultiUserClient",
                "MultiUserServer",
                "CoreUObject",
				"Engine",
                "PropertyEditor",
                "EditorFramework",
                "Slate",
				"SlateCore",
                "Json",
                "UnrealEd",
                "HTTP",
				"Messaging",
                "UdpMessaging"
               // "WindowsTargetPlatform", // For ECompilerVersion
               // "DesktopPlatform"
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
