/*
 * Author: Marcel Tiews
 * License: Apache 2
 * 
 * Usage: 
 * - Construct Automode with the proper callbacks for movement
 * - call nextMove() continuously in loop() 
 * - reset state via reset(), if vehicle stops 
 * 
 * Autonomous mode for a car implemented via state machine.
 * Behaviour:
 *  - Before each move checks whether an obstacle is in sight (obstacleInSight())
 *  - if no obstacle is in sight in drives forward (moveForward())
 *  - if an obstacle is in sight it randomly turns left (moveLeft()) or right (moveRight()) until no obstacle is in sight
 *  - stop after AUTOMODE_MAX_TURNCOUNT if (when moving left or right) there is still a obstacle in sight
 */
#ifndef AUTOMODE_H
#define AUTOMODE_H

#include "Arduino.h"

#define AUTOMODE_STATE_STOP 0
#define AUTOMODE_STATE_FORWARD 1
#define AUTOMODE_STATE_TURNLEFT 2
#define AUTOMODE_STATE_TURNRIGHT 3
#define AUTOMODE_MAX_TURNCOUNT 50

class Automode {
  public:
    Automode(
      boolean (*obstacleInSightHandler)(void), 
      void (*moveForwardHandler)(void), 
      void (*moveLeftHandler)(void), 
      void (*moveRightHandler)(void), 
      void (*moveStopHandler)(void)
    );
    boolean nextMove();
    void reset();
  private:
    boolean (*_obstacleInSightHandler)(void); 
    void (*_moveForwardHandler)(void); 
    void (*_moveLeftHandler)(void);
    void (*_moveRightHandler)(void);
    void (*_moveStopHandler)(void);

    int _currentAutoMode;
    int _turnCount;
};

#endif
