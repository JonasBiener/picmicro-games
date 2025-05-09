/*
 * File:   snake_game.c
 * Author: Jonas Biener
 * Date: 2025-05-05
 */

#include <LCD/GLCD_library.h>

#include "snake_game.h"
#include "game_engine.h"

unsigned int snake_score = 0;
char snake_direction_change = 0;
SnakeState *snake;
AppleState *apple;

void InitSnake (void) {
    apple = (AppleState*) calloc(1, sizeof(*apple));
    apple->x_field = RandomInRange(1, MAX_X_FIELD - 2);
    apple->y_field = RandomInRange(1, MAX_Y_FIELD - 2);
    snake = (SnakeState*) calloc(1, sizeof(*snake));
    snake->x_fields[0] = MAX_X_FIELD/2;
    snake->y_fields[0] = MAX_Y_FIELD/2;
    snake->x_fields[1] = MAX_X_FIELD/2 - 1;
    snake->y_fields[1] = MAX_Y_FIELD/2;
    snake->direction = 0;
    snake->length = 2;
}

char TickSnake (void) {
    // Change snake direction
    snake->direction = (snake->direction + snake_direction_change + 4) % 4;
    snake_direction_change = 0;
    // Set new snake head
    unsigned char new_x_head = 0;
    unsigned char new_y_head = 0;
    switch (snake->direction) {
        case 0:
            new_x_head = snake->x_fields[0] + 1;
            new_y_head = snake->y_fields[0];
            break;
        case 1:
            new_x_head = snake->x_fields[0];
            new_y_head = snake->y_fields[0] + 1;
            break;
        case 2:
            new_x_head = snake->x_fields[0] - 1;
            new_y_head = snake->y_fields[0];
            break;
        case 3:
            new_x_head = snake->x_fields[0];
            new_y_head = snake->y_fields[0] - 1;
            break;
    }
    // Check if snake ate apple
    if (new_x_head == apple->x_field && new_y_head == apple->y_field) {
        for (unsigned char i = 0; i < 10; i++) {
            unsigned char intersect = 0;
            apple->x_field = RandomInRange(1, MAX_X_FIELD - 2);
            apple->y_field = RandomInRange(1, MAX_Y_FIELD - 2);
            for (unsigned char j = 0; i < snake->length; i++) {
                intersect += snake->x_fields[j] == apple->x_field;
                intersect += snake->y_fields[j] == apple->y_field;
            } 
            if (!intersect) 
                break;
        }
        if (snake->length < MAX_SNAKE_LENGTH) 
            snake->length++;
        snake_score++;
    }
    // Shift snake a step forward
    for (unsigned char i = snake->length - 1; i > 0; i--) {
        snake->x_fields[i] = snake->x_fields[i - 1];
        snake->y_fields[i] = snake->y_fields[i - 1];
    }
    snake->x_fields[0] = new_x_head;
    snake->y_fields[0] = new_y_head;
    // Check game boundary
    if (snake->x_fields[0] < 1) 
        return 1;
    if (snake->x_fields[0] >= MAX_X_FIELD - 1) 
        return 2;
    if (snake->y_fields[0] < 1) 
        return 3;
    if (snake->y_fields[0] >= MAX_Y_FIELD - 1) 
        return 4;
    // Check self-intersection
    for (unsigned char i = 1; i < snake->length; i++) {
        if (snake->x_fields[0] == snake->x_fields[i] && snake->y_fields[0] == snake->y_fields[i]) 
            return 5;
    }
    return 0;
}

void PrintSnake (void) {
    // GLCD_Clear();
    for (char yd = 0; yd < MAX_Y_FIELD / 2; yd++) {
        GLCD_SetCursor(yd, 0);
        for (char xd = 0; xd < MAX_X_FIELD * 4; xd++) {
            unsigned char x_coord = xd / 4;
            unsigned char y_coord_1 = yd * 2;
            unsigned char y_coord_2 = yd * 2 + 1;
            unsigned char output = 0x00; 
            if (x_coord == snake->x_fields[0]) {
                unsigned char head_bits = 0x00;
                if (xd % 4 == 0 || xd % 4 == 3) 
                    head_bits = 0b1111;
                else if (xd % 4 == 1 || xd % 4 == 2) 
                    head_bits = 0b1001;
                if (y_coord_1 == snake->y_fields[0])
                    output |= head_bits;
                else if (y_coord_2 == snake->y_fields[0])
                    output |= head_bits << 4;
            }
            for (char i = 1; i < snake->length; i++) {
                if (x_coord != snake->x_fields[i]) 
                    continue;
                if (y_coord_1 == snake->y_fields[i]) 
                    output |= 0x0F;
                else if (y_coord_2 == snake->y_fields[i]) 
                    output |= 0xF0;
            }
            if (x_coord == apple->x_field){
                unsigned char apple_bits = 0x00;
                if (xd % 4 == 0 || xd % 4 == 3) 
                    apple_bits = 0b0110;
                else if (xd % 4 == 1 || xd % 4 == 2) 
                    apple_bits = 0b1111;
                if (y_coord_2 == apple->y_field) 
                    apple_bits <<= 4;
                else if (y_coord_1 != apple->y_field) 
                    apple_bits = 0x00;
                output ^= apple_bits;
            }
            if (xd <= 2 || xd >= MAX_X_FIELD * 4 - 3) 
                output = 0xFF;
            if (yd == 0) 
                output |= 0x07;
            else if (yd == MAX_Y_FIELD / 2 - 1) 
                output |= 0xE0;
            GLCD_Write(output, DTA);
        }
        GLCD_Write(0x00, DTA);
    }
}
