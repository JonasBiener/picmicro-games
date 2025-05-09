#define TE2 659
#define TC2 532
#define TG2 784
#define TG1 392
#define TE1 330
#define TA1 440
#define TH1 494
#define TB1 466
#define TA2 880
#define TF1 349
#define TF2 698
#define TD2 587

typedef struct Tone {
    unsigned int frequency;
    unsigned char eighths;
} Tone;

Tone mario_tones[24] = {
    {TE2, 1}, {TE2, 2}, {TE2, 2}, 
    {TC2, 1}, {TE2, 2}, {TG2, 4}, {TG1, 2},
    
    {TC2, 2}, {TG1, 2}, {TE1, 2},
    {TA1, 2}, {TH1, 2}, {TB1, 1}, {TA1, 2},
    {TG1, 2}, {TE2, 2}, {TG2, 2}, {TA2, 2},
    {TF2, 1}, {TG2, 2}, {TE2, 2}, {TC2, 1}, {TD2, 2}, {TH1, 2},
};

void FrequencyToPWM (unsigned int frequency) { //250 ns per Step
    T3CONbits.TMR3ON = frequency != 0;
    CCPR1 = (unsigned short)(4000000UL / frequency);
}

void PlayTone (void) {
    ResetTimer();
    for (int i = 0; i < 24; i++) {
        FrequencyToPWM(mario_tones[i].frequency);
        while (!IsTimeElapsedMS(357 / 4 * mario_tones[i].eighths));
        ResetTimer();
        FrequencyToPWM(0);
        while (!IsTimeElapsedMS(357 / 4 * mario_tones[i].eighths));
        ResetTimer(); 

        ledstate <<= 1;
        if (ledstate == 0b01000000) ledstate = 0b00000010;
        LATB = ~ledstate;
    }
}

/*
if (PIR1bits.CCP1IF) {
        PIR1bits.CCP1IF = 0;
        LATCbits.LC2 ^= 1;
        return;
    }
*/