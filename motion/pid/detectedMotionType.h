#ifndef PID_POSITION_TYPE_H
#define PID_POSITION_TYPE_H

/**
 * Define the type of Position which is detected by the PID
 */
enum DetectedMotionType {
    /** No position to reach. */
    DETECTED_MOTION_TYPE_NO_POSITION_TO_REACH = 0,
    
    /** No position to reach, but a position to maintain. */
    DETECTED_MOTION_TYPE_POSITION_TO_MAINTAIN = 1,
    
    /** The trajectory is in progress. */
    DETECTED_MOTION_TYPE_POSITION_IN_PROGRESS = 2,
    
    /** The robot reach the position. */
    DETECTED_MOTION_TYPE_POSITION_REACHED = 3,
    
    /** The robot is directly blocked to move. */
    DETECTED_MOTION_TYPE_POSITION_BLOCKED_WHEELS = 4,
    
    /** The robot will hurt something (detection by RobotDetector). */
    DETECTED_MOTION_TYPE_POSITION_OBSTACLE = 5
};

#endif
