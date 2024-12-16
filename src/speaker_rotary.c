#include <stdio.h>
#include <wiringPi.h>
#include <unistd.h>
#include <stdlib.h>  // For system()

#define CLK_PIN 7   // GPIO7 connected to the rotary encoder's CLK pin
#define DT_PIN 8    // GPIO8 connected to the rotary encoder's DT pin
#define SW_PIN 1   // GPIO25 connected to the rotary encoder's SW pin

#define DIRECTION_CW 0
#define DIRECTION_CCW 1

#define MIN_VOLUME 0  // Minimum volume (mute)
#define MAX_VOLUME 100 // Maximum volume

int volume = 20;  // Initial volume set to 20
int prev_volume = 20;  // Previous volume before mute
int direction = DIRECTION_CW;
int CLK_state = 0;
int prev_CLK_state = 0;
int button_pressed = 0;
int prev_button_state = HIGH;

// NEW: Variables for mute/unmute button
int mute_button_pressed = 0;
int prev_mute_button_state = LOW;  // Since pull-down is used

// NEW: Mute flag
int is_muted = 0;

// Function to execute system commands
void execute_command(const char *cmd) {
    printf("Executing: %s\n", cmd);  // For debugging
    int ret = system(cmd);
    if (ret == -1) {
        perror("Error executing command");
    }
}

// Function to set the system volume
void set_volume(int vol) {
    if (vol == 0) {
        // Mute the system volume using amixer
        execute_command("amixer set Master mute");
    } else {
        // Set the system volume using amixer, setting the volume percentage
        char command2[50];
        snprintf(command2, sizeof(command2), "amixer -D pulse sset Master %d%%", vol);
        execute_command(command2);
        execute_command("amixer set Master unmute");
    }
}

 // Setup function for GPIO initialization
void setup() {
    wiringPiSetupGpio();  // Initialize wiringPi using BCM GPIO numbering

    pinMode(CLK_PIN, INPUT);
    pinMode(DT_PIN, INPUT);
    pinMode(SW_PIN, INPUT);
    pullUpDnControl(SW_PIN, PUD_UP); // Enable pull-up resistor for existing button pin
}

int main() {
    setup();

    prev_CLK_state = digitalRead(CLK_PIN);  // Read the initial state of CLK pin

    // Set initial volume
    set_volume(volume);
    execute_command("amixer set Master unmute");
   
    while (1) {
        // Read the current state of the rotary encoder's CLK pin
        CLK_state = digitalRead(CLK_PIN);

        // If the state of CLK is changed, then a pulse occurred
        // React to only the rising edge (from LOW to HIGH) to avoid double count
        if (CLK_state != prev_CLK_state && CLK_state == HIGH) {
            // If the DT state is HIGH, the encoder is rotating in counter-clockwise direction
            // Decrease the volume
            if (digitalRead(DT_PIN) == HIGH) {
                direction = DIRECTION_CCW;
            } else {
                // The encoder is rotating in clockwise direction => increase the volume
                direction = DIRECTION_CW;
            }

            // NEW: If currently muted, restore the previous volume before adjusting
            if (is_muted) {
                volume = prev_volume;
                is_muted = 0;
                set_volume(volume);
                printf("Unmuted: Restored volume to %d%%\n", volume);
            }

            // Adjust the volume based on rotation direction
            if (direction == DIRECTION_CW) {
                volume++;
            } else {
                volume--;
            }

            // Clamp the volume value between MIN_VOLUME and MAX_VOLUME
            if (volume < MIN_VOLUME) {
                volume = MIN_VOLUME;
            }
            if (volume > MAX_VOLUME) {
                volume = MAX_VOLUME;
            }

            // Update system volume using amixer
            set_volume(volume);

            // Update prev_volume if volume is not muted
            if (volume > 0) {
                prev_volume = volume;
            }

            printf("Rotary Encoder:: direction: %s - volume: %d%%\n",
                (direction == DIRECTION_CW) ? "CLOCKWISE" : "ANTICLOCKWISE", volume);
        }

        // Save last CLK state
        prev_CLK_state = CLK_state;

        // Check for button press (rotary encoder's SW_PIN)
        int button_state = digitalRead(SW_PIN);
        if (button_state != prev_button_state) {
            usleep(10000);  // Add a small delay to debounce (10ms)
            if (button_state == LOW) {
                printf("The rotary encoder button is pressed\n");
                button_pressed = 1;

                if (!is_muted) {
                    // Mute the system
                    prev_volume = volume;  // Save current volume before muting
                    volume = 0;
                    set_volume(volume);
                    is_muted = 1;
                    
                } else {
                    // Unmute the system
                    volume = prev_volume;
                    set_volume(volume);
                    is_muted = 0;
                    
                }
            } else {
                button_pressed = 0;
            }
        }

        prev_button_state = button_state;

        // Add a small delay to prevent CPU overuse
        usleep(1000);  // 1ms delay
    }

    return 0;
}
