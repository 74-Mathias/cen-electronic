#include <stdbool.h>
#include <stdlib.h>

#include "navigationDevice.h"
#include "navigationDeviceInterface.h"

#include "../../common/error/error.h"
#include "../../common/io/printWriter.h"
#include "../../common/io/printTableWriter.h"
#include "../../common/io/reader.h"

#include "../../common/log/logger.h"

#include "../../device/device.h"

#include "../../navigation/navigation.h"
#include "../../navigation/location.h"
#include "../../navigation/locationList.h"
#include "../../navigation/path.h"
#include "../../navigation/pathList.h"

// TEMP

#define NAVIGATION_LOCATION_LIST_ARRAY_LENGTH        30
#define NAVIGATION_PATH_LIST_TEST_ARRAY_LENGTH       30
#define BIT_LIST_NAVIGATION_TEST_ARRAY_LENGTH        30

static LocationList locationList;
static Location locationListArray[NAVIGATION_LOCATION_LIST_ARRAY_LENGTH];

static PathList pathList;
static PathData pathListArray[NAVIGATION_PATH_LIST_TEST_ARRAY_LENGTH];

static BitList outgoingPathBitList;
static unsigned int outgoingPathBitArray[BIT_LIST_NAVIGATION_TEST_ARRAY_LENGTH];
static BitList availablePathBitList;
static unsigned int availablePathBitArray[BIT_LIST_NAVIGATION_TEST_ARRAY_LENGTH];

void deviceNavigationInit(void) {

    initLocationList(&locationList, (Location(*)[]) &locationListArray, NAVIGATION_LOCATION_LIST_ARRAY_LENGTH);
    initPathList(&pathList, (PathData(*)[]) &pathListArray, NAVIGATION_PATH_LIST_TEST_ARRAY_LENGTH);
    initBitList(&outgoingPathBitList, (unsigned int(*)[]) &outgoingPathBitArray, BIT_LIST_NAVIGATION_TEST_ARRAY_LENGTH);
    initBitList(&availablePathBitList, (unsigned int(*)[]) &availablePathBitArray, BIT_LIST_NAVIGATION_TEST_ARRAY_LENGTH);

    initNavigation(&locationList, &pathList, &outgoingPathBitList, &availablePathBitList);
}

void deviceNavigationShutDown(void) {
}

bool isNavigationDeviceOk(void) {
    return true;
}

