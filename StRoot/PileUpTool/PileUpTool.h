#ifndef __CENT_TOOL_HEADER__
#define __CENT_TOOL_HEADER__

#include <iostream>
#include "TF1.h"
#include "TRandom3.h"

class PileUpTool {

    private:
        TF1* PileUpCurve[3][2]; // [FxtMult3_DCA3, TnMIP, TofMult3][upper, lower]

    public:
        PileUpTool();
        ~PileUpTool(){}

        // -------------------------------------------------------------------
        void ReadParams();
        bool IsPileUp(int FXTMult3_DCA1, int FXTMult3_DCA3, double TnMIP, int TofMult3);
};

#endif