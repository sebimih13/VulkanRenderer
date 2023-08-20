@echo off
setlocal

rem Retrieve the value of the VULKAN_SDK_PATH environment variable
set "VulkanSDKPath=%VK_SDK_PATH%"

rem Define the URL for the Vulkan SDK installer
set "vulkanSdkUrl=https://sdk.lunarg.com/sdk/download/latest/windows/vulkan-sdk.exe"

rem Check if the VK_SDK_PATH environment variable is defined
if not defined VulkanSDKPath (
    echo VK_SDK_PATH is not defined. Downloading Vulkan SDK...

    rem Download the Vulkan SDK installer
    curl -o "vulkan-sdk.exe" "%vulkanSdkUrl%"

    rem Install the Vulkan SDK
    start "" "vulkan-sdk.exe"

    rem Wait for the user to finish the installation
    echo Please install the Vulkan SDK. Press any key to continue...
    pause > nul

    rem Retrieve the VK_SDK_PATH environment variable after installation
    set "VulkanSDKPath=%VK_SDK_PATH%"
)

rem Check if the environment variable is defined
if defined VulkanSDKPath (
    echo VK_SDK_PATH is defined as: %VulkanSDKPath%

    rem Check if the "include" folder exists in the VK_SDK_PATH
    if exist "%VulkanSDKPath%\Include" (
        echo Copying "include" folder...
        xcopy "%VulkanSDKPath%\Include" "%~dp0vendor\VulkanSDK\include\" /E /I /Y
        echo "include" folder copied successfully.
    ) else (
        echo "include" folder not found in VK_SDK_PATH.
    )
    
    rem Check if the "Lib" folder exists in the VK_SDK_PATH
    if exist "%VulkanSDKPath%\Lib" (
        echo Copying "Lib" folder...
        xcopy "%VulkanSDKPath%\Lib" "%~dp0vendor\VulkanSDK\lib\" /E /I /Y
        echo "Lib" folder copied successfully.
    ) else (
        echo "Lib" folder not found in VK_SDK_PATH.
    )
) else (
    echo VK_SDK_PATH is not defined.
    echo Please download Vulkan SDK
)

endlocal

call vendor\premake5\premake5.exe vs2022
PAUSE
