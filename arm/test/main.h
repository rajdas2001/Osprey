#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <catch/single_include/catch.hpp>

#include "event.h"
#include "gps.h"
#include "stub.h"

#define STABILIZE_ITERATIONS 5

#define DEFAULT_TEST_ACCELERATION 1
#define DEFAULT_TEST_ALTITUDE 100

namespace Osprey {
  extern Accelerometer accelerometer;
  extern Barometer barometer;
  extern Event event;
  extern GPS gps;
  extern Radio radio;
  Stub stub;

  extern int commandStatus;
}

using namespace Osprey;

extern void setup(void);
extern void loop(void);

void testFlightPhases();
void setupTestForFixture(char *fixture);

int step(size_t steps=1, size_t iterations=STABILIZE_ITERATIONS);
void stabilize(size_t iterations=STABILIZE_ITERATIONS);
