#ifndef EVENT_H
#define EVENT_H

#include "accelerometer.h"
#include "barometer.h"
#include "constants.h"
#include "radio.h"
#include "sensor.h"

#define NUM_EVENTS 2
#define DURATION 100 // ms
#define DEFAULT_ALTITUDE 152.4 // m
#define APOGEE -1

#define APOGEE_PIN 5
#define MAIN_PIN 6

#define EVENT_APOGEE 0
#define EVENT_MAIN 1

// A crude approximation of the number of loop() cycles we go through in one second
#define CYCLES_PER_SECOND 4

#define APOGEE_COUNTDOWN 15 // seconds
#define BOOST_ACCELERATION 1.25 // g
#define COAST_ACCELERATION 0.75 // g
#define APOGEE_IDEAL 0.25 // g
#define APOGEE_OKAY 0.5 // g
#define LANDED_ALTITUDE 10 // meters

typedef struct event_t {
  int pin;
  int altitude;
  int fired;
} event_t;

using namespace std;

class Event : public virtual Sensor {
  public:
    Event(Accelerometer accelerometer, Barometer barometer, Radio radio);
    int init();
    void check();
    void fire(int eventNum);
    void set(int eventNum, int altitude);
    int didFire(int eventNum);
    int altitude(int eventNum);
    int numEvents();
    int getPhase();
    void arm();
    void disarm();
    int isArmed();
    void resetFiredStatus();

  protected:
    void phasePad(float acceleration);
    void phaseBoost(float acceleration);
    void phaseCoast(float acceleration, int eventNum);
    void phaseDrogue(float acceleration, float altitude, int eventNum);
    void phaseMain(float acceleration, int eventNum);
    void phaseLanded();
    void atApogee(int eventNum);

    int armed;
    int phase;
    event_t events[NUM_EVENTS];

    Accelerometer accelerometer;
    Barometer barometer;
    Radio radio;
};

#endif
