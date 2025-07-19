# WEEKEND_PROJECTS
#include <xc.h>
#pragma config WDTE = OFF

#define SWITCH_MASK 0x3F

// Function pointer type: Action is a pointer to function taking no arguments, returning void
typedef void (*Action)(void);

typedef struct {
    unsigned char in;   // Button pattern (input)
    Action action;      // Function to call when button is pressed
} SwitchMap;

SwitchMap map[4];  // Using 4 buttons for demo

// Simple delay for blinking/debounce
void d() {
    volatile long int i;
    for(i=0; i<15000; i++);
}

// Actions: Each function is a different behavior

void toggle_LED1() {
    PORTD ^= 0x01;  // Toggle RD0
}

void blink_LED2() {
    PORTD |= 0x02;  // Turn ON RD1
    d();
    PORTD &= ~0x02; // Turn OFF RD1
}

void turn_ON_LED3() {
    PORTD |= 0x04;  // Turn ON RD2
}

void turn_OFF_LED3() {
    PORTD &= ~0x04; // Turn OFF RD2
}

// Initialize GPIO
void init_config() {
    TRISB = SWITCH_MASK; // Lower 6 bits as input
    TRISD = 0x00;        // PORTD as output
    PORTB = 0xFF;        // Enable pull-ups if needed
    PORTD = 0x00;        // All LEDs off initially
}

// Map each button to a function directly
void define(SwitchMap *m) {
    m[0].in = 0x3E; m[0].action = toggle_LED1;   // Button 1
    m[1].in = 0x3D; m[1].action = blink_LED2;    // Button 2
    m[2].in = 0x3B; m[2].action = turn_ON_LED3;  // Button 3
    m[3].in = 0x37; m[3].action = turn_OFF_LED3; // Button 4
}

void main(void) {
    init_config();
    define(map);

    unsigned char i;

    while(1) {
        for(i=0; i<4; i++) {
            if((PORTB & SWITCH_MASK) == map[i].in) {
                d(); // Debounce

                // Wait for button release (edge detection)
                while((PORTB & SWITCH_MASK) == map[i].in);

                // Call the function mapped to this button
                map[i].action();

                break; // Prevent multiple matches in one loop
            }
        }
    }
}
