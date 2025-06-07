// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class MenuSystem : ModuleRules
{
	public MenuSystem(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] {
            "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput"
        });

        if (Target.Type != TargetType.Server)
        {
            // 서버 빌드가 아닌 경우 (즉, 클라이언트 전용 빌드일 때만 UI 관련 모듈 포함)
            PrivateDependencyModuleNames.AddRange(new string[] {
                "Slate", "SlateCore", "UMG"
            });
        }

        // Steam 모듈은 서버/클라이언트 모두 필요할 수 있으므로 별도로 유지
        PrivateDependencyModuleNames.AddRange(new string[] {
            "OnlineSubsystem", "OnlineSubsystemSteam", "OnlineSubsystemUtils"
        });

        AddEngineThirdPartyPrivateStaticDependencies(Target, "Steamworks");

        if (Target.Type == TargetType.Server)
        {
            // Steamworks 라이브러리 경로를 직접 지정
            string SteamSDKPath = Path.Combine(Target.RelativeEnginePath, "Plugins", "Online", "OnlineSubsystemSteam", "Source", "ThirdParty", "Steamworks", "Steamv154", "sdk");
            PublicAdditionalLibraries.Add(Path.Combine(SteamSDKPath, "redistributable_bin", "win64", "steam_api64.lib"));
        }
    }
}
