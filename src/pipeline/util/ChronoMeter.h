//
// Created by Jonathan on 11/05/2021.
//

#ifndef INC_3D_RENDERER_CHRONOMETER_H
#define INC_3D_RENDERER_CHRONOMETER_H
#define CHRONO_OFF

/**
 * Functions to measure performance
 */


void start_chrono(int index);

void stop_chrono(int index);

long getAverageMilliseconds(int index);

long getAverageMicroseconds(int index);

long getAverageNanoseconds(int index);

#endif //INC_3D_RENDERER_CHRONOMETER_H
