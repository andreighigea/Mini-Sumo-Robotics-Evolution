#ifndef Strategy_h
#define Strategy_h

#include "RandomSearch.h"
#include "Kamikaze.h"
#include "Wait2React.h"

// ========== Logic ==========
enum DebugMode {
  NORMAL = 0,
  LINE_SENSORS,
  FOLLOW
};
extern const uint8_t speed;
enum Strategies {
  RANDOM_SEARCH = 0,
  KAMIKAZE,
  WAIT_2_REACT
};
// ===========================

void RunStrategy(enum Strategies strategyIndex, enum DebugMode mode){
  switch (mode){
    case NORMAL:
      switch (strategyIndex){
        case RANDOM_SEARCH:
          RandomSearch();
          break;
        case KAMIKAZE:
          Kamikaze();
          break;
        case WAIT_2_REACT:
          Wait2React();
          break;
      }
      break;

    case LINE_SENSORS:
      break;

    case FOLLOW:
      break;
  }
}

#endif