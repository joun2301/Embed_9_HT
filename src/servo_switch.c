#include <stdio.h>
#include <wiringPi.h>
#include <stdlib.h>

#define PWM0 18
#define UP 17
#define DOWN 27

void rotate_Servo(float angle) {
    int value = 30 + (int)(angle / 90 * 20);
    printf("%d\n", value);
    pwmWrite(PWM0, value);
}

int main() {
	wiringPiSetupGpio();
	
	pinMode(DOWN, INPUT);
	pinMode(UP, INPUT);

    pinMode(PWM0, PWM_OUTPUT);
    pwmSetMode(PWM_MODE_MS);
    pwmSetRange(400);
    pwmSetClock(960);
    
    float angle = 15;
	int buttonPressed = 0;
	
	rotate_Servo(angle);
	
	while(1){
		int buttonPressed = 0;
		if(digitalRead(UP) == HIGH && digitalRead(DOWN) == HIGH) {
			while(angle > 15){
				angle -= 0.5;
				rotate_Servo(angle);
				delay(50);
			}

		}
		else if (digitalRead(UP) == HIGH){
			angle += 0.5;
			if (angle >= 70) {
				angle = 70;
			}
			rotate_Servo(angle);
			buttonPressed = 1;
			delay(50);
		}
		else if (digitalRead(DOWN) == HIGH) {
			angle -= 0.5;
			if(angle <= 15){
				angle = 15;
			}
			buttonPressed = 1;
			rotate_Servo(angle);
			delay(50);
		}
		

	}
	
	
    return 0;
}
