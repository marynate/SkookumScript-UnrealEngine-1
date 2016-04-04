﻿// Copyright 2000 Agog Labs Inc., All Rights Reserved.
using System.IO;
using System.Net;
using System.Diagnostics;
using System.Collections.Generic;
using UnrealBuildTool;


public class SkookumScript : ModuleRules
{
  public SkookumScript(TargetInfo Target)
  { 
    // Check if Sk source code is present (Pro-RT license) 
    var bFullSource = File.Exists(Path.Combine(ModuleDirectory, "Private", "SkookumScript", "SkookumScript.cpp"));
    // Allow packaging script to force a lib build by creating a temp file (Agog Labs internal)
    bFullSource = bFullSource && !File.Exists(Path.Combine(ModuleDirectory, "force-lib-build.txt"));

    // If full source is present, build module from source, otherwise link with binary library
    Type = bFullSource ? ModuleType.CPlusPlus : ModuleType.External;
    
    var bPlatformAllowed = false;

    List<string> platPathSuffixes = new List<string>();

    string libNameExt = ".a";
    string libNamePrefix = "lib";
    string libNameSuffix = "";
    string platformName = "";
    bool useDebugCRT = BuildConfiguration.bDebugBuildsActuallyUseDebugCRT;

    switch (Target.Platform)
    {
      case UnrealTargetPlatform.Win32:
        bPlatformAllowed = true;
        platformName = "Win32";
        platPathSuffixes.Add(Path.Combine(platformName, WindowsPlatform.Compiler == WindowsCompiler.VisualStudio2015 ? "VS2015" : "VS2013"));
        libNameExt = ".lib";
        libNamePrefix = "";
        Definitions.Add("WIN32_LEAN_AND_MEAN");
        break;
      case UnrealTargetPlatform.Win64:
        bPlatformAllowed = true;
        platformName = "Win64";
        platPathSuffixes.Add(Path.Combine(platformName, WindowsPlatform.Compiler == WindowsCompiler.VisualStudio2015 ? "VS2015" : "VS2013"));
        libNameExt = ".lib";
        libNamePrefix = "";
        Definitions.Add("WIN32_LEAN_AND_MEAN");
        break;
      case UnrealTargetPlatform.Mac:
        bPlatformAllowed = true;
        platformName = "Mac";
        platPathSuffixes.Add(platformName);
        useDebugCRT = true;
        break;
      case UnrealTargetPlatform.IOS:
        bPlatformAllowed = true;
        platformName = "IOS";
        platPathSuffixes.Add(platformName);
        useDebugCRT = true;
        break;
      case UnrealTargetPlatform.TVOS:
        bPlatformAllowed = true;
        platformName = "TVOS";
        platPathSuffixes.Add(platformName);
        useDebugCRT = true;
        break;
      case UnrealTargetPlatform.Android:
        bPlatformAllowed = true;
        platformName = "Android";
        platPathSuffixes.Add(Path.Combine(platformName, "ARM"));
        platPathSuffixes.Add(Path.Combine(platformName, "ARM64"));
        platPathSuffixes.Add(Path.Combine(platformName, "x86"));
        platPathSuffixes.Add(Path.Combine(platformName, "x64"));
        useDebugCRT = true;
        break;
    }

    // NOTE: All modules inside the SkookumScript plugin folder must use the exact same definitions!
    switch (Target.Configuration)
    {
      case UnrealTargetConfiguration.Debug:
      case UnrealTargetConfiguration.DebugGame:
        Definitions.Add("SKOOKUM=31");
        libNameSuffix = useDebugCRT ? "-Debug" : "-DebugCRTOpt";
        break;

      case UnrealTargetConfiguration.Development:
      case UnrealTargetConfiguration.Test:
        Definitions.Add("SKOOKUM=31");
        libNameSuffix = "-Development";
        break;

      case UnrealTargetConfiguration.Shipping:
        Definitions.Add("SKOOKUM=8");
        libNameSuffix = "-Shipping";
        break;
    }

    // Determine if monolithic build
    var bIsMonolithic = (!Target.bIsMonolithic.HasValue || (bool)Target.bIsMonolithic); // Assume monolithic if not specified

    if (!bIsMonolithic)
    {
      Definitions.Add("SK_IS_DLL");
    }

    // Public include paths
    PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Public"));

    // Public dependencies
    PublicDependencyModuleNames.Add("AgogCore");

    if (bFullSource)
    {
      // We're building SkookumScript from source - not much else needed
      PrivateIncludePaths.Add(Path.Combine(ModuleDirectory, "Private"));
      // Build system wants us to be dependent on some module with precompiled headers, so be it
      PrivateDependencyModuleNames.Add("Core");
    }
    else if (bPlatformAllowed)
    {
      var moduleName = "SkookumScript";
      var libFileNameStem = libNamePrefix + moduleName + (bIsMonolithic ? "" : "-" + platformName) + libNameSuffix;
      var libFileName = libFileNameStem + libNameExt;
      var libDirPathBase = Path.Combine(ModuleDirectory, "Lib");
      // Add library paths to linker parameters
      foreach (var platPathSuffix in platPathSuffixes)
      {
        var libDirPath = Path.Combine(libDirPathBase, platPathSuffix);
        var libFilePath = Path.Combine(libDirPath, libFileName);

        PublicLibraryPaths.Add(libDirPath);

        // For non-Android, add full path
        if (Target.Platform != UnrealTargetPlatform.Android)
        {
          PublicAdditionalLibraries.Add(libFilePath);
        }
      }

      // For Android, just add core of library name, e.g. "SkookumScript-Development"
      if (Target.Platform == UnrealTargetPlatform.Android)
      {
        PublicAdditionalLibraries.Add(moduleName + libNameSuffix);
      }
    }
  }    
}
