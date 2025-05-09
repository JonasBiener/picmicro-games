/*
 * File:   main.c
 * Author: Jonas Biener
 * Date: 2025-05-05
 */

#include <xc.h>
#include <LCD/GLCD_library.h>

#include "game_engine.h"
#include "snake_game.h"

int main (void) {
    
    InitEngine();
    BeginRandomEntropyCollection();
    GLCD_Text2Out(0,0,"PIC-Snake");
    GLCD_Text2Out(1,0,"V - 1.0");
    GLCD_Text2Out(2,0,"J.Biener");
    while (PORTBbits.RB1 || !IsTimeElapsedMS(100)) Nop();
    ResetRandomEntropy();
    InitSnake();
    PrintSnake();
    ResetTimer();
    
    while (1) {
        
        if (InputAvailable()) {
            char input = InputRead();
            if (input == 'L') {
                snake_direction_change = -1;
            }
            else if (input == 'R') {
                snake_direction_change = 1;
            }
        }
        
        if (IsTimeElapsedMS(250)) {
            char tick_response = TickSnake();
            if (tick_response) {
                GLCD_Clear();
                while ((InputRead() != 'M')) {
                    if (IsTimeElapsedMS(500)) {
                        GLCD_Text2Out(0,3,"GAME ");
                        GLCD_Text2Out(1,3,"OVER");
                        GLCD_Value2Out_00(2, 3, (short)snake_score, 4);
                        ResetTimer();
                        LATB ^= 0b00111100;
                    }
                }
                return 0;
            }
            PrintSnake();
            LATB = ~((unsigned char)(snake_score) << 2);
            ResetTimer();
        }
        
    }
    
}
