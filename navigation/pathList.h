#ifndef PATH_LIST_H
#define PATH_LIST_H

#include "../common/io/outputStream.h"

#include "path.h"
#include "location.h"

/** The max limit of path count. */
#define MAX_PATH          31

/**
 * Tre struct defining a list of paths.
 */
typedef struct {
    /** An array of pointer on paths. */
    PathData(*paths)[];
    /** the size of the list. */
    unsigned int size;
    /** the max size of the list. */
    unsigned int maxSize;
} PathList;

/**
 * Initializes the path.
 * @param pathList the pathList to initialize.
 */
void initPathList(PathList* pathList, PathData(*pathListArray)[], unsigned int pathListSize);

/**
 * Clear the path list.
 */
void clearPathList(PathList* pathList);

/**
 * Fill a path and add a path to the list.
 * @return the path from the list
 */
PathData* addPath(PathList* pathList);

/**
* Add a Path, with structure filled with all data.
*/
PathData* addFilledPath(PathList* pathList,
	LocationList* locationList,
	char* locationName1, char* locationName2,
	unsigned int cost,
	int controlPointDistance1, int controlPointDistance2,
	int angle1, int angle2,
	unsigned char accelerationFactor, unsigned char speedFactor,
	bool mustGoBackward);

/**
 * Get the path at index.
 */
PathData* getPath(PathList* pathList, unsigned int index);

/**
 * Returns the path corresponding to the both location.
 */
PathData* getPathOfLocations(PathList* pathList, Location* location1, Location* location2, bool* reversed);

/**
 * Get the count of paths.
 */
unsigned int getPathCount(PathList* pathList);

/**
 * Clear the outgoing paths.
 */
void resetOutgoingPathInfo(PathList* pathList);

// DEBUG

/**
 * Print the list of path.
 */
void printPathList(OutputStream* outputStream, char* pathListName, PathList* pathList);

/**
* Print the list of path as table.
*/
void printPathListTable(OutputStream* outputStream, PathList* pathList);

// TESTS DATA

void addPathListTestsData(PathList* pathList, LocationList* locationList);

#endif
