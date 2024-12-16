#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>

#define SLAVE_ADDR_01 0x48
#define LED 13

static const char* I2C_DEV = "/dev/i2c-1";

int main(int argc, char* argv[]){
    //printf("jododjodjodjodjodjodjodjodjodjodjo");
    int i2c_fd;
    int curVal = 0;
    int preVal = 0;
    int adcChannel = 2;
    int prevRange = -1; // 이전 값의 범위를 저장하는 변수

    // wiringPi 초기화 (GPIO 사용)
    if (wiringPiSetupGpio() < 0) {
        printf("wiringPiSetup() is failed\n");
        return 1;
    }

    if ((i2c_fd = wiringPiI2CSetupInterface(I2C_DEV, SLAVE_ADDR_01)) < 0) {
        printf("wiringPi2CSetup Failed: \n");
        return 1;
    }

    pinMode(LED, OUTPUT);
    // pinMode(LED, PWM_OUTPUT);
    // pwmSetMode(PWM_MODE_MS);
    // pwmSetRange(100);
    // pwmSetClock(192);

    if(strcmp(argv[1], "0") == 0){
        printf("Auto Mode On\n");
    }
    else if(strcmp(argv[1], "1") == 0){
        printf("Manual : ON");
        digitalWrite(LED, HIGH);
        return 0;
    }
    else if(strcmp(argv[1], "2") == 0){
        digitalWrite(LED, LOW);
        printf("Manual : OFF");
        return 0;
    }
    else
        printf("Auto : 0\nAlways On : 1\nAlways Off : 2\n");

    while (1) {
        wiringPiI2CWrite(i2c_fd, 0x40 | adcChannel); // 채널 설정
        preVal = wiringPiI2CRead(i2c_fd); // 이전 값
        curVal = wiringPiI2CRead(i2c_fd); // 현재 값

        printf("Current value = %d\n", curVal);

        if (curVal <= 70) digitalWrite(LED, LOW);
        else digitalWrite(LED, HIGH);
        // int currentRange = 0;
        // if (curVal <= 30) currentRange = 0;
        // else if (curVal <= 60) currentRange = 1;
        // else if (curVal <= 90) currentRange = 2;
        // else currentRange = 3;

        // // 이전 범위와 현재 범위가 다를 경우에만 함수 호출
        // if (currentRange != prevRange) {
        //     if (currentRange == 0){
        //         pwmWrite(LED, 0);
        //         // wiringPiI2CWriteReg8(i2c_fd, 0x40, 0); // 0 ~ 35
        //         printf("00000\n");
        //     }
        //     else if (currentRange == 1){
        //         pwmWrite(LED, 33);
        //         // wiringPiI2CWriteReg8(i2c_fd, 0x40, curVal * 3); // 36 ~ 70
        //         printf("11111\n");
        //     }
        //     else if (currentRange == 2) {
        //         pwmWrite(LED, 66);
        //         // wiringPiI2CWriteReg8(i2c_fd, 0x40, curVal * 3); // 71 ~ 105
        //         printf("22222\n");
        //     }
        //     else if (currentRange == 3) {
        //         pwmWrite(LED, 99);
        //         printf("33333\n");
        //     }
        //     prevRange = currentRange; // 이전 범위를 업데이트
        // }

        delay(500);  // 500ms 대기
    }

    return 0;
}
