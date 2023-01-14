cmake . -B build-ios \
        -G Xcode \
        -DCMAKE_TOOLCHAIN_FILE=cmake/ios.toolchain.cmake \
        -DPLATFORM=OS64COMBINED
open -a /Applications/Xcode.app build-ios
