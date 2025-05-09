/*
 * File:   game_engine.c
 * Author: Jonas Biener
 * Date: 2025-05-05
 */

#include <xc.h>
#include <LCD/GLCD_library.h>
#include "mcc_generated_files/nvm/nvm.h"

#include "game_engine.h"

unsigned long milliseconds = 0;
static unsigned long tick_timer = 0;
static char trig_encoder_cw = 0;
static char trig_encoder_ccw = 0;
static char trig_encoder_btn = 0;
static uint32_t random_number = 0;

char InitEngine (void) {
    // Initialize internal oscillator
    OSCCONbits.IRCF = 0b111; // Fosc =  16 MHz
    
    // Initialize LCD
    GLCD_Init();
    
    // Initialize LEDs
    ANSELB = 0b00000000;
    TRISB = 0b11000011;
    LATB = 0b00111100;
    
    // Initialize Timer1 and CCP5 for millisecond timer
    T1CONbits.T1CKPS = 0b11; // PS = 8
    T1CONbits.T1SYNC = 1;
    TMR1 = 0;
    CCP5CONbits.CCP5M = 0b1011; // CCP5 special event trigger mode
    CCPTMRS1bits.C5TSEL = 0; // Use Timer1 for CCP5 comaparision
    CCPR5 = 500; // Set CCP5 comparison value 125 = 1 ms
    PIE4bits.CCP5IE = 1; // Enable CCP5 interrupt
    PIR4bits.CCP5IF = 0; // Reset CCP5 interrupt flag
    
    // Initialize Timer3 and CCP1 for audio-generatrion timer
    T3CONbits.T3CKPS = 0; // No prescaler
    T3CONbits.nT3SYNC = 1;
    TMR3 = 0;
    CCPTMRS0bits.C1TSEL = 0b01; // User Timer3 for CCP1
    CCP1CONbits.CCP1M = 0b1011; // Special event trigger
    CCPR1 = 1000; // Set CCP5 initial value
    // PIE1bits.CCP1IE = 1; // Enable CCP1 interrupt
    PIR1bits.CCP1IF = 0; // Clear CCP1 interrupt flag
    
    // Initialize External Interrupts
    ANSELBbits.ANSB0 = 0;
    ANSELAbits.ANSA2 = 0;
    TRISBbits.RB0 = 1;
    TRISBbits.RB1 = 1;
    TRISAbits.RA2 = 1;
    INTCONbits.INT0IE = 1; // Enable INT0 external interrupt
    INTCONbits.INT0IF = 1; // Resset INT0 interrupt flag
    INTCON2bits.INTEDG0 = 0; // Trigger on falling edge
    INTCON3bits.INT1E = 1; // Enable INT0 external interrupt
    INTCON3bits.INT1F = 1; // Resset INT0 interrupt flag
    INTCON2bits.INTEDG1 = 0; // Trigger on falling edge
    
    // Start Timers and Interrupts
    T1CONbits.TMR1ON = 1; // Enable Timer1
    // T3CONbits.TMR3ON = 1; // Enable Timer3
    INTCONbits.GIE = 1; // Global interrupt enable
    INTCONbits.PEIE = 1; // Peripheral interrupt enable
}

char IsTimeElapsedMS (int compare) {
    if (milliseconds < compare) return 0;
    return milliseconds - compare >= tick_timer;
}

void ResetTimer (void) {
    tick_timer = milliseconds;
}

int RandomInRange (int min, int max) {
    random_number = (random_number * 1103515245UL) + 12345UL;
    return min + random_number % (max - min + 1);
}

void ResetRandomSeed (void) {
    uint16_t eeprom_value = 0;
    eeprom_value += EEPROM_Read(0x00);
    eeprom_value += EEPROM_Read(0x01) << 8;
    random_number = eeprom_value++;
    EEPROM_Write(0x00, (char)(eeprom_value & 0x00FF));
    EEPROM_Write(0x01, (char)(eeprom_value >> 8));
    random_number = 1;
}

char InputAvailable (void) {
    return trig_encoder_cw || trig_encoder_ccw || trig_encoder_btn;
}

char InputRead (void) {
    if (trig_encoder_cw) {
        trig_encoder_cw = 0;
        return 'R';
    }
    if (trig_encoder_ccw) {
        trig_encoder_ccw = 0;
        return 'L';
    }
    if (trig_encoder_btn) {
        trig_encoder_btn = 0;
        return 'M';
    }
    return 0;
}

void __interrupt(high_priority) __isr(void){
    if (PIR4bits.CCP5IF) {
        PIR4bits.CCP5IF = 0;
        milliseconds += 1;
        return;
    }
    if (PIR1bits.CCP1IF) {
        PIR1bits.CCP1IF = 0;
        LATCbits.LC2 ^= 1;
        return;
    }
    if (INTCONbits.INT0IF) {
        INTCONbits.INT0IF = 0; 
        trig_encoder_ccw = !PORTAbits.RA2;
        trig_encoder_cw = PORTAbits.RA2;
        return;
    }
    if (INTCON3bits.INT1F) {
        INTCON3bits.INT1F = 0; 
        trig_encoder_btn = 1;
        return;
    }
    while(1) Nop(); // Unhandled interrupts
}
