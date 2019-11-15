#ifndef PSCONSTATNS
#define PSCONSTATNS 1

// Time constants in microseconds
const double ORTHO_LIFETIME = 0.142;
const double PARA_LIFETIME = 0.000125;

// Ortho-Para fraction
const double OPS2PPS = 1.0;
// double OPS2PPS = 0.289;

// Electron capture ratio
const double ELECAP = 0.0;
// double ELECAP = 0.0964;

// length of a timeslot [microseconds]
const double TIMESLOT = 500.0;

// Positronium formation percentage
const double PS_FORMATION = 1.0;

// Reconstruction efficiencies
const double EFF_ONE_GAMMA = 1.0;
const double EFF_TWO_GAMMA = 1.0;
const double EFF_THREE_GAMMA = 1.0;

// Activity range in MBq
const double ACT_MIN = 0.1;
const double ACT_MAX = 15.0;

// Calculate for such number of steps, also number of bins
const int INTERVALS = 100;
// Repeat event generation N times to get some average
const int REPEAT = 100;

#endif /* !PSCONSTATNS */
