#!/bin/bash

# 프로세스 이름 배열 정의 (쉼표 제거)
process_names=("ultra_tv" "dht" "speaker_rotary" "jodo" "blue" "servo_switch")

# 각 프로세스 이름에 대해 실행
for process_name in "${process_names[@]}"
do
    # ps -ef 명령어를 사용하여 프로세스 목록에서 해당 프로세스의 PID 검색
    # grep -v grep을 사용하여 grep 명령 자체는 제외
    pids=$(ps -ef | grep "$process_name" | grep -v grep | awk '{print $2}')

    # PID가 존재하면 종료
    if [ -n "$pids" ]; then
        echo "Killing process: $process_name (PID: $pids)"
        kill -9 $pids
    else
        echo "No process found for: $process_name"
    fi
done

# 완료 메시지
echo "All specified processes have been handled."