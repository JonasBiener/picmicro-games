/*
 * File:   snake_game.h
 * Author: Jonas Biener
 * Date: 2025-05-05
 */

#ifndef SNAKE_GAME_H
#define	SNAKE_GAME_H

#ifdef	__cplusplus
extern "C" {
#endif

#define MAX_X_FIELD 21
#define MAX_Y_FIELD 12
#define MAX_SNAKE_LENGTH 20

// Stores game-state of the snake
typedef struct SnakeState {
    unsigned char x_fields[MAX_SNAKE_LENGTH];
    unsigned char y_fields[MAX_SNAKE_LENGTH];
    unsigned char direction;
    unsigned char length;
} SnakeState;
extern SnakeState *snake;

// Stores game-state of the apple
typedef struct AppleState {
    unsigned char x_field;
    unsigned char y_field;
} AppleState;
extern AppleState *apple;

// Current score of the game
extern unsigned int snake_score;

// Proposed directional change
extern char snake_direction_change;

// Initilializes the game
void InitSnake (void);

// Do a game tick
char TickSnake (void);

// Print current game state
void PrintSnake (void);

#ifdef	__cplusplus
}
#endif

#endif	/* SNAKE_GAME_H */

