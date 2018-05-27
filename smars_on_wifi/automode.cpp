#include "Automode.h"

/* c'tor */
Automode::Automode(boolean (*obstacleInSightHandler)(void), void (*moveForwardHandler)(void), void(*moveLeftHandler)(void), void(*moveRightHandler)(void), void(*moveStopHandler)(void))
: _obstacleInSightHandler(obstacleInSightHandler)
, _moveForwardHandler(moveForwardHandler)
, _moveLeftHandler(moveLeftHandler)
, _moveRightHandler(moveRightHandler)
, _moveStopHandler(moveStopHandler) 
{
  reset();
}

/* resets automode to its initial state */
void Automode::reset() {
  _currentAutoMode = AUTOMODE_STATE_FORWARD;
  _turnCount = 0;
}

/* triggers the next move, returns true if still moving, returns false if stopped */
boolean Automode::nextMove() {
  boolean result = true;
  
  // If to many turns in a row, then stop, maybe in a trap?
  _currentAutoMode = _turnCount > AUTOMODE_MAX_TURNCOUNT ? AUTOMODE_STATE_STOP : _currentAutoMode;
  boolean obstacleInSight = _obstacleInSightHandler();
  
  switch (_currentAutoMode) {
    case AUTOMODE_STATE_FORWARD:
      if(obstacleInSight) {
        _currentAutoMode = random(1, 3) == 1 ? AUTOMODE_STATE_TURNLEFT : AUTOMODE_STATE_TURNRIGHT;
      } else {
        _turnCount = 0;
        _moveForwardHandler();
      }
      break;
    case AUTOMODE_STATE_TURNLEFT:
      if(obstacleInSight) {
        _turnCount++;
        _moveLeftHandler();
      } else {
        _currentAutoMode = AUTOMODE_STATE_FORWARD;
      }
      break;
    case AUTOMODE_STATE_TURNRIGHT:
      if(obstacleInSight) {
        _turnCount++;
        _moveRightHandler();
      } else {
        _currentAutoMode = AUTOMODE_STATE_FORWARD;
      }
      break;
    case AUTOMODE_STATE_STOP:
    default:
      _moveStopHandler();
      result = false;

    return result;
  }
}
