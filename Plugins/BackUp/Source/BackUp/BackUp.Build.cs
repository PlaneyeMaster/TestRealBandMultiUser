// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class BackUp : ModuleRules
{
	public BackUp(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
        //bUsePrecompiled = true;

        PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
				Path.Combine(EngineDirectory, @"Plugins/Developer/Concert/ConcertApp/MultiUserClient/Source/MultiUserClient/Public"),
                Path.Combine(EngineDirectory, @"Plugins/Developer/Concert/ConcertSync/ConcertSyncClient/Source/ConcertSyncClient/Public"),
                Path.Combine(EngineDirectory, @"Plugins/Messaging/UdpMessaging/Source/UdpMessaging/Public/Shared"),
                Path.Combine(EngineDirectory, @"Plugins/VirtualProduction/Takes/Source/TakeRecorder/Public"),
                Path.Combine(EngineDirectory, @"Plugins/VirtualProduction/Takes/Source/TakeTrackRecorders/Public"),
                Path.Combine(EngineDirectory, @"Plugins/Developer/Concert/ConcertSync/ConcertSyncServer/Source/ConcertSyncServer/Public")

            }
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
				Path.Combine(EngineDirectory, @"Plugins/Developer/Concert/ConcertApp/MultiUserClient/Source/MultiUserClient/Public"),
                Path.Combine(EngineDirectory, @"Plugins/Messaging/UdpMessaging/Source/UdpMessaging/Public/Shared"),
                Path.Combine(EngineDirectory, @"Plugins/Developer/GitSourceControl/Source/GitSourceControl/Private/")
            }
			);


        PrivateIncludePathModuleNames.AddRange(
                new string[]
                {
                    "UdpMessaging",    
                    "GitSourceControl"
                }
            );

        PublicIncludePathModuleNames.AddRange(
                new string[]
                {
                    "UdpMessaging"
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
                "UdpMessaging",
                "GitSourceControl",
                "TakeMovieScene",
                "Sockets"
                
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
                "UdpMessaging",
                "GitSourceControl",
                "SourceControl",
                "SourceControlWindows",
                "DesktopWidgets",
                "TakeMovieScene"
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
