#include <iostream>
#include "TMath.h"
#include "TF1.h"
#include "StPicoEvent/StPicoDst.h"
#include "StPicoEvent/StPicoEvent.h"
#include "StPicoEvent/StPicoTrack.h"
#include "StPicoDstMaker/StPicoDstMaker.h"
#include "StThreeVectorF.hh"

#include "MeanDcaTool.h"
#include "MeanDcaParams.h"

MeanDcaTool::MeanDcaTool() {
    funcUpperZ = new TF1("z_upper", "[0]+[1]/pow(x, [2])", 1, 300);
    funcUpperXY = new TF1("xy_upper", "[0]+[1]/pow(x, [2])", 1, 300);
    funcLowerZ = new TF1("z_lower", "[0]+[1]/pow(x, [2])", 1, 300);
    funcLowerXY = new TF1("xy_lower", "[0]+[1]/pow(x, [2])", 1, 300);
    SetUpperCurveParZ(1.0, 1.0, 2.0);
    SetUpperCurveParXY(1.0, 1.0, 2.0);
    SetLowerCurveParZ(1.0, -1.0, 2.0);
    SetLowerCurveParXY(1.0, -1.0, 2.0);
    clean();
}

void MeanDcaTool::clean() {
    haveCache = false;
    mDCAz = 0;
    mDCAxy = 0;
    sDCAz = 0;
    sDCAxy = 0;
    vDCAz.clear();
    vDCAxy.clear();
}

void MeanDcaTool::SetUpperCurveParZ(double p0, double p1, double p2) {
    funcUpperZ->SetParameters(p0, p1, p2);
}
void MeanDcaTool::SetUpperCurveParXY(double p0, double p1, double p2) {
    funcUpperXY->SetParameters(p0, p1, p2);
}
void MeanDcaTool::SetLowerCurveParZ(double p0, double p1, double p2) {
    funcLowerZ->SetParameters(p0, p1, p2);
}
void MeanDcaTool::SetLowerCurveParXY(double p0, double p1, double p2) {
    funcLowerXY->SetParameters(p0, p1, p2);
}

void MeanDcaTool::ReadParams() {
    std::cout << "[LOG] - From MeanDcaTool: Current data set shoule be: [" << DcaPars::dstName << "]\n";
    std::cout << "[LOG] - From MeanDcaTool: Now reading mean DCA parameters:\n";
    std::cout << "\t [DCAz] upper curve parameters: {" << DcaPars::DcaZUpperPars[0] << ", " << DcaPars::DcaZUpperPars[1] << ", " << DcaPars::DcaZUpperPars[2] << "}\n";
    std::cout << "\t [DCAz] lower curve parameters: {" << DcaPars::DcaZLowerPars[0] << ", " << DcaPars::DcaZLowerPars[1] << ", " << DcaPars::DcaZLowerPars[2] << "}\n";
    std::cout << "\t [sDCAxy] upper curve parameters: {" << DcaPars::DcaXYUpperPars[0] << ", " << DcaPars::DcaXYUpperPars[1] << ", " << DcaPars::DcaXYUpperPars[2] << "}\n";
    std::cout << "\t [sDCAxy] lower curve parameters: {" << DcaPars::DcaXYLowerPars[0] << ", " << DcaPars::DcaXYLowerPars[1] << ", " << DcaPars::DcaXYLowerPars[2] << "}\n";
    SetUpperCurveParZ(
        DcaPars::DcaZUpperPars[0],
        DcaPars::DcaZUpperPars[1],
        DcaPars::DcaZUpperPars[2]
    );
    SetLowerCurveParZ(
        DcaPars::DcaZLowerPars[0],
        DcaPars::DcaZLowerPars[1],
        DcaPars::DcaZLowerPars[2]
    );
    SetUpperCurveParXY(
        DcaPars::DcaXYUpperPars[0],
        DcaPars::DcaXYUpperPars[1],
        DcaPars::DcaXYUpperPars[2]
    );
    SetLowerCurveParXY(
        DcaPars::DcaXYLowerPars[0],
        DcaPars::DcaXYLowerPars[1],
        DcaPars::DcaXYLowerPars[2]
    );
}

bool MeanDcaTool::Make(StPicoDst *pico) {
    int nTracks = pico->numberOfTracks();

    clean();
    StPicoEvent* event = (StPicoEvent*)pico->event();
    if (!event) { return false; }

    TVector3 vertex = event->primaryVertex();
    double vx = vertex.X();
    double vy = vertex.Y();
    double vz = vertex.Z();
    double mField = event->bField();

    for (int i=0; i<nTracks; i++) {
        StPicoTrack* track = (StPicoTrack*)pico->track(i);
        if (!track) { continue; }
        if (!track->isPrimary()) { continue; }
        vDCAz.push_back(track->gDCAz(vz));
        vDCAxy.push_back(track->helix(mField).geometricSignedDistance(vx, vy));
    }

    if (vDCAz.size() == 0) { return false; }

    for (auto const& x : vDCAz) { mDCAz += x; }
    mDCAz = mDCAz / vDCAxy.size();
    for (auto const& x : vDCAz) { sDCAz += (x - mDCAz) * (x - mDCAz); }
    sDCAz = TMath::Sqrt(sDCAz / vDCAz.size());
    for (auto const& x : vDCAxy) { mDCAxy += x; }
    mDCAxy = mDCAxy / vDCAxy.size();
    for (auto const& x : vDCAxy) { sDCAxy += (x - mDCAxy) * (x - mDCAxy); }
    sDCAxy = TMath::Sqrt(sDCAxy / vDCAxy.size());

    haveCache = true;

    return true;
}

bool MeanDcaTool::IsBadMeanDcaZEvent(StPicoDst* pico) {
    bool res = true;
    if (!haveCache) { res = Make(pico); }
    if (!res) { return true; }
    int refMult = pico->event()->refMult();
    if (funcUpperZ->Eval(refMult) < mDCAz || funcLowerZ->Eval(refMult) > mDCAz) { return true; }
    return false;
}

bool MeanDcaTool::IsBadMeanDcaXYEvent(StPicoDst* pico) {
    bool res = true;
    if (!haveCache) { res = Make(pico); }
    if (!res) { return true; }
    int refMult = pico->event()->refMult();
    if (funcUpperXY->Eval(refMult) < mDCAxy || funcLowerXY->Eval(refMult) > mDCAxy) { return true; }
    return false;
}
