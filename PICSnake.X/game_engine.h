/*
 * File:   game_engine.h
 * Author: Jonas Biener
 * Date: 2025-05-05
 */

#ifndef GAME_ENGINE_H
#define	GAME_ENGINE_H

#ifdef	__cplusplus
extern "C" {
#endif
    
// Initializes the game-engine
char InitEngine (void);

// Returns if the game-tick timer is elapsed
char IsTimeElapsedMS (int compare);

// Resets the game-tick timer
void ResetTimer (void);

// Retrieves random-value from PNR
int RandomInRange (int min, int max);

// Starts the random-seed-timer
void BeginRandomEntropyCollection (void);

// Resets the PNR-seed
void ResetRandomEntropy (void);

// Get if user-input is available in buffer
char InputAvailable (void);

// Read input from input-buffer
char InputRead (void);

#ifdef	__cplusplus
}
#endif

#endif	/* GAME_ENGINE_H */

