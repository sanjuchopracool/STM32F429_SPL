import qbs 1.0
import qbs.FileInfo

Product {
    type: ["application", "hex", "bin", "size"]
    Depends { name: "cpp" }

    Rule {
        id: hex
        inputs: ["application"]
        prepare: {
            var args = ["-O", "ihex", input.filePath, output.filePath];
            var objcopyPath = input.cpp.objcopyPath
            var cmd = new Command(objcopyPath, args);
            cmd.description = "converting to hex: " + FileInfo.fileName(input.filePath);
            cmd.highlight = "linker";
            return cmd;

        }
        Artifact {
            fileTags: ["hex"]
            filePath: FileInfo.baseName(input.filePath) + ".hex"
        }
    }

    Rule {
        id: bin
        inputs: ["application"]
        prepare: {
            var objcopyPath = input.cpp.objcopyPath
            var args = ["-O", "binary", input.filePath, output.filePath];
            var cmd = new Command(objcopyPath, args);
            cmd.description = "converting to bin: "+ FileInfo.fileName(input.filePath);
            cmd.highlight = "linker";
            return cmd;

        }
        Artifact {
            fileTags: ["bin"]
            filePath: FileInfo.baseName(input.filePath) + ".bin"
        }
    }

    Rule {
        id: size
        inputs: ["application"]
        alwaysRun: true
        prepare: {
            var sizePath = input.cpp.toolchainPrefix + "size"
            var args = [input.filePath];
            var cmd = new Command(sizePath, args);
            cmd.description = "File size: " + FileInfo.fileName(input.filePath);
            cmd.highlight = "linker";
            return cmd;
        }
        Artifact {
            fileTags: ["size"]
            filePath: undefined
        }
    }

    FileTagger {
        patterns: "*.ld"
        fileTags: ["linkerscript"]
    }

    cpp.cLanguageVersion: "c11"
    cpp.cxxLanguageVersion: "c++11"
    cpp.positionIndependentCode: false
    cpp.optimization: "none"
    cpp.debugInformation: true
    cpp.enableExceptions: false
    cpp.enableRtti: false
    cpp.enableReproducibleBuilds: true
    cpp.treatSystemHeadersAsDependencies: true

    cpp.driverFlags:
        [
        "-mcpu=cortex-m4",
        "-mthumb",
        "-mfloat-abi=softfp",
        "-mfpu=fpv4-sp-d16",
        "-Os",
        "-ffunction-sections",
        "-fdata-sections",
        "-nodefaultlibs",
        "-Wdouble-promotion",
        "-Wall",
        "-flto"
    ]

    cpp.cxxFlags: [
    ]

    cpp.linkerFlags: [
        "--gc-sections",
        "-u,Reset_Handler"
    ]


    cpp.defines: [
        "STM32F429_439xx",
        "USE_FULL_ASSERT",
        "USE_STDPERIPH_DRIVER",
        "HSE_VALUE=8000000"
    ]


    property  string libraryPath: "./Libraries"
    property string librarySourcesPath : libraryPath + "/STM32F4xx_StdPeriph_Driver/src/"
    cpp.includePaths:
        [
        ".",
        "./Config",
        libraryPath + "/STM32F4xx_StdPeriph_Driver/inc",
        libraryPath + "/CMSIS/Device/ST/STM32F4xx/Include",
        libraryPath + "/CMSIS/Include"
    ]

    files: {
        var projectFiles = [
                    "readme.txt",
                    "main.h",
                    "main.c",
                    "./Config/*",
                ];

        var librarySources = [
                    "stm32f4xx_rcc.c",
                    "stm32f4xx_gpio.c"
                ]



        for(var i=0; i<librarySources.length; i++){
            projectFiles.push( librarySourcesPath + librarySources[i]);
        }

        return projectFiles;
    }

    cpp.staticLibraries:
        [
        "gcc",
        "c",
        "m",
        "nosys",
    ]
}
