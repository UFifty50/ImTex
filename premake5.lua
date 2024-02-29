workspace "ImTex"
    architecture "x86_64"
    configurations { "Debug", "Release" }
    flags { "MultiProcessorCompile" }
    startproject "ImTeX"
    debugdir "bin/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/ImTex"

    filter "configurations:Debug"
        defines { "IMTEX_DEBUG", "_DEBUG" }
        symbols "On"
        optimize "Off"

    filter "configurations:Release"
        defines { "IMTEX_RELEASE", "NDEBUG" }
        runtime "Release"
        symbols "Off"
        optimize "On"


outputDir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"


group "Dependencies"
    include "vendor/Glad"
    include "vendor/GLFW"

group ""
project "MicroTeX"
    location "MicroTeX"
    kind "StaticLib"
    staticruntime "off"
    language "C++"
    cppdialect "C++20"

    targetdir ("bin/" .. outputDir .. "/%{prj.name}")
    objdir ("bin/intermediate/" .. outputDir .. "/%{prj.name}")

    files {
        "%{prj.name}/src/atom/atom_basic.cpp",
        "%{prj.name}/src/atom/atom_char.cpp",
        "%{prj.name}/src/atom/atom_impl.cpp",
        "%{prj.name}/src/atom/atom_matrix.cpp",
        "%{prj.name}/src/atom/atom_row.cpp",
        "%{prj.name}/src/atom/atom_space.cpp",
        "%{prj.name}/src/atom/colors_def.cpp",
        "%{prj.name}/src/atom/unit_conversion.cpp",
        "%{prj.name}/src/box/box.cpp",
        "%{prj.name}/src/box/box_factory.cpp",
        "%{prj.name}/src/box/box_group.cpp",
        "%{prj.name}/src/box/box_single.cpp",
        "%{prj.name}/src/core/core.cpp",
        "%{prj.name}/src/core/formula.cpp",
        "%{prj.name}/src/core/formula_def.cpp",
        "%{prj.name}/src/core/glue.cpp",
        "%{prj.name}/src/core/localized_num.cpp",
        "%{prj.name}/src/core/macro.cpp",
        "%{prj.name}/src/core/macro_def.cpp",
        "%{prj.name}/src/core/macro_impl.cpp",
        "%{prj.name}/src/core/parser.cpp",
        "%{prj.name}/src/fonts/alphabet.cpp",
        "%{prj.name}/src/fonts/font_basic.cpp",
        "%{prj.name}/src/fonts/font_info.cpp",
        "%{prj.name}/src/fonts/fonts.cpp",
        "%{prj.name}/src/utils/string_utils.cpp",
        "%{prj.name}/src/utils/utf.cpp",
        "%{prj.name}/src/utils/utils.cpp",
        "%{prj.name}/src/res/builtin/formula_mappings.res.cpp",
        "%{prj.name}/src/res/builtin/symbol_mapping.res.cpp",
        "%{prj.name}/src/res/builtin/tex_param.res.cpp",
        "%{prj.name}/src/res/builtin/tex_symbols.res.cpp",
        "%{prj.name}/src/res/font/bi10.def.cpp",
        "%{prj.name}/src/res/font/bx10.def.cpp",
        "%{prj.name}/src/res/font/cmbsy10.def.cpp",
        "%{prj.name}/src/res/font/cmbx10.def.cpp",
        "%{prj.name}/src/res/font/cmbxti10.def.cpp",
        "%{prj.name}/src/res/font/cmex10.def.cpp",
        "%{prj.name}/src/res/font/cmmi10.def.cpp",
        "%{prj.name}/src/res/font/cmmi10_unchanged.def.cpp",
        "%{prj.name}/src/res/font/cmmib10.def.cpp",
        "%{prj.name}/src/res/font/cmmib10_unchanged.def.cpp",
        "%{prj.name}/src/res/font/cmr10.def.cpp",
        "%{prj.name}/src/res/font/cmss10.def.cpp",
        "%{prj.name}/src/res/font/cmssbx10.def.cpp",
        "%{prj.name}/src/res/font/cmssi10.def.cpp",
        "%{prj.name}/src/res/font/cmsy10.def.cpp",
        "%{prj.name}/src/res/font/cmti10.def.cpp",
        "%{prj.name}/src/res/font/cmti10_unchanged.def.cpp",
        "%{prj.name}/src/res/font/cmtt10.def.cpp",
        "%{prj.name}/src/res/font/dsrom10.def.cpp",
        "%{prj.name}/src/res/font/eufb10.def.cpp",
        "%{prj.name}/src/res/font/eufm10.def.cpp",
        "%{prj.name}/src/res/font/i10.def.cpp",
        "%{prj.name}/src/res/font/moustache.def.cpp",
        "%{prj.name}/src/res/font/msam10.def.cpp",
        "%{prj.name}/src/res/font/msbm10.def.cpp",
        "%{prj.name}/src/res/font/r10.def.cpp",
        "%{prj.name}/src/res/font/r10_unchanged.def.cpp",
        "%{prj.name}/src/res/font/rsfs10.def.cpp",
        "%{prj.name}/src/res/font/sb10.def.cpp",
        "%{prj.name}/src/res/font/sbi10.def.cpp",
        "%{prj.name}/src/res/font/si10.def.cpp",
        "%{prj.name}/src/res/font/special.def.cpp",
        "%{prj.name}/src/res/font/ss10.def.cpp",
        "%{prj.name}/src/res/font/stmary10.def.cpp",
        "%{prj.name}/src/res/font/tt10.def.cpp",
        "%{prj.name}/src/res/parser/font_parser.cpp",
        "%{prj.name}/src/res/parser/formula_parser.cpp",
        "%{prj.name}/src/res/reg/builtin_font_reg.cpp",
        "%{prj.name}/src/res/reg/builtin_syms_reg.cpp",
        "%{prj.name}/src/res/sym/amsfonts.def.cpp",
        "%{prj.name}/src/res/sym/amssymb.def.cpp",
        "%{prj.name}/src/res/sym/base.def.cpp",
        "%{prj.name}/src/res/sym/stmaryrd.def.cpp",
        "%{prj.name}/src/res/sym/symspecial.def.cpp",
        "%{prj.name}/src/latex.cpp",
        "%{prj.name}/src/render.cpp",
        "%{wks.location}/vendor/tinyxml2/tinyxml2.cpp"
    }

    includedirs {
        "%{prj.name}/src",
        "%{wks.location}/vendor/tinyxml2/"
    }

    links {
    --    "tinyxml2"
    }

    filter "system:windows"
        systemversion "latest"

        defines {
            "BUILD_WIN32",
            "_HAS_STD_BYTE=0",
            "_CRT_SECURE_NO_WARNINGS"
        }

    filter "system:linux"
        pic "On"
        systemversion "latest"

        defines {
            "BUILD_GTK"
        }

    filter "toolset:msc*"
        buildoptions { 
            "/utf-8",
            "/Zc:preprocessor"
        }


