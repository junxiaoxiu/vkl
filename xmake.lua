add_rules("mode.debug", "mode.release")
add_rules("plugin.compile_commands.autoupdate",{outputdir ="build"})

set_toolchains("msvc")

set_languages("c++17")

target("vulkan_learn")
    set_kind("binary")
    add_files("src/*.cpp")
    add_files("vkl/*.cpp")
    add_includedirs("include")
    -- vulkan
    add_includedirs("$(env VK_SDK_PATH)/Include")
    add_linkdirs("$(env VK_SDK_PATH)/Lib")
    add_links("vulkan-1")
    --SDL
    add_linkdirs("$(env VK_SDK_PATH)/Bin")
    add_links("SDL2")
