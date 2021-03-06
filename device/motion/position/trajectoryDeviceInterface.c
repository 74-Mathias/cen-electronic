#include <stdbool.h>

#include "trajectoryDeviceInterface.h"

#include "../../../device/device.h"
#include "../../../device/deviceInterface.h"
#include "../../../device/deviceConstants.h"

#define GET_ABS_POS_STRING             "getAbsPosition"
#define GET_DEBUG_ABS_POS_STRING     "getDebugAbsPosition"
#define SET_ABS_POS_STRING             "setAbsPosition"
#define ANGLE_1_10_DEG                 "angle(1/10)deg"
#define X_MM                         "x(mm)"
#define Y_MM                         "y(mm)"

const char* getTrajectoryDeviceName(void) {
    return "Trajectory";
}

int trajectoryGetInterface(char commandHeader, DeviceInterfaceMode mode, bool fillDeviceArgumentList) {
    if (commandHeader == COMMAND_TRAJECTORY_GET_ABSOLUTE_POSITION) {
        if (fillDeviceArgumentList) {
            setFunction(GET_ABS_POS_STRING, 0, 5);
            setResultUnsignedHex4(0, X_MM);
            setResultSeparator(1);
            setResultUnsignedHex4(2, Y_MM);
            setResultSeparator(3);
            setResultUnsignedHex4(4, ANGLE_1_10_DEG);
        }
        return commandLengthValueForMode(mode, 0, 14);
    } else if (commandHeader == COMMAND_TRAJECTORY_DEBUG_GET_ABSOLUTE_POSITION) {
        // Same return in case of input / output
        if (fillDeviceArgumentList) {
            setFunctionNoArgumentAndNoResult(GET_DEBUG_ABS_POS_STRING);
        }
        return commandLengthValueForMode(mode, 0, 0);
	}
	else if (commandHeader == COMMAND_TRAJECTORY_DEBUG_CODERS) {
		// Same return in case of input / output
		if (fillDeviceArgumentList) {
			setFunctionNoArgumentAndNoResult("Debug Coders History");
		}
		return commandLengthValueForMode(mode, 0, 0);
	} else if (commandHeader == COMMAND_TRAJECTORY_SET_ABSOLUTE_POSITION) {
        if (fillDeviceArgumentList) {
            setFunction(SET_ABS_POS_STRING, 5, 0);
            setArgumentUnsignedHex4(0, X_MM);
            setArgumentSeparator(1);
            setArgumentUnsignedHex4(2, Y_MM);
            setArgumentSeparator(3);
            setArgumentUnsignedHex4(4, ANGLE_1_10_DEG);
        }
        return commandLengthValueForMode(mode, 14, 0);
    }
    return DEVICE_HEADER_NOT_HANDLED;
}

static DeviceInterface deviceInterface = {
    .deviceHeader = TRAJECTORY_DEVICE_HEADER,
    .deviceGetName = &getTrajectoryDeviceName,
    .deviceGetInterface = &trajectoryGetInterface
};

DeviceInterface* getTrajectoryDeviceInterface(void) {
    return &deviceInterface;
}