project "ImTex"
    location "ImTex"
    kind "ConsoleApp"
    staticruntime "off"
    language "C++"
    cppdialect "C++20"

    targetdir ("bin/" .. outputDir .. "/%{prj.name}")
    objdir ("bin/intermediate/" .. outputDir .. "/%{prj.name}")

    files {
        "%{prj.name}/src/cpp/**.cpp",
        "%{prj.name}/src/include/**.hpp",
        "%{wks.location}/vendor/nanovg/**.c",
        "%{wks.location}/vendor/nanovg/**.h"
    }

    includedirs {
        "%{prj.name}/src/include",
        "%{wks.location}/MicroTeX/src",
        "%{wks.location}/vendor/Glad/include",
        "%{wks.location}/vendor/GLFW/include",
        "%{wks.location}/vendor/nanovg/"
    }

    links {
        "MicroTeX",
        "Glad",
        "GLFW",
     --   "tinyxml2"
    }

    postbuildcommands {
            "{COPYDIR} ../MicroTeX/res/ ../bin/" .. outputDir .. "/ImTeX/res"
    }

    filter "system:windows"
        systemversion "latest"

        defines { "_CRT_SECURE_NO_WARNINGS" }

        links {
            "opengl32.lib",
        }

    filter "system:linux"
        pic "On"
        systemversion "latest"

        links {
            "X11",
            "Xrandr",
            "Xi",
            "GLU",
            "GL",
            "dl",
            "pthread",
        }

    filter "toolset:msc*"
        buildoptions { 
            "/utf-8",
            "/Zc:preprocessor"
        }