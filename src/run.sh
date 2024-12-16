#!/bin/bash

# C 파일 리스트
FILES=("blue.c" "dht.c" "jodo.c" "servo_switch.c" "speaker_rotary.c" "ultra_tv.c" "multi_edit.c")

# 컴파일 옵션
COMPILER="gcc"
OPTIONS="-lwiringPi"

# 모든 파일 컴파일
for FILE in "${FILES[@]}"; do
    # 파일 이름에서 확장자 제거
    OUTPUT_FILE="${FILE%.c}"

    echo "Compiling $FILE..."
    $COMPILER $FILE -o $OUTPUT_FILE $OPTIONS

    if [ $? -eq 0 ]; then
        echo "$FILE compiled successfully to $OUTPUT_FILE."
    else
        echo "Error compiling $FILE. Exiting."
        exit 1
    fi
done

# multi_edit 실행
if [ -f "multi_edit" ]; then
    echo "Running multi_edit..."
    ./multi_edit
else
    echo "multi_edit executable not found."
    exit 1
fi
