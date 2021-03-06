#include <stdbool.h>
#include <stdlib.h>

#include "../../common/commons.h"

#include "startMatch.h"
#include "startMatchDevice.h"
#include "startMatchDeviceInterface.h"
#include "endMatchDetectorDevice.h"

#include "../../common/io/binaryPrintWriter.h"
#include "../../common/io/inputStream.h"
#include "../../common/io/outputStream.h"
#include "../../common/io/printWriter.h"
#include "../../common/io/reader.h"

#include "../../common/eeprom/eeprom.h"
#include "../../common/eeprom/eepromAreas.h"

#include "../../common/log/logLevel.h"
#include "../../common/log/logger.h"

#include "../../device/device.h"

#include "../../drivers/strategy/strategyDriver.h"


#include "../../robot/config/robotConfigDevice.h"

static StartMatch* startMatch;
static RobotConfig* robotConfig;

void initStartMatchDevice(void) {

}

void stopStartMatchDevice(void) {

}

bool isStartMatchDeviceOk(void) {
    return true;
}

void notifyWaitingStart(OutputStream* pcOutputStream) {
    appendString(pcOutputStream, NOTIFY_TO_PC_RESET);
    appendString(getAlwaysOutputStreamLogger(), "CFG:");
    appendBinary16(getAlwaysOutputStreamLogger(), getConfigValue(robotConfig), 4);
    println(getAlwaysOutputStreamLogger());
    appendString(getAlwaysOutputStreamLogger(), "Waiting start:");
}

void notifyStarted(OutputStream* pcOutputStream) {
    // Notify the pc that the match started
    appendString(pcOutputStream, NOTIFY_TO_PC_START);
    appendStringCRLF(getAlwaysOutputStreamLogger(), "Go !");
}

void deviceStartMatchDetectorHandleRawData(char commandHeader, InputStream* inputStream, OutputStream* outputStream) {
    if (commandHeader == COMMAND_MATCH_IS_STARTED) {
        int value = isStarted(startMatch);
        ackCommand(outputStream, START_MATCH_DEVICE_HEADER, COMMAND_MATCH_IS_STARTED);
        appendHex2(outputStream, value);
    }
    if (commandHeader == COMMAND_MATCH_SET_STARTED) {
        int value = readHex2(inputStream);
        setSimulateStartedMatch(startMatch, value);

        ackCommand(outputStream, START_MATCH_DEVICE_HEADER, COMMAND_MATCH_SET_STARTED);
    }

    else if (commandHeader == COMMAND_STEP_BY_STEP) {
        robotNextStep();
        ackCommand(outputStream, START_MATCH_DEVICE_HEADER, COMMAND_STEP_BY_STEP);
    }
    else if (commandHeader == COMMAND_START_MATCH_GET_INITIAL_POSITION) {
        ackCommand(outputStream, START_MATCH_DEVICE_HEADER, COMMAND_START_MATCH_GET_INITIAL_POSITION);
        enum TeamColor teamColor = (enum TeamColor) readHex2(inputStream);
        RobotPosition robotPositionForColor;
        fillStartMatchPositionForColor(startMatch, &robotPositionForColor, teamColor);
    
        appendHex2(outputStream, teamColor);
        appendSeparator(outputStream);
        appendHex4(outputStream, robotPositionForColor.x);
        appendSeparator(outputStream);
        appendHex4(outputStream, robotPositionForColor.y);
        appendSeparator(outputStream);
        appendHex4(outputStream, robotPositionForColor.angleDeciDeg);
    }
    else if (commandHeader == COMMAND_START_MATCH_SET_INITIAL_POSITION) {
        ackCommand(outputStream, START_MATCH_DEVICE_HEADER, COMMAND_START_MATCH_SET_INITIAL_POSITION);

        enum TeamColor teamColor = (enum TeamColor) readHex2(inputStream);

        RobotPosition robotPositionForColor;
        checkIsSeparator(inputStream);
        robotPositionForColor.x = readHex4(inputStream);

        checkIsSeparator(inputStream);
        robotPositionForColor.y = readHex4(inputStream);

        checkIsSeparator(inputStream);
        robotPositionForColor.angleDeciDeg = readHex4(inputStream);
    
        saveMatchPositionForColor(startMatch, &robotPositionForColor, teamColor);
    }
}

// Allocate memory
DeviceDescriptor startMatchDetectorDevice = {
    .deviceInit = &initStartMatchDevice,
    .deviceShutDown = &stopStartMatchDevice,
    .deviceIsOk = &isStartMatchDeviceOk,
    .deviceHandleRawData = &deviceStartMatchDetectorHandleRawData,
};

DeviceDescriptor* getStartMatchDeviceDescriptor(StartMatch* startMatchParam, RobotConfig* robotConfigParam) {
    startMatch = startMatchParam;
    robotConfig = robotConfigParam;
    return &startMatchDetectorDevice;
}
