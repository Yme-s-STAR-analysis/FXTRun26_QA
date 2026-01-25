#ifndef __PUTOOL_CONFXXX__
#define __PUTOOL_CONFXXX__

namespace PUConf {
    // The name and mode are used to check in the LOG file if the wrong parameter file is used.
    static const char* Name = "4p5";
    static const char* Mode = 
        // "TEST";
        // "RAW";
        "PU";

    // Pile-up parameters
    // fitting function: [0] + [1] / pow(x, [2])
    // Note: when par [2] is close to -1 -> which means this is indeed approximate a linear function
    static constexpr double FXTMULTDCA3_UPars[3] = {
        6.11603, 4.44923, -0.821991
    };
    static constexpr double FXTMULTDCA3_LPars[3] = {
        -5.30187, 0.913069, -1.08195
    };
    static constexpr double TNMIP_UPars[3] = {
        -6452.36, 6658.89, -0.004758291
    };
    static constexpr double TNMIP_LPars[3] = {
        -6550.6, 6245.39, -0.018077
    };
    static constexpr double TOFMULT3_Upars[3] = {
        9999, 0, 0
    };
    static constexpr double TOFMULT3_Lpars[3] = {
        -1, 0, 0
    };

}

#endif
