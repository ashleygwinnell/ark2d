Replace vs2015 .sln and .vcxproj files with these. They have been set to compile for Win10/UWP.
Here's what's changed:
* No precompiled headers
* CharacterSet "NotSet" removed for x64
* PlatformToolset v141
* <RuntimeLibrary> add "DLL" to the end of the tag.
    <MinimumVisualStudioVersion>12.0</MinimumVisualStudioVersion>
    <AppContainerApplication>true</AppContainerApplication>
    <ApplicationType>Windows Store</ApplicationType>
    <ApplicationTypeRevision>10.0</ApplicationTypeRevision>
    <WindowsTargetPlatformVersion>10.0.14393.0</WindowsTargetPlatformVersion>
    <WindowsTargetPlatformMinVersion>10.0.10586.0</WindowsTargetPlatformMinVersion>
