#ifndef BINARY_PRINT_WRITER_H
#define BINARY_PRINT_WRITER_H

#include "outputStream.h"

// --- Management of bin conversion

/**
 * Appends a binary value, group by xx bits separated by space (for more visibility)
 * @param outputStream the outputStream in which we write the binary value
 * @param value a 32 bit value
 * @param groupBy the number of bits grouped and separated by space
 * @return the number of char written
 */
unsigned int appendBinary32(OutputStream* outputStream,
        unsigned long value,
        unsigned groupBy);

/**
 * Appends a binary value, group by xx bits separated by space (for more visibility)
 * @param outputStream the outputStream in which we write the binary value
 * @param value a 16 bit value
 * @param groupBy the number of bits grouped and separated by space
 * @return the number of char written
 */
unsigned int appendBinary16(OutputStream* outputStream,
        unsigned int value,
        unsigned groupBy);

/**
 * Appends a binary value, group by xx bits separated by space (for more visibility)
 * @param outputStream the outputStream in which we write the binary value 
 * @param value a 8 bit value
 * @param groupBy the number of bits grouped and separated by space
 * @return the number of char written
 */
unsigned int appendBinary8(OutputStream* outputStream,
        unsigned char value,
        unsigned groupBy);

#endif

