//
// Created by Alessandro Salo on 22.11.2024.
//
#include <cmath>
#include "uniform_sampler.hh"

void uniform::initialize(unsigned long long j) {
    u = j ^ v; int64();
    v = u; int64();
    w = v; int64();
}

uniform::uniform(unsigned long long j) : v(4101842887655102017LL), w(1), minimum(0.0), maximum(1.0) {
    // Constructor. Call with any integer seed (except value of v above).
    initialize(j);
}

uniform::uniform(unsigned long long j, float a, float b) : v(4101842887655102017LL), w(1), minimum(a), maximum(b) {
    // Constructor. Call with any integer seed (except value of v above).
    initialize(j);
}

unsigned long long uniform::int64() {
    // Return 64-bit random integer. See text for explanation of method.
    u = u * 2862933555777941757LL + 7046029254386353087LL;
    v ^= v >> 17; v ^= v << 31; v ^= v >> 8;
    w = 4294957665U*(w & 0xffffffff) + (w >> 32);
    unsigned long long x = u ^ (u << 21); x ^= x >> 35; x ^= x << 4;
    return (x + v) ^ w;
}

double uniform::sample() { return minimum + (maximum - minimum) * (5.42101086242752217E-20 * int64()); }
// Return random double-precision floating value in the range 0. to 1.

