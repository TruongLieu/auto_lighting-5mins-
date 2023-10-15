#include <pic12f675.h>

#define SENSOR_THRESHOLD 50 // Adjust this threshold based on your sensor characteristics
#define RELAY_PIN GP1

unsigned int timerCount = 0;

void interrupt() {
    if (T0IF_bit) {
        TMR0 = 131; // Load TMR0 value for a 1ms interrupt (assuming 4MHz oscillator)
        T0IF_bit = 0; // Clear Timer0 interrupt flag
        timerCount++; // Increment the timer count

        if (timerCount >= 300000) { // Approximately 5 minutes (300,000 ms)
            GPIO.B1 = 0; // Turn off the relay
            timerCount = 0; // Reset the timer count
        }
    }
}

void main() {
    OPTION_REG = 0b00000111; // Prescaler 1:256, TMR0 as counter
    TMR0 = 131; // Load TMR0 value for a 1ms interrupt (assuming 4MHz oscillator)
    T0IE_bit = 1; // Enable Timer0 interrupt

    INTCON = 0xC0; // Enable global and peripheral interrupts

    ADCON0 = 0b00000001; // Configure AN0 (GP1) as analog input
    CMCON = 0x07;        // Disable comparators
    TRISIO = 0b00000010; // Configure GP0 as output, GP1 as input

    GPIO.B1 = 0; // Set GP1 (RELAY_PIN) initially low

    while (1) {
        if (ADC_Read(1) > SENSOR_THRESHOLD) {
            GPIO.B1 = 1; // Turn on the relay
        }
    }
}
