#include <stdbool.h>

#include "extendedMotionDeviceInterface.h"

#include "../../../device/device.h"
#include "../../../device/deviceInterface.h"
#include "../../../device/deviceConstants.h"

const char* deviceExtendedMotionGetName(void) {
	return "extendedMotion";
}

void fillSplineParameters(void) {
	// X3/Y3 as absolute position
	setArgumentUnsignedHex4(0, "x3(mm)");
	setArgumentSeparator(1);
	setArgumentUnsignedHex4(2, "y3(mm)");
	setArgumentSeparator(3);
	setArgumentUnsignedHex4(4, "finalAngDecDeg");
	setArgumentSeparator(5);
	setArgumentSignedHex4(6, "distP0-P1(mm)");
	setArgumentSeparator(7);
	setArgumentSignedHex4(8, "distP2-P3(mm)");
	setArgumentSeparator(9);
	setArgumentUnsignedChar1(10, "speedFactor");
	setArgumentUnsignedChar1(11, "accFactor");
}

int deviceExtendedMotionGetInterface(char commandHeader, DeviceInterfaceMode mode, bool fillDeviceArgumentList) {
	// B-Spline with relative spline
	if (commandHeader == COMMAND_MOTION_SPLINE_RELATIVE) {
		if (fillDeviceArgumentList) {
			setFunction("bSplineRel", 12, 0);
			fillSplineParameters();
		}
		return commandLengthValueForMode(mode, 27, 0);
	}// B-Spline with absolute spline
	else if (commandHeader == COMMAND_MOTION_SPLINE_ABSOLUTE) {
		if (fillDeviceArgumentList) {
			setFunction("bSplineAbs", 12, 0);
			fillSplineParameters();
		}
		return commandLengthValueForMode(mode, 27, 0);
	} // BSpline : test
	else if (commandHeader == COMMAND_MOTION_SPLINE_TEST_LEFT) {
		// Same INPUT/OUTPUT
		if (fillDeviceArgumentList) {
			setFunctionNoArgumentAndNoResult("bSplineTestLeft");
		}
		return commandLengthValueForMode(mode, 0, 0);
	}
	else if (commandHeader == COMMAND_MOTION_SPLINE_TEST_RIGHT) {
		// Same INPUT/OUTPUT
		if (fillDeviceArgumentList) {
			setFunctionNoArgumentAndNoResult("bSplineTestRight");
		}
		return commandLengthValueForMode(mode, 0, 0);
	}
	return DEVICE_HEADER_NOT_HANDLED;
}

static DeviceInterface deviceInterface = {
	.deviceHeader = EXTENDED_MOTION_DEVICE_HEADER,
	.deviceGetName = &deviceExtendedMotionGetName,
	.deviceGetInterface = &deviceExtendedMotionGetInterface
};

DeviceInterface* getExtendedMotionDeviceInterface(void) {
	return &deviceInterface;
}
