#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <mqueue.h>
#include <wiringPi.h>
#include <wiringSerial.h>

#define BAUD_RATE 115200
#define BUFFER_SIZE 1024
static const char* UART2_DEV = "/dev/ttyS0";
static const char* MQ_NAME = "/posix_mq";

unsigned char serialRead(const int fd);

unsigned char serialRead(const int fd) {
    unsigned char x;
    if (read(fd, &x, 1) != 1) // read 함수를 통해 1바이트 읽음
        return -1;
    return x; // 읽어온 데이터 반환
}

int main() {
    mqd_t mq;
    int fd_serial;
    unsigned char buffer[BUFFER_SIZE]; // 블루투스 데이터 저장용 버퍼
    int index = 0;

    // 메시지 큐 열기
    mq = mq_open(MQ_NAME, O_WRONLY);
    if (mq == -1) {
        perror("mq_open failed");
        return 1;
    }

    // 블루투스 설정
    if (wiringPiSetupGpio() < 0) return 1;
    if ((fd_serial = serialOpen(UART2_DEV, BAUD_RATE)) < 0) {
        printf("Unable to open serial device.\n");
        return 1;
    }

    while (1) {
        if (serialDataAvail(fd_serial)) { // 블루투스에서 데이터가 들어왔을 때
            unsigned char dat = serialRead(fd_serial);

            if (dat == '\n' || dat == '\r') { // 한 줄의 데이터가 끝날 때
                buffer[index] = '\0'; // 문자열 종료
                // printf("Sending to server: %s\n", buffer);
                mq_send(mq, (const char*)buffer, strlen((char*)buffer), 0); // 메시지 큐로 전송
                index = 0; // 버퍼 초기화
            } else if (index < BUFFER_SIZE - 1) {
                buffer[index++] = dat; // 데이터를 버퍼에 저장
            } else {
                index = 0; // 버퍼 초과 방지
            }
        }
        delay(10);
    }

    mq_close(mq);
    return 0;
}