void deviceNavigationHandleRawData(char commandHeader, InputStream* inputStream, OutputStream* outputStream) {
    // Locations
    if (commandHeader == COMMAND_NAVIGATION_LOCATION_COUNT) {
        ackCommand(outputStream, NAVIGATION_DEVICE_HEADER, COMMAND_NAVIGATION_LOCATION_COUNT);

        LocationList* locationList = getNavigationLocationList();
        unsigned int locationCount = getLocationCount(locationList);
        appendHex4(outputStream, locationCount);
    }
    else if (commandHeader == COMMAND_NAVIGATION_GET_LOCATION) {
        ackCommand(outputStream, NAVIGATION_DEVICE_HEADER, COMMAND_NAVIGATION_GET_LOCATION);
        unsigned int locationIndex = readHex4(inputStream);
        LocationList* locationList = getNavigationLocationList();
        Location* location = getLocation(locationList, locationIndex);
        appendHexFixedCharArray(outputStream, &(location->name));
        appendSeparator(outputStream);
        appendHex4(outputStream, location->x);
        appendSeparator(outputStream);
        appendHex4(outputStream, location->y);
    }
    else if (commandHeader == COMMAND_NAVIGATION_LOCATION_CLEAR) {
        ackCommand(outputStream, NAVIGATION_DEVICE_HEADER, COMMAND_NAVIGATION_GET_LOCATION);
        LocationList* locationList = getNavigationLocationList();
        clearLocationList(locationList);
    }
    else if (commandHeader == COMMAND_NAVIGATION_SET_LOCATION) {
        ackCommand(outputStream, NAVIGATION_DEVICE_HEADER, COMMAND_NAVIGATION_SET_LOCATION);
        LocationList* locationList = getNavigationLocationList();
        FixedCharArray locationNameFixedCharArray;
		readHexFixedCharArray(inputStream, &locationNameFixedCharArray);
        checkIsSeparator(inputStream);
        int x = readHex4(inputStream);
        checkIsSeparator(inputStream);
        int y = readHex4(inputStream);

        // Find an existing location if any to update or create if it doesn't find
        Location* location = findLocationByName(locationList, &locationNameFixedCharArray);
        if (location == NULL) {
            addLocation(locationList, &locationNameFixedCharArray, x, y);
        }
        else {
            copyFixedCharArray(&locationNameFixedCharArray, &(location->name));
            location->x = x;
            location->y = y;
        }
    }
	else if (commandHeader == COMMAND_NAVIGATION_LOCATION_LIST) {
		ackCommand(outputStream, NAVIGATION_DEVICE_HEADER, COMMAND_NAVIGATION_LOCATION_LIST);
		LocationList* locationList = getNavigationLocationList();
		printLocationListTable(getAlwaysOutputStreamLogger(), locationList);
	}
	else if (commandHeader == COMMAND_NAVIGATION_LOCATION_ADD_TESTS_DATA) {
		ackCommand(outputStream, NAVIGATION_DEVICE_HEADER, COMMAND_NAVIGATION_LOCATION_ADD_TESTS_DATA);
		LocationList* locationList = getNavigationLocationList();
		addLocationListTestsData(locationList);
	}
    // Paths
    else if (commandHeader == COMMAND_NAVIGATION_PATH_COUNT) {
        ackCommand(outputStream, NAVIGATION_DEVICE_HEADER, COMMAND_NAVIGATION_PATH_COUNT);

        PathList* pathList = getNavigationPathList();
        unsigned int pathCount = getPathCount(pathList);
        appendHex4(outputStream, pathCount);
    }
    else if (commandHeader == COMMAND_NAVIGATION_GET_PATH) {
        ackCommand(outputStream, NAVIGATION_DEVICE_HEADER, COMMAND_NAVIGATION_GET_PATH);
        unsigned int pathIndex = readHex4(inputStream);
        PathList* pathList = getNavigationPathList();
        PathData* pathData = getPath(pathList, pathIndex);

        Location* location1 = pathData->location1;
        if (location1 == NULL) {
            writeError(LOCATION_DOES_NOT_EXIST);
            appendString(outputStream, "\0\0\0\0");
        }
        else {
            FixedCharArray* locationName1 = &(location1->name);
			appendHexFixedCharArray(outputStream, locationName1);
        }
        appendSeparator(outputStream);

        Location* location2 = pathData->location2;
        if (location2 == NULL) {
            writeError(LOCATION_DOES_NOT_EXIST);
            appendString(outputStream, "\0\0\0\0");
        }
        else {
            FixedCharArray* locationName2 = &(location2->name);
			appendHexFixedCharArray(outputStream, locationName2);
        }
        appendSeparator(outputStream);
        appendHex4(outputStream, pathData->cost);
        appendSeparator(outputStream);
        appendHex4(outputStream, pathData->controlPointDistance1);
        appendSeparator(outputStream);
        appendHex4(outputStream, pathData->controlPointDistance2);
        appendSeparator(outputStream);
        appendHex4(outputStream, pathData->angle1);
        appendSeparator(outputStream);
        appendHex4(outputStream, pathData->angle2);
        appendSeparator(outputStream);
        appendHex2(outputStream, pathData->accelerationFactor);
        appendSeparator(outputStream);
        appendHex2(outputStream, pathData->speedFactor);
        appendSeparator(outputStream);
        appendBool(outputStream, pathData->mustGoBackward);
    }
    else if (commandHeader == COMMAND_NAVIGATION_PATH_CLEAR) {
        ackCommand(outputStream, NAVIGATION_DEVICE_HEADER, COMMAND_NAVIGATION_PATH_CLEAR);
        PathList* pathList = getNavigationPathList();
        clearPathList(pathList);
    }
    else if (commandHeader == COMMAND_NAVIGATION_SET_PATH) {
        ackCommand(outputStream, NAVIGATION_DEVICE_HEADER, COMMAND_NAVIGATION_SET_PATH);
        PathList* pathList = getNavigationPathList();
        LocationList* locationList = getNavigationLocationList();

        FixedCharArray tempLocationName1;
		readHexFixedCharArray(inputStream, &tempLocationName1);
        checkIsSeparator(inputStream);
        FixedCharArray tempLocationName2;
		readHexFixedCharArray(inputStream, &tempLocationName2);
        checkIsSeparator(inputStream);


        Location* location1 = findLocationByName(locationList, &tempLocationName1);
        if (location1 == NULL) {
            writeError(LOCATION_DOES_NOT_EXIST);
        }

        Location* location2 = findLocationByName(locationList, &tempLocationName2);
        if (location2 == NULL) {
            writeError(LOCATION_DOES_NOT_EXIST);
        }

        bool reversed = false;
        // TODO : Must returns a different path if reversed ?
        PathData* pathData = getPathOfLocations(pathList, location1, location2, &reversed);

        // Create if does not exist
        if (pathData == NULL) {
            pathData = addPath(pathList);
        }
        pathData->location1 = location1;
        pathData->location2 = location2;

        pathData->cost = readHex4(inputStream);
        checkIsSeparator(inputStream);
        pathData->controlPointDistance1 = readHex4(inputStream);
        checkIsSeparator(inputStream);
        pathData->controlPointDistance2 = readHex4(inputStream);
        checkIsSeparator(inputStream);
        pathData->angle1 = readHex4(inputStream);
        checkIsSeparator(inputStream);
        pathData->angle2 = readHex4(inputStream);
        checkIsSeparator(inputStream);
        pathData->accelerationFactor = readHex2(inputStream);
        checkIsSeparator(inputStream);
        pathData->speedFactor = readHex2(inputStream);
        checkIsSeparator(inputStream);
        pathData->mustGoBackward = readBool(inputStream);
    }
	else if (commandHeader == COMMAND_NAVIGATION_PATH_LIST) {
		ackCommand(outputStream, NAVIGATION_DEVICE_HEADER, COMMAND_NAVIGATION_PATH_LIST);
		PathList* pathList = getNavigationPathList();
		printPathListTable(getAlwaysOutputStreamLogger(), pathList);
	}
	else if (commandHeader == COMMAND_NAVIGATION_PATH_LIST_ADD_TESTS_DATA) {
		ackCommand(outputStream, NAVIGATION_DEVICE_HEADER, COMMAND_NAVIGATION_PATH_LIST_ADD_TESTS_DATA);
		LocationList* locationList = getNavigationLocationList();
		PathList* pathList = getNavigationPathList();
		addPathListTestsData(pathList, locationList);
	}
}

static DeviceDescriptor descriptor = {
    .deviceInit = &deviceNavigationInit,
    .deviceShutDown = &deviceNavigationShutDown,
    .deviceIsOk = &isNavigationDeviceOk,
    .deviceHandleRawData = &deviceNavigationHandleRawData,
};

DeviceDescriptor* getNavigationDeviceDescriptor() {
    return &descriptor;
}
