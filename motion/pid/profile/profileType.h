#ifndef PROFILE_TYPE_H
#define PROFILE_TYPE_H

#include "../../../common/io/outputStream.h"

enum ProfileType {

    /** Acceleration phase / constant speed / Deceleration phase. */
    PROFILE_TYPE_TRAPEZE = 0,

    /** Type of trajectory in which we do not reach the max speed (=> acceleration phase, deceleration phase). */
    PROFILE_TYPE_TRIANGLE = 1
};

void appendProfileTypeAsString(enum ProfileType profileType, OutputStream* outputStream);

#endif