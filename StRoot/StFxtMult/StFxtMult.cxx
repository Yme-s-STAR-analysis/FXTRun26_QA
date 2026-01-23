#include "StFxtMult.h"
#include "TMath.h"

#include "TpcShiftTool/TpcShiftTool.h"

StFxtMult::StFxtMult() {
    clean();
    mtShift = nullptr;
}

void StFxtMult::clean() {
    mFXTMult_DCA1 = 0;
    mFXTMult_DCA3 = 0;
    mFXTMult3_DCA1 = 0;
    mFXTMult3_DCA3 = 0;
    mTofMult = 0;
    mTofMult3 = 0;
    mTofMatch = 0;
    mBTofMatch = 0;
    mEpdTnMip = 0;
}

void StFxtMult::ImportShiftTool(TpcShiftTool* shift_ptr) {
    mtShift = shift_ptr;
}

void StFxtMult::IgnoreShift() {
    mtShift = nullptr;
}

bool StFxtMult::make(StPicoDst *picoDst) {
    clean();
    Int_t nTracks = picoDst->numberOfTracks();

    StPicoEvent* picoEvent = (StPicoEvent*)picoDst->event();
    if (!picoEvent) { return false; }

    Int_t runId = picoEvent->runId();
    TVector3 vertex = picoEvent->primaryVertex();
    const Float_t mField = picoEvent->bField();

    for (Int_t iTrack=0; iTrack<nTracks; iTrack++) {
        StPicoTrack* picoTrack = (StPicoTrack *)picoDst->track(iTrack);
        if (!picoTrack || !picoTrack->isPrimary()) { continue; }

        Int_t nHitsFit = picoTrack->nHitsFit();
        Int_t nHitsPoss = picoTrack->nHitsPoss();
        auto nHitsRatio = nHitsFit * 1.0 / nHitsPoss;
        if (nHitsRatio < 0.51) { continue; }
        // if (nHitsFit <= 10) { continue; } // nHitsRatio cut is applied, but nHitsFit is NOT 

        auto q = picoTrack->charge();
        // nHitsDedx cut is NOT applied in FXT runs
        // Int_t nHitsDedx = picoTrack->nHitsDedx();
        // if (nHitsDedx <= 5) { continue; }

        // Double_t dca = fabs(picoTrack->gDCA(vx, vy, vz));
        StPicoPhysicalHelix helix = picoTrack->helix(mField);
        Double_t dca = fabs(helix.geometricSignedDistance(vertex));
        if (dca > 3) { continue; }

        TVector3 pmomentum = picoTrack->pMom();
        Double_t pcm = pmomentum.Mag();
        Double_t pt = pmomentum.Perp();

        if (pcm < 1e-10) { continue; }
        Double_t eta = pmomentum.PseudoRapidity();
        Double_t nsig = picoTrack->nSigmaProton();
        if (mtShift) { // apply n sigma shift
            // nsig -= mtShift->GetShift(runId, pt, eta);
            nsig -= mtShift->GetShift();
        }

        if (1) { // might be used, just store them
            mFXTMult_DCA3 += 1;
            if (dca < 1) { mFXTMult_DCA1 += 1; }
        }

        if ((q < 0) || (nsig < -3 && pcm < 2)) {
            mFXTMult3_DCA3 += 1;
            if (dca < 1) { mFXTMult3_DCA1 += 1; }
        }

        // bTOF and eTOF track check
        Int_t bTofId = picoTrack->bTofPidTraitsIndex();
        Int_t eTofId = picoTrack->eTofPidTraitsIndex();
        bool isBTof = picoTrack->isTofTrack();
        bool isETof = picoTrack->isETofTrack();
        Double_t bTofBeta = -999;
        Double_t eTofBeta = -999;
        Double_t bTofMass2 = -999;
        Double_t eTofMass2 = -999;
        if (isBTof && bTofId >= 0) {
            StPicoBTofPidTraits* bPidTraits = picoDst->btofPidTraits(bTofId);
            if (bPidTraits->btofMatchFlag() > 0) {
                bTofBeta = bPidTraits->btofBeta();
                if (bTofBeta > 0.1) { mTofMatch += 1; }
                if (bTofBeta > 0.05 &&
                    fabs(bPidTraits->btofYLocal()) < 1.6 &&
                    fabs(bPidTraits->btofZLocal()) < 3.0) {
                      bTofMass2 = pcm * pcm * (pow(bTofBeta, -2) - 1);
                }
            }
        }
        if (isETof && eTofId >= 0) {
            StPicoETofPidTraits* ePidTraits = picoDst->etofPidTraits(eTofId);
            StPicoETofHit* eTofHit = picoDst->etofHit(eTofId);
            Int_t clusterSize = eTofHit->clusterSize();
            Int_t hitId = ePidTraits->hitIndex();
            auto eTofId8 = GetETofId8(eTofHit);
            bool isMask = IsETofMask(); // will change this when we need to
            if (!isMask) {} // do nothing for now
            if (hitId >= 0) {
                eTofBeta = ePidTraits->beta();
                if (eTofBeta > 0.1) { mTofMatch += 1; }
                if (eTofBeta > 0.05 && 
                    fabs(ePidTraits->deltaX()) < 5 && 
                    fabs(ePidTraits->deltaY()) < 10 && 
                    clusterSize < 100) {
                        eTofMass2 = pcm * pcm * (pow(eTofBeta, -2) - 1);
                    }
            }
        }
        
        //
        if (bTofMass2 < 0.6 && bTofMass2 > 0.0) { mTofMult3 += 1; }
        if (eTofMass2 < 0.6 && eTofMass2 > 0.0) { mTofMult3 += 1; }
    }

    // EPD TnMIP
    Int_t numberOfEpdHits = picoDst->numberOfEpdHits();
    for (Int_t iHit=0; iHit<numberOfEpdHits; iHit++) {
        auto epdHit = picoDst->epdHit(iHit);
        if (!epdHit) { continue; }
        if (!epdHit->isGood()) { continue; }
        if (epdHit->side() != -1) { continue; } // drop out west side
        mEpdTnMip += epdHit->TnMIP(4.0, 0.3);
    }

    mTofMult = picoEvent->btofTrayMultiplicity();
    mBTofMatch = picoEvent->nBTOFMatch();
    return true;
}

Int_t StFxtMult::GetETofId8(StPicoETofHit* eTofHit){
    int iGet4 = 1;
    double lx = eTofHit->localX();
    double ly = eTofHit->localY();
    if(lx < -12 && lx >= -16) { iGet4 = 1; }
    else if(lx < -8 && lx >= -12) { iGet4 = 2; }
    else if(lx < -4 && lx >= -8) { iGet4 = 3; }
    else if(lx < 0 && lx >= -4) { iGet4 = 4; }
    else if(lx < 4 && lx >= 0) { iGet4 = 5; }
    else if(lx < 8 && lx >= 4) { iGet4 = 6; }
    else if(lx < 12 && lx >= 8) { iGet4 = 7; }
    else if(lx < 16 && lx >= 12) { iGet4 = 8; }
    return 3*3*8*(eTofHit->sector()-13)+ 3*8*(eTofHit->zPlane() -1) + 8*(eTofHit->counter() -1) + iGet4-1;
}