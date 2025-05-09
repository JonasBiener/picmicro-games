#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <conio.h>

#define MAX_X_FIELD 10
#define MAX_Y_FIELD 10
#define MAX_SNAKE_LENGTH 20

struct {
    unsigned char x_fields[MAX_SNAKE_LENGTH];
    unsigned char y_fields[MAX_SNAKE_LENGTH];
    unsigned char direction;
    unsigned char length;
} snake = {{[0]=MAX_X_FIELD/2, [1]=MAX_X_FIELD/2-1}, {[0]=MAX_Y_FIELD/2, [1]=MAX_Y_FIELD/2}, 0, 2};

struct {
    unsigned char x_field;
    unsigned char y_field;
} apple = {0, 0};

unsigned int score = 0;
char changed_direction = 0;
clock_t timer; // PLATFORM SPECIFIC

// PLATFORM SPECIFIC
void ClearScreen (void)
{
  const char *CLEAR_SCREEN_ANSI = "\e[2J";
  write(STDOUT_FILENO, CLEAR_SCREEN_ANSI, 5);
}

// PLATFORM SPECIFIC
char IsTimeElapsedMS (int compare) {
    return (clock() - timer) * 1000 / CLOCKS_PER_SEC >= compare;
}

// PLATFORM SPECIFIC
void ResetTimer (void) {
    timer = clock();
}

// PLATFORM SPECIFIC
int RandomInRange(int min, int max) {
    return min + rand() % (max - min + 1);
}

// PLATFORM SPECIFIC
void ResetRandomSeed (void) {
    srand(time(NULL));
}

// PLATFORM SPECIFIC
char InputAvailable (void) {
    return kbhit();
}

// PLATFORM SPECIFIC
char InputRead (void) {
    char key = getch();
    if (key == 'a') 
        return -1;
    else if (key == 'd')
        return 1;
    else 
        return 0;
}

// PLATFORM SPECIFIC
void PrintField (void) { 
    ClearScreen();
    for (unsigned char y = 0; y < MAX_Y_FIELD; y++) {
        for (unsigned char x = 0; x < MAX_X_FIELD; x++) {
            char field = '.';
            if (apple.x_field == x && apple.y_field == y) 
                field = 'Q';
            else if (snake.x_fields[0] == x && snake.y_fields[0] == y) 
                field = 'X';
            else 
                for (int i = 1; i < snake.length; i++) {
                    if (snake.x_fields[i] == x && snake.y_fields[i] == y) 
                        field = 'S';
                }
            printf("%c", field);
        }
        printf("\n");
    }
    printf("CSnake - Score: %i\n", score);
}

// PLATFORM SPECIFIC
void GameOver (char cause) { 
    PrintField();
    printf("GAME OVER! Cause: %i\n", cause); 
}

/*
    3
2  Dir   0
    1
*/

int TickSnake (void) {
    // Set new snake head
    unsigned char new_x_head = 0;
    unsigned char new_y_head = 0;
    switch (snake.direction) {
        case 0:
            new_x_head = snake.x_fields[0] + 1;
            new_y_head = snake.y_fields[0];
            break;
        case 1:
            new_x_head = snake.x_fields[0];
            new_y_head = snake.y_fields[0] + 1;
            break;
        case 2:
            new_x_head = snake.x_fields[0] - 1;
            new_y_head = snake.y_fields[0];
            break;
        case 3:
            new_x_head = snake.x_fields[0];
            new_y_head = snake.y_fields[0] - 1;
            break;
    }
    // Check if snake ate apple
    if (new_x_head == apple.x_field && new_y_head == apple.y_field) {
        apple.x_field = RandomInRange(0, MAX_X_FIELD - 1);
        apple.y_field = RandomInRange(0, MAX_Y_FIELD - 1);
        if (snake.length < MAX_SNAKE_LENGTH) 
            snake.length++;
        score++;
    }
    // Shift snake a step forward
    for (unsigned char i = snake.length - 1; i > 0; i--) {
        snake.x_fields[i] = snake.x_fields[i - 1];
        snake.y_fields[i] = snake.y_fields[i - 1];
    }
    snake.x_fields[0] = new_x_head;
    snake.y_fields[0] = new_y_head;
    // Check game boundary
    if (snake.x_fields[0] < 0) 
        return 1;
    if (snake.x_fields[0] >= MAX_X_FIELD) 
        return 2;
    if (snake.y_fields[0] < 0) 
        return 3;
    if (snake.y_fields[0] >= MAX_Y_FIELD) 
        return 4;
    // Check self-intersection
    for (unsigned char i = 1; i < snake.length; i++) {
        if (snake.x_fields[0] == snake.x_fields[i] && snake.y_fields[0] == snake.y_fields[i]) 
            return 5;
    }
    return 0;
}

void ChangeSnakeDirection(char change) {
    snake.direction = (snake.direction + change + 4) % 4;
}

int main() {
    // Init game and RNG
    ResetRandomSeed();
    apple.x_field = RandomInRange(0, MAX_X_FIELD - 1);
    apple.y_field = RandomInRange(0, MAX_X_FIELD - 1);
    PrintField();
    ResetTimer();
    // Run and tick game
    while (1) {
        // Process Inputs
        if (InputAvailable() && !changed_direction) {
            char input = InputRead();
            ChangeSnakeDirection(input);
            changed_direction = 1;
        }
        // Game Tick every 500 ms
        if (IsTimeElapsedMS(500)) {
            char game_state = TickSnake();
            if (game_state) {
                GameOver(game_state);
                return 0;
            }
            PrintField();
            ResetTimer();
            changed_direction = 0;
        }
    }
    return 0;
}