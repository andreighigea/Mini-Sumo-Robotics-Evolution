#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H

#include <Arduino.h>

#define MAX_LISTENERS 5  // Max listeners per event
#define MAX_EVENTS 5     // Max events

typedef void (*Callback)(void*);

struct Event {
  String name;
  Callback listeners[MAX_LISTENERS];
  int listenerCount;
};

class EventManager {
public:
  EventManager() : eventCount(0) {}

  void on(const String& eventName, Callback callback) {
    Event* ev = getEvent(eventName);
    if (!ev) {
      if (eventCount >= MAX_EVENTS) return;
      ev = &events[eventCount++];
      ev->name = eventName;
      ev->listenerCount = 0;
    }
    if (ev->listenerCount < MAX_LISTENERS) {
      ev->listeners[ev->listenerCount++] = callback;
    }
  }

  void off(const String& eventName, Callback callback) {
    Event* ev = getEvent(eventName);
    if (!ev) return;

    for (int i = 0; i < ev->listenerCount; i++) {
      if (ev->listeners[i] == callback) {
        // Shift remaining callbacks down
        for (int j = i; j < ev->listenerCount - 1; j++) {
          ev->listeners[j] = ev->listeners[j + 1];
        }
        ev->listenerCount--;
        break;
      }
    }
  }

  void emit(const String& eventName, void* data = nullptr) {
    Event* ev = getEvent(eventName);
    if (!ev) return;

    for (int i = 0; i < ev->listenerCount; i++) {
      ev->listeners[i](data);
    }
  }

private:
  Event* getEvent(const String& eventName) {
    for (int i = 0; i < eventCount; i++) {
      if (events[i].name == eventName) return &events[i];
    }
    return nullptr;
  }

  Event events[MAX_EVENTS];
  int eventCount;
};

#endif
