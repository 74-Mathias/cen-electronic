#ifndef PID_MOTION_PROFIL_COMPUTER_H
#define PID_MOTION_PROFIL_COMPUTER_H

#include "../../../motion/parameters/motionParameterType.h"
#include "../pidType.h"
#include "../instructionType.h"
#include "../motionInstruction.h"
#include "profileType.h"

/**
 * Clear initial speeds to avoid that the trajectory finished
 * when we stop the robot.
 */
void clearInitialSpeeds();

void computeMotionInstruction(MotionInstruction* inst);

/**
 * Set the next Position that the robot must reach.
 * @param instructionType the index of which instruction to follow
 * @param motionParameterType the type of motion which must be used
 * @param pidType the type of pid which must be used
 * @param pNextPosition can be either negative or positive
 * @param pa is the maximal absolute value of acceleration
 * @param pSpeedMax is the maximal absolute value of speed
 */
void setNextPosition(enum InstructionType instructionType,
        enum MotionParameterType motionParameterType,
        enum PidType pidType,
        float pNextPosition,
        float pa,
        float pSpeedMax);


#endif