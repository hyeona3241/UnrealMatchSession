// MenuSystemServer.Target.cs

using UnrealBuildTool;
using System.Collections.Generic;

public class MenuSystemServerTarget : TargetRules
{
    public MenuSystemServerTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Server;
        DefaultBuildSettings = BuildSettingsVersion.V5;
        IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_4;
        ExtraModuleNames.Add("MenuSystem");
    }
}