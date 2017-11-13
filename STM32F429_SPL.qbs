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

    property string floatAbi: "soft"
    property string cpuName: "cortex-m4"
    property string fpuName: "fpv4-sp-d16"

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

    cpp.driverFlags: {
        var arr = [
                    "-mcpu=" + cpuName,
                    "-mfloat-abi=" + floatAbi,
                    "-mthumb",
                    "-mabi=aapcs",
                    "-mno-sched-prolog",
                    "-mabort-on-noreturn",
                    "-fdata-sections",
                    "-ffunction-sections",
                    "-fno-strict-aliasing",
                    "-fno-builtin",
                    "-specs=nosys.specs",
                    "-specs=nano.specs",
                    "-static",
                    "-nodefaultlibs",
                    "-Wdouble-promotion",
                    "-ggdb",
                    "-g3",
                ];

        if (fpuName && typeof(fpuName) === "string") {
            arr.push("-mfpu=" + fpuName);
        }

        return arr;
    }

    cpp.cxxFlags: [
    ]

    cpp.linkerFlags: [
        "--gc-sections",
    ]


    cpp.defines: [
        "STM32F429_439xx",
        "USE_FULL_ASSERT",
        "USE_STDPERIPH_DRIVER"
    ]

    cpp.includePaths:
        [
        ".",
        "./config",
        "./Libraries/STM32F4xx_StdPeriph_Driver/inc",
        "./Libraries/CMSIS/DEVICE/ST/STM32F4xx/include",
        "./Libraries/CMSIS/Include"
        ]

    files: [
        "readme.txt",
        "main.h",
        "main.c",
        "./config/*",
        "./Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_rcc.c",
        "./Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_gpio.c"
    ]
}
