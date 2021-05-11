//
// Created by Jonathan on 11/05/2021.
//
#include <array>
#include "ChronoMeter.h"
#include <ctime>

#ifdef CHRONO_ON
    std::array<clock_t, 16> CHRONO_ARRAY{};
    std::array<long, 16> AVERAGES{0};
    std::array<long, 16> COUNTS{0};

    void start_chrono(int index) {
        CHRONO_ARRAY[index] = clock();
    }
    void stop_chrono(int index) {
        clock_t now = clock();
        AVERAGES[index] += (now - CHRONO_ARRAY[index]);
        COUNTS[index] += 1;
    }

    long getAverageMilliseconds(int index) {
        double average = ((double) AVERAGES[index]) / COUNTS[index];
        double seconds = average/CLOCKS_PER_SEC;
        return (long) round(seconds * 1000);
    }

    long getAverageMicroseconds(int index) {
        double average = ((double) AVERAGES[index]) / COUNTS[index];
        double seconds = average/CLOCKS_PER_SEC;
        return (long) round(seconds * 1000000);
    }

    long getAverageNanoseconds(int index) {
        double average = ((double) AVERAGES[index]) / COUNTS[index];
        double seconds = average/CLOCKS_PER_SEC;
        return (long) round(seconds * 1000000000);
    }
#else
void start_chrono(int index){}
void stop_chrono(int index){}
long getAverageMilliseconds(int index){return 0;}
long getAverageMicroseconds(int index){return 0;}
long getAverageNanoseconds(int index){return 0;}
#endif