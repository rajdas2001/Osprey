#define CATCH_CONFIG_MAIN
#include "main.h"

using namespace Osprey;

TEST_CASE("should have correct flight phases with altitude apogee event") {
  setupTestForFixture((char*)"test/fixtures/1.json");
  testFlightPhases();
}

TEST_CASE("should have correct flight phases with countdown apogee event") {
  setupTestForFixture((char*)"test/fixtures/2.json");
  testFlightPhases();
}

TEST_CASE("should have correct flight phases with safety countdown apogee event") {
  setupTestForFixture((char*)"test/fixtures/3.json");
  testFlightPhases();
}

TEST_CASE("should have correct flight phases with free fall apogee event") {
  setupTestForFixture((char*)"test/fixtures/4.json");
  testFlightPhases();
}

void testFlightPhases() {
  // Start flight command
  Serial1.insert("4\n");

  while(stub.read()) {
    stabilize();
    REQUIRE(event.getPhase() == stub.getField("expected_phase").intVal);
  }

  REQUIRE(event.getApogeeCause() == stub.getField("expected_apogee_cause").intVal);
}

TEST_CASE("should arm igniter when sent arm igniter command") {
  setup();

  // Send the arm igniter command
  Serial1.insert("8\n");

  REQUIRE(event.isArmed() == 0);
  step();
  REQUIRE(event.isArmed() == 1);
  REQUIRE(commandStatus == COMMAND_ACK);
}

TEST_CASE("should disarm igniter when sent disarm igniter command") {
  setup();
  event.arm();

  // Send the disarm igniter command
  Serial1.insert("9\n");

  REQUIRE(event.isArmed() == 1);
  step();
  REQUIRE(event.isArmed() == 0);
  REQUIRE(commandStatus == COMMAND_ACK);
}

TEST_CASE("should firm apogee event when sent fire command") {
  setup();
  event.arm();
  Serial1.insert("70\n");

  REQUIRE(event.didFire(0) == 0);
  step();
  REQUIRE(event.didFire(0) == 1);
  REQUIRE(commandStatus == COMMAND_ACK);
}

TEST_CASE("should fire main event when sent fire command") {
  setup();
  event.arm();
  Serial1.insert("71\n");

  REQUIRE(event.didFire(1) == 0);
  step();
  REQUIRE(event.didFire(1) == 1);
  REQUIRE(commandStatus == COMMAND_ACK);
}

TEST_CASE("should not fire apogee when disarmed and sent fire command") {
  setup();
  Serial1.insert("70\n");

  REQUIRE(event.didFire(0) == 0);
  step();
  REQUIRE(event.didFire(0) == 0);
  REQUIRE(commandStatus == COMMAND_ERR);
}

TEST_CASE("should set event altitude when sent set event command") {
  setup();
  Serial1.insert("61100\n");

  REQUIRE(event.getAltitude(1) == DEFAULT_MAIN_ALTITUDE);
  step();
  REQUIRE(event.getAltitude(1) == 100);
  REQUIRE(commandStatus == COMMAND_ACK);
}

TEST_CASE("should arm igniter, enable logging, and zero sensors when sent start flight command") {
  setup();
  Serial1.insert("4\n");

  REQUIRE(event.isArmed() == 0);
  REQUIRE(radio.isLogging() == 0);
  REQUIRE(barometer.getAltitudeAboveGround() != 0);

  step();

  REQUIRE(event.isArmed() == 1);
  REQUIRE(radio.isLogging() == 1);
  REQUIRE(barometer.getAltitudeAboveGround() == 0);
  REQUIRE(commandStatus == COMMAND_ACK);
}

TEST_CASE("should disarm igniter and disable logging when sent end flight command") {
  setup();

  // Start the flight before ending it
  Serial1.insert("4\n");
  step();

  Serial1.insert("5\n");

  REQUIRE(event.isArmed() == 1);
  REQUIRE(radio.isLogging() == 1);

  step();

  REQUIRE(event.isArmed() == 0);
  REQUIRE(radio.isLogging() == 0);
  REQUIRE(commandStatus == COMMAND_ACK);
}

TEST_CASE("should set pressure when sent set pressure command") {
  setup();

  Serial1.insert("142.37\n");

  REQUIRE(barometer.getPressureSetting() == DEFAULT_PRESSURE_SETTING);
  step();
  REQUIRE(barometer.getPressureSetting() == 42.37f);
  REQUIRE(commandStatus == COMMAND_ACK);
}

void setupTestForFixture(char *fixture) {
  if(!stub.open(fixture)) {
    fprintf(stderr, "Error opening fixture: %s: %s\n", fixture, strerror(errno));
    exit(1);
  }

  setup();

  stub_t acceleration = {DEFAULT_TEST_ACCELERATION};
  stub_t altitude = {DEFAULT_TEST_ALTITUDE};

  stub.setField("acceleration", acceleration);
  stub.setField("pressure_altitude", altitude);
}

void step(size_t steps, size_t iterations) {
  for(unsigned int i=0; i<steps; i++) {
    stub.read();
    stabilize(iterations);
  }
}

void stabilize(size_t iterations) {
  for(unsigned int i=0; i<iterations; i++) {
    loop();
  }
}
