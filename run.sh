#!/bin/bash

# set environment variables
git clone --depth=1 https://github.com/picasso09/clang-9.0.3-r353983c1 clang
export KBUILD_BUILD_HOST="picasso09"
export KBUILD_BUILD_USER="picasso"
export PATH="$(pwd)/clang/bin:$PATH"

# build kernel
make -j$(nproc --all) O=out ARCH=arm64 oppo6765_defconfig
make -j1 ARCH=arm64 O=out \
                  CC=clang \
                  CROSS_COMPILE=aarch64-linux-gnu- \
                  CROSS_COMPILE_ARM32=arm-linux-gnueabi-

# Check if build
if [ -f "out/arch/arm64/boot/Image.gz-dtb" ]; then
    # move to output
    cd out/arch/arm64/boot/

    # clone AnyKernel
    git clone --depth=1 https://github.com/picasso09/AnyKernel3 -b UI1
    # Copy image.gz to AnyKernel repository
    cp Image.gz-dtb AnyKernel3/

    # move to anykernel repo
    cd AnyKernel3

    # run make command to create zip
    make

    # find the latest zip file created by Makefile
    LATEST_ZIP=$(ls -t *.zip | head -n1)

    # check if zip file created
    if [ -f "$LATEST_ZIP" ]; then
        # Send file to Telegram bot
        BOT_TOKEN="null"
        CHAT_ID="null"
        FILE_PATH="$LATEST_ZIP"

        # Send message to Telegram
        MESSAGE="kernel build success,and send kernel"
        curl -s -X POST https://api.telegram.org/bot$BOT_TOKEN/sendMessage -d text="$MESSAGE" -d chat_id=$CHAT_ID > /dev/null

        # Send file to Telegram
        curl -F chat_id="$CHAT_ID" -F document=@"$FILE_PATH" https://api.telegram.org/bot$BOT_TOKEN/sendDocument
    else
        # Send error message to Telegram
        ERROR_MESSAGE="Error: failed to send zip file, file not found."
        curl -s -X POST https://api.telegram.org/bot$BOT_TOKEN/sendMessage -d text="$ERROR_MESSAGE" -d chat_id=$CHAT_ID > /dev/null
    fi
else
    # Send error message to Telegram
    ERROR_MESSAGE="Error: failed to build."
    curl -s -X POST https://api.telegram.org/bot$BOT_TOKEN/sendMessage -d text="$ERROR_MESSAGE" -d chat_id=$CHAT_ID > /dev/null
fi
