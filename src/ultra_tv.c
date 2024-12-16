#include <stdio.h>
#include <wiringPi.h>
#include <stdlib.h>
#include <unistd.h>

#define trigPin 21          // GPIO 21
#define echoPin 20
#define SWITCH 23
#define TIMEOUT 30000       // 초음파 타임아웃 (마이크로초 단위)
#define MAX_DISTANCE 50     // 무시할 최대 거리 (cm)
#define OVER_MAX_THRESHOLD 2 // TV를 끄기 위한 초과 거리 발생 횟수

int main(void) {
    int flag = 0;
    int tvpower = 0;
    int distance = 0;
    int overMaxCount = 0; // 초과 거리 카운터

    unsigned long startTime, travelTime;

    // 스위치 디바운싱을 위한 이전 상태 저장 변수
    int prevSwitchState = HIGH;

    if (wiringPiSetupGpio() == -1) {
        perror("GPIO 초기화 실패");
        return 1;
    }

    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    pinMode(SWITCH, INPUT); // 스위치를 입력 모드로 설정
    pullUpDnControl(SWITCH, PUD_UP); // 내부 풀업 저항 활성화

    // TV 초기 상태를 끈 상태로 설정
    system("wlr-randr --output DSI-1 --off");

    for (;;) {
        // 스위치 상태 읽기
        int switchState = digitalRead(SWITCH);

        // 스위치가 눌렸는지(LOW로 변했는지) 확인
        if (switchState == LOW && prevSwitchState == HIGH) {
            // TV 상태 토글
            tvpower = !tvpower;
            if (tvpower) {
                system("wlr-randr --output DSI-1 --on");
                printf("TV 켜짐 (스위치)\n");
            } else {
                system("wlr-randr --output DSI-1 --off");
                printf("TV 꺼짐 (스위치)\n");
            }
            // 디바운싱을 위해 잠시 대기
            // usleep(200000); // 200ms
        }
        delay(500);
        // 이전 스위치 상태 업데이트
        prevSwitchState = switchState;

        // 초음파 센서 측정 시작
        digitalWrite(trigPin, LOW);
        usleep(2);
        digitalWrite(trigPin, HIGH);
        usleep(20);
        digitalWrite(trigPin, LOW);

        unsigned long waitStart = micros();
        // Echo 핀이 HIGH가 될 때까지 대기
        while (digitalRead(echoPin) == LOW) {
            if (micros() - waitStart > TIMEOUT) {
                printf("Echo LOW 타임아웃\n");
                break;
            }
        }

        startTime = micros();
        waitStart = micros();
        // Echo 핀이 LOW로 돌아올 때까지 대기
        while (digitalRead(echoPin) == HIGH) {
            if (micros() - waitStart > TIMEOUT) {
                printf("Echo HIGH 타임아웃\n");
                break;
            }
        }
        travelTime = micros() - startTime;
        distance = travelTime / 58;

        // 거리 값 검증: MAX_DISTANCE를 초과하면 무시하고 카운트 증가
        if (distance > MAX_DISTANCE || distance == 0) {
            overMaxCount++;
            printf("무시된 거리: %dcm (초과) [카운트: %d]\n", distance, overMaxCount);

            // 초과 거리 발생 횟수가 임계값을 넘으면 TV를 끔
            if (overMaxCount >= OVER_MAX_THRESHOLD) {
                if (tvpower) {
                    system("wlr-randr --output DSI-1 --off");
                    tvpower = 0;
                    printf("TV 꺼짐 (초과 거리 횟수)\n");
                }
                // 카운트 리셋
                overMaxCount = 0;
            }
            // 무시할 경우, 루프의 나머지 부분을 건너뜀
            continue;
        } else {
            // 정상적인 거리 측정이 되면 카운트 리셋
            if (overMaxCount > 0) {
                printf("정상 거리 측정: %dcm [카운트 리셋]\n", distance);
                overMaxCount = 0;
            }
        }

        // printf("Distance: %dcm\n", distance);
        // 거리에 따른 TV 제어
        if (distance > 10) {
            flag = 0; // 사람이 멀어졌다고 가정
            if (tvpower == 1) {
                system("wlr-randr --output DSI-1 --off");
                tvpower = 0;
                printf("TV 꺼짐 (거리)\n");
            }
            continue;
        } else {
            if (flag == 0) {
                flag = 1;
                system("wlr-randr --output DSI-1 --on");
                tvpower = 1;
                printf("TV 켜짐 (거리)\n");
            }
        }

        //delay(1000);
    }
}
