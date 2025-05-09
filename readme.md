# PICmicro Game Collection

Jonas Biener, Technische Hochschule Ulm (2025)

# Snake

- The snake is controlled by the rotary encoder
- To start a new game, press the encoder button
- The apple is randomly placed at different locations on the board
- Entropy for the PRNG is collected by user input delay
- Your score is being displayed in binary by the LEDs or on the Game-Over-Display
- The snakes maximum length is 40 fields for performace reasons
- Although the apple should not spawn inside the snake, it can happen especially if the snake is very long
- Don't eat yourself or collide with the wall!

![Tux, the Linux mascot](/Documentation/Snake_Game_Total.jpeg)

![Tux, the Linux mascot](/Documentation/Snake_Game_Display.jpeg)