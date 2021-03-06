#define NO_DATA -1
#define MERCURY_TO_HPA_CONVERSION 33.8638866667
#define DEFAULT_PRESSURE_SETTING 29.92f // inches of mercury
#define MS2_TO_G 0.101971621

#define COMMAND_ZERO_SENSORS 0
#define COMMAND_SET_PRESSURE 1
#define COMMAND_ENABLE_LOGGING 2
#define COMMAND_DISABLE_LOGGING 3
#define COMMAND_START_FLIGHT 4
#define COMMAND_END_FLIGHT 5
#define COMMAND_SET_EVENT 6
#define COMMAND_FIRE_EVENT 7
#define COMMAND_ARM_IGNITER 8
#define COMMAND_DISARM_IGNITER 9

#define COMMAND_ERR 0
#define COMMAND_ACK 1

// Flight phases
#define PAD 0
#define BOOST 1
#define COAST 2
#define DROGUE 3
#define MAIN 4
#define LANDED 5
