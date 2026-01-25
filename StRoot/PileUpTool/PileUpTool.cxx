#include "PileUpPars.h"
#include "PileUpTool.h"

PileUpTool::PileUpTool() {
    PileUpCurve[0][0] = new TF1("IndianTofMultUpper", "[0]+[1]/pow(x, [2])", 0, 300);
    PileUpCurve[0][1] = new TF1("IndianTofMultLower", "[0]+[1]/pow(x, [2])", 0, 300);
    PileUpCurve[1][0] = new TF1("IndianTofMatchUpper", "[0]+[1]/pow(x, [2])", 0, 300);
    PileUpCurve[1][1] = new TF1("IndianTofMatchLower", "[0]+[1]/pow(x, [2])", 0, 300);
    PileUpCurve[2][0] = new TF1("IndianTofBetaUpper", "[0]+[1]/pow(x, [2])", 0, 300);
    PileUpCurve[2][1] = new TF1("IndianTofBetaLower", "[0]+[1]/pow(x, [2])", 0, 300);
}

void PileUpTool::ReadParams() {
    std::cout << "[LOG] - PileUpTool: The parameter list is named as [" << PUConf::Name << "] version [" << PUConf::Mode << "]." << std::endl;
    for (int i=0; i<3; i++) {
        PileUpCurve[0][0]->SetParameter(i, PUConf::FXTMULTDCA3_UPars[i]);
        PileUpCurve[0][1]->SetParameter(i, PUConf::FXTMULTDCA3_LPars[i]);
        PileUpCurve[1][0]->SetParameter(i, PUConf::TNMIP_UPars[i]);
        PileUpCurve[1][1]->SetParameter(i, PUConf::TNMIP_LPars[i]);
        PileUpCurve[2][0]->SetParameter(i, PUConf::TOFMULT3_Upars[i]);
        PileUpCurve[2][1]->SetParameter(i, PUConf::TOFMULT3_Lpars[i]);
    }
}

bool PileUpTool::IsPileUp(int FXTMult3_DCA1, int FXTMult3_DCA3, double TnMIP, int TofMult3) {
    return PileUpCurve[0][0]->Eval(FXTMult3_DCA1) < FXTMult3_DCA3 ||
    PileUpCurve[0][1]->Eval(FXTMult3_DCA1) > FXTMult3_DCA3 ||
    PileUpCurve[1][0]->Eval(FXTMult3_DCA1) < TnMIP ||
    PileUpCurve[1][1]->Eval(FXTMult3_DCA1) > TnMIP ||
    PileUpCurve[2][0]->Eval(FXTMult3_DCA1) < TofMult3 ||
    PileUpCurve[2][1]->Eval(FXTMult3_DCA1) > TofMult3;
}