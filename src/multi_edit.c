#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>
#include <spawn.h>
#include <signal.h>
#include <mqueue.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <wiringPi.h>
#include <wiringSerial.h>

#define BAUD_RATE 115200

#define BUFFER_SIZE 1024
static const char* MQ_NAME = "/posix_mq";

extern char** environ;
static const char* UART2_DEV = "/dev/ttyS0";

unsigned char serialRead(const int fd) {
    unsigned char x;
    if (read(fd, &x, 1) != 1)
        return -1;
    return x;
}

void serialWrite(const int fd, const unsigned char c) {
    write(fd, &c, 1);
}

void reap_zombie_processes() {
    pid_t pid;
    int status;
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        // printf("Reaped child process %d with status %d\n", pid, status);
    }
}

void processCommand(char *command, pid_t jodo, pid_t speaker, pid_t fan) {

}

int main() {
    pid_t ultrasonic;
    pid_t temperSensor;
    pid_t bluetooth;
    pid_t jodo;
    pid_t servo;
    pid_t speaker;

    // signal(SIGCHLD, reap_zombie_processes); // SIGCHLD 핸들러 등록
    char command[256];
    int status;

    char* argv1[] = { "./ultra_tv", NULL };
    char* argv2[] = { "./dht", "0", NULL };
    char* argv3[] = { "sudo", "./blue", NULL };
    char* argv4[] = { "./jodo", "0", NULL };
    char* argv5[] = { "sudo", "./servo_switch", NULL };
    char* argv6[] = { "./speaker_rotary", NULL };

    posix_spawn(&ultrasonic, "/home/pi/Desktop/Embedded/src/ultra_tv", NULL, NULL, argv1, environ);
    posix_spawn(&temperSensor, "/home/pi/Desktop/Embedded/src/dht", NULL, NULL, argv2, environ);
    posix_spawn(&jodo, "/home/pi/Desktop/Embedded/src/jodo", NULL, NULL, argv4, environ);
    posix_spawn(&servo, "/usr/bin/sudo", NULL, NULL, argv5, environ);
    posix_spawn(&speaker, "/home/pi/Desktop/Embedded/src/speaker_rotary", NULL, NULL, argv6, environ);

    // message queue variable init
    mqd_t mq;
    struct mq_attr attr;
    char buffer[BUFFER_SIZE];
    int n;
    int bQuit = 0;

    // 메시지 큐 속성 설정
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = BUFFER_SIZE;
    attr.mq_curmsgs = 0;

    // 메시지 큐 생성 및 열기
    mq = mq_open(MQ_NAME, O_CREAT | O_RDONLY, 0644, &attr);
    if (mq == -1) {
        perror("mq_open failed");
        return 1;
    }

    posix_spawn(&bluetooth, "/usr/bin/sudo", NULL, NULL, argv3, environ);

    while (!bQuit) {
        n = mq_receive(mq, buffer, sizeof(buffer), NULL); // 메시지 큐에서 데이터 수신
        if (n > 0) {
            buffer[n] = '\0'; // 문자열 종료
            // printf("Received from client: %s\n", buffer); // 데이터 출력
            char *device;
            char *action;
            char *parameterStr;
            int parameter = 0;

            // ':' 기준으로 분리
            device = strtok(buffer, ":");
            action = strtok(NULL, ";");
            parameterStr = strtok(NULL, ";");

            // 파라미터가 존재하면 정수로 변환
            if (parameterStr != NULL) {
                parameter = atoi(parameterStr);
            }

            // 장치별 명령 처리
            if (strcmp(device, "SPEAKER") == 0) {
                if (strcmp(action, "ON") == 0) {
                    sprintf(command, "amixer set Master unmute");
                    int ret = system(command);
                    // printf("스피커 켜셈\n");
                } else if (strcmp(action, "OFF") == 0) {
                    sprintf(command, "amixer set Master mute");
                    int ret = system(command);
                    // printf("스피커 끄셈\n");
                } else if (strcmp(action, "SET_VOLUME") == 0) {
                    sprintf(command, "amixer set Master unmute;amixer set Master %d%%", parameter);
                    int ret = system(command);
                    // printf("스피커 볼륨 고정 ㄱㄱ\n");
                } else {
                    // printf("Invalid SPEAKER command\n");
                }
            } else if (strcmp(device, "FAN") == 0) {
                if (strcmp(action, "ON") == 0) {
                    // printf("PID : %d\n", jodo);
                    sprintf(command, "kill -9 %d", temperSensor);
                    // printf("Command : %s\n", command);
                    int ret = system(command);
                    // printf("Jodo kill\n");
                    char* argv[] = { "./dht", "1", NULL };
                    posix_spawn(&jodo, "/home/pi/Desktop/Embedded/src/dht", NULL, NULL, argv, environ);
                }
                else if (strcmp(action, "OFF") == 0) {
                    // printf("LED 끄셈 ㄱㄱ\n");
                    sprintf(command, "kill -9 %d", temperSensor);
                    int ret = system(command);
                    // printf("Jodo kill\n");
                    char* argv[] = { "./dht", "2", NULL };
                    posix_spawn(&jodo, "/home/pi/Desktop/Embedded/src/dht", NULL, NULL, argv, environ);
                }
                else if (strcmp(action, "AUTO") == 0) {
                    // printf("자동 ㄱㄱ\n");
                    sprintf(command, "kill -9 %d", temperSensor);
                    int ret = system(command);
                    // printf("Jodo kill\n");
                    char* argv[] = { "./dht", "0", NULL };
                    posix_spawn(&jodo, "/home/pi/Desktop/Embedded/src/dht", NULL, NULL, argv, environ);
                }
                else {
                    // printf("Invalid FAN command\n");
                }
            } else if (strcmp(device, "LED") == 0) {
                if (strcmp(action, "ON") == 0) {
                    // printf("PID : %d\n", jodo);
                    sprintf(command, "kill -9 %d", jodo);
                    printf("Command : %s\n", command);
                    int ret = system(command);
                    // printf("Jodo kill\n");
                    char* argv[] = { "./jodo", "1", NULL };
                    posix_spawn(&jodo, "/home/pi/Desktop/Embedded/src/jodo", NULL, NULL, argv, environ);
                }
                else if (strcmp(action, "OFF") == 0) {
                    // printf("LED 끄셈 ㄱㄱ\n");
                    sprintf(command, "kill -9 %d", jodo);
                    int ret = system(command);
                    // printf("Jodo kill\n");
                    char* argv[] = { "./jodo", "2", NULL };
                    posix_spawn(&jodo, "/home/pi/Desktop/Embedded/src/jodo", NULL, NULL, argv, environ);
                }
                else if (strcmp(action, "AUTO") == 0) {
                    // printf("자동 ㄱㄱ\n");
                    sprintf(command, "kill -9 %d", jodo);
                    int ret = system(command);
                    // printf("Jodo kill\n");
                    char* argv[] = { "./jodo", "0", NULL };
                    posix_spawn(&jodo, "/home/pi/Desktop/Embedded/src/jodo", NULL, NULL, argv, environ);
                }
                else {
                    // printf("Invalid LED command\n");
                }
            } else {
                // printf("Unknown device: %s\n", device);
            }
            if (buffer[0] == 'q') { // 종료 조건
                bQuit = 1;
            }
        }
    }

    mq_close(mq);
    mq_unlink(MQ_NAME); // 메시지 큐 삭제
    return 0;

    return 0;
}



    // // Bluetooth Init
    // int fd_serial;
    // char dat;
    
    // if (wiringPiSetupGpio() < 0) return 1;
    
    // if ((fd_serial = serialOpen(UART2_DEV, BAUD_RATE)) < 0) {
    //     printf("Unable to open serial device.\n");
    //     return 1;
    // }
    // // Bluetooth Init End

    // while(1) {
    //     if(serialDataAvail(fd_serial)) {
    //         dat = serialRead(fd_serial);
    //         printf("%c", dat);
    //         fflush(stdout);
    //         // serialWrite(fd_serial, dat);
    //     }
    //     delay(10);

    //     char buffer[2] = {dat, '\0'};
    //     if (strcmp(buffer, "1") == 0) {
    //         char command[256];
    //         printf("ready\n");
    //         snprintf(command, sizeof(command), "kill -9 %d", temperSensor);
    //         int ret = system(command);
    //         printf("kill\n");
    //         char* argv2[] = {"./dht", "1", NULL};
    //         posix_spawn(&temperSensor, "/home/pi/Desktop/Embedded/dht", NULL, NULL, argv2, environ);
    //     }
    // }
