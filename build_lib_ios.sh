cmake . -B build-ios \
        -G Xcode \
        -DCMAKE_TOOLCHAIN_FILE=cmake/ios.toolchain.cmake \
        -DPLATFORM=OS64COMBINED
cmake --build ./build-ios --target seen --config Release
