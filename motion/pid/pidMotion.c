#include "pidMotion.h"

#include <stdbool.h>

#include "../../common/error/error.h"
#include "../../robot/kinematics/robotKinematics.h"

#include "pidTimer.h"

bool checkPidMotionNotNull(const PidMotion* pidMotion) {
	if (pidMotion == NULL) {
		writeError(PID_MOTION_NULL);
		return false;
	}
	return true;
}

bool isPidMotionInitialized(const PidMotion* pidMotion) {
	if (!checkPidMotionNotNull(pidMotion)) {
		return false;
	}
	return pidMotion->length > 0;
}

void clearPidMotion(PidMotion* pidMotion) {
	if (!checkPidMotionNotNull(pidMotion)) {
		return;
	}
	pidMotion->writeIndex = 0;
	pidMotion->readIndex = 0;
}

bool isPidMotionFull(const PidMotion* pidMotion) {
	if (!checkPidMotionNotNull(pidMotion)) {
		return false;
	}
	return ((pidMotion->writeIndex + 1) % pidMotion->length) == pidMotion->readIndex;
}

bool isPidMotionEmpty(const PidMotion* pidMotion) {
	if (!checkPidMotionNotNull(pidMotion)) {
		return true;
	}
	return pidMotion->readIndex == pidMotion->writeIndex;
}

unsigned int getPidMotionElementsCount(const PidMotion* pidMotion) {
	if (!checkPidMotionNotNull(pidMotion)) {
		return 0;
	}
	int result = pidMotion->writeIndex - pidMotion->readIndex;
	if (result < 0) {
		result += pidMotion->length;
	}
	return result;
}

unsigned int getPidMotionCapacity(const PidMotion* pidMotion) {
	if (!checkPidMotionNotNull(pidMotion)) {
		return 0;
	}
	return pidMotion->length - 1;
}

PidMotionDefinition* pidMotionReadMotionDefinition(PidMotion* pidMotion) {
	if (!checkPidMotionNotNull(pidMotion)) {
		return NULL;
	}
	bool isEmpty = isPidMotionEmpty(pidMotion);
	if (!isEmpty) {
		PidMotionDefinition* result = (PidMotionDefinition*)pidMotion->motionDefinitions;
		// Shift to the right cell index
		result += pidMotion->readIndex;

		pidMotion->readIndex++;
		pidMotion->readIndex %= pidMotion->length;
		return result;
	}
	else {
		// We must log the problem
		writeError(PID_MOTION_EMPTY);
		return 0;
	}
}

PidMotionDefinition* pidMotionGetNextToWritePidMotionDefinition(PidMotion* pidMotion) {
	if (!checkPidMotionNotNull(pidMotion)) {
		return NULL;
	}
	// If we stack or if the pidMotion is Empty, we add a new definition !
	if (pidMotion->stackMotionDefinitions || isPidMotionEmpty(pidMotion)) {
		bool isFull = isPidMotionFull(pidMotion);
		if (!isFull) {
			PidMotionDefinition* result = (PidMotionDefinition*)pidMotion->motionDefinitions;
			// Shift to the right cell index
			result += pidMotion->writeIndex;
			// For next time
			pidMotion->writeIndex++;
			pidMotion->writeIndex %= pidMotion->length;
			return result;
		}
		else {
			// We must log the problem
			writeError(PID_MOTION_FULL);
			return NULL;
		}
	}
	// if we don't stack and already a current motion Definition
	return pidMotionGetCurrentMotionDefinition(pidMotion);
}

PidMotionDefinition* pidMotionGetCurrentMotionDefinition(PidMotion* pidMotion) {
	if (!checkPidMotionNotNull(pidMotion)) {
		return NULL;
	}
	PidMotionDefinition* result = (PidMotionDefinition*)pidMotion->motionDefinitions;
	// Shift to the right cell index
	result += pidMotion->readIndex;

	return result;
}

PidMotionDefinition* getMotionDefinition(PidMotion* pidMotion, unsigned int index) {
	unsigned int size = getPidMotionElementsCount(pidMotion);
	if (index < size) {
		PidMotionDefinition* result = (PidMotionDefinition*)pidMotion->motionDefinitions;
		// Shift to the right cell index
		result += ((pidMotion->readIndex + index) % pidMotion->length);

		return result;
	}
	else {
		// We must log the problem
		writeError(PID_MOTION_NOT_ENOUGH_DATA);
	}
	return 0;
}

// STACK / REPLACE MODE

void setMotionModeAdd(PidMotion* pidMotion) {
	pidMotion->stackMotionDefinitions = true;
}

void setMotionModeReplace(PidMotion* pidMotion) {
	pidMotion->stackMotionDefinitions = false;
}

bool isStackMotionDefinitions(PidMotion* pidMotion) {
	return pidMotion->stackMotionDefinitions;
}

// MOTION PARAMETERS

MotionEndDetectionParameter* getMotionEndDetectionParameter(PidMotion* pidMotion) {
	return &(pidMotion->globalParameters.motionEndDetectionParameter);
}

// INIT

void initPidMotion(PidMotion* pidMotion, Eeprom* _eeprom, PidMotionDefinition(*array)[], unsigned int length) {
	if (!checkPidMotionNotNull(pidMotion)) {
		return;
	}
	pidMotion->motionDefinitions = array;
	pidMotion->length = length;
	pidMotion->pidPersistenceEeprom = _eeprom;
    initMotionEndParameter(getMotionEndDetectionParameter(pidMotion));

	// loadPidParameters(loadDefaultParameters);
	RobotKinematics* robotKinematics = getRobotKinematics();
	loadRobotKinematicsParameters(robotKinematics, _eeprom, true);
	initPidTimer();

	// PidMotionDefinition* motionDefinition = getCurrentMotionDefinition();
	// initFirstTimeBSplineCurve(&(motionDefinition->curve));
}

