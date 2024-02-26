#!/usr/bin/env node

import { execSync } from "child_process";
import { program } from "commander";
import { argv } from "process";

enum SeenKitPlatform {
  kAndroid = "android",
  kIOS = "ios",
  kMacOS = "macos",
  kLinux = "linux",
  kWeb = "web",
  kWindows = "windows",
}
const PLATFORMS = Object.values(SeenKitPlatform).join("/");

function isSeenKitPlatform(platform: unknown): platform is SeenKitPlatform {
  if (typeof platform !== "string") {
    return false;
  }
  const values = Object.values(SeenKitPlatform) as string[];
  return values.includes(platform);
}

const PROJECT_COMMANDS = {
  [SeenKitPlatform.kAndroid]: "",
  [SeenKitPlatform.kIOS]: `cmake . -B build-ios \
                                   -G Xcode \
                                   -DCMAKE_TOOLCHAIN_FILE=cmake/ios.toolchain.cmake \
                                   -DPLATFORM=OS64COMBINED
                           open -a /Applications/Xcode.app build-ios`,
  [SeenKitPlatform.kMacOS]: `cmake . -B build-macos -G Xcode
                             open -a /Applications/Xcode.app build-macos`,
  [SeenKitPlatform.kLinux]: "",
  [SeenKitPlatform.kWeb]: "",
  [SeenKitPlatform.kWindows]: "",
};

const BUILD_COMMANDS = {
  [SeenKitPlatform.kAndroid]: "",
  [SeenKitPlatform.kIOS]: `cmake . -B build-ios \
                                   -G Xcode \
                                   -DCMAKE_TOOLCHAIN_FILE=cmake/ios.toolchain.cmake \
                                   -DPLATFORM=OS64COMBINED
                           cmake --build ./build-ios --target seen --config Release`,
  [SeenKitPlatform.kMacOS]: `cmake . -B build-macos -G Xcode
                             cmake --build build-macos --target seen --config Release`,
  [SeenKitPlatform.kLinux]: "",
  [SeenKitPlatform.kWeb]: "",
  [SeenKitPlatform.kWindows]: "",
};

program //
  .name("seen")
  .description("An all-in-one CLI program for SeenKit.")
  .version("1.0.0");

program
  .command("build")
  .description("build SeenKit to platform-specific library")
  .argument("<platform>", `specify SeenKit target platform (${PLATFORMS})`)
  .option("--project", "try to generate a SeenKit library project if possible", false)
  .action((platform?: SeenKitPlatform, option?: { project: boolean }) => {
    if (!isSeenKitPlatform(platform)) {
      return;
    }
    const command = option?.project ? PROJECT_COMMANDS[platform] : BUILD_COMMANDS[platform];
    execSync(command, { stdio: "inherit" });
  });

program.parse(argv);
