#ifndef __YQAMAKER__
#define __YQAMAKER__

#include <iostream>
#include <vector>
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TProfile.h"
#include "TString.h"

#include "StChain/StMaker.h"
#include "StPicoDstMaker/StPicoDstMaker.h"
#include "StPicoEvent/StPicoDst.h"
#include "StPicoEvent/StPicoEvent.h"
#include "StPicoEvent/StPicoTrack.h"
#include "StPicoEvent/StPicoBTofPidTraits.h"
#include "StEpdUtil/StEpdGeom.h"
#include "StPicoEvent/StPicoEpdHit.h"
#include "StPicoEvent/StPicoETofPidTraits.h"

#include "StRoot/StFxtMult/StFxtMult.h"

class TH1D;
class TH1F;
class TH2F;
class TProfile;

class StYQAMaker : public StMaker {
    public:
        StYQAMaker(const char* name, StPicoDstMaker* picoMaker, const char* outName);
        virtual ~StYQAMaker();
        virtual Int_t Init();
        virtual void Clear(Option_t* option = "");
        virtual Int_t Finish();
        
        Int_t Make();
        Int_t MakeTrack(const Int_t iTrack);
        Bool_t IsGoodTrigger();

        Int_t MakeEpdHits(const Int_t iHit);
        Double_t GetBTofBeta();
        Double_t GetETofBeta();

    private:

        TString mOutName;
        
        StPicoDstMaker* mPicoDstMaker;
        StPicoDst* mPicoDst;
        StPicoEvent* mPicoEvent;
        StPicoTrack* mPicoTrack;

        StEpdGeom* mEpdGeom;
        StPicoEpdHit* mEpdHit;
        StPicoBTofPidTraits* mBTofPidTraits;
        StPicoETofPidTraits* mETofPidTraits;

        StFxtMult* mtMult;
        
        Int_t mRunId;
        Int_t mRefMult;
        Int_t mRefMult3;
        Double_t mBField;
        Double_t vx, vy, vz;

        Int_t mNEpdHitsEast;
        Int_t mNEpdHitsWest;
        
        // those track-wise quantities will be collected for each event
        // and calculate their mean (also sigma for some of them) to fill TProfiles
        std::vector<Double_t> vpt;
        std::vector<Double_t> veta;
        std::vector<Double_t> vphi;
        std::vector<Double_t> vsdca;
        std::vector<Double_t> vnhitsdedx;
        std::vector<Double_t> vnhitsfit;
        std::vector<Double_t> vsdcaxy;
        std::vector<Double_t> vsdcaz;
        
        // Histograms
        // Event-wise
        TH1D* hNev;
        TH2F* h2VxVy;
        TH2F* h2VxVyVrCut;
        TH1F* hTpcVz;
        TH1F* hTpcVzVrCut;
        TH1F* hVpdVz;
        TH2F* h2VzVpd;
        TH1F* hNVpdHitsEast;
        TH1F* hNVpdHitsWest;
        TH1F* hBbcX;
        TH1F* hZdcX;
        TH1F* hRefMult;
        TH1F* hRefMult3;

        // Multiplicities from StFxtMult
        TH1F* hFXTMult_DCA1;
        TH1F* hFXTMult_DCA3;
        TH1F* hFXTMult3_DCA1;
        TH1F* hFXTMult3_DCA3;
        TH1F* hTofMult;
        TH1F* hTofMult3;
        TH1F* hTofMatch;
        TH1F* hBTofMatch;
        TH1F* hEpdTnMip;
        // pile-up
        TH2F* h2FXTMult3DCA11FXTMult3DCA3;
        TH2F* h2FXTMult3DCA1TofMult3;
        TH2F* h2FXTMult3DCA1EpdTnMip;
        
        // mean DCAxyz
        TH2F* h2FXTMultDCA3sDCAxy;
        TH2F* h2FXTMultDCA3sDCAz;

        TH1F* hNBTofHits;
        TH1F* hNEpdHitsEast;
        TH1F* hNEpdHitsWest;
        TH1F* hNETofDigis;
        TH1F* hNETofHits;

        // Track-wise
        TH1F* hNHitsFit;
        TH1F* hNHitsFitRatioCut;
        TH1F* hNHitsDedx;
        TH1F* hNHitsDedxRatioCut;
        TH1F* hNHitsRatio;
        TH1F* hDca;
        TH1F* hDcaZ;
        TH1F* hsDcaXY;
        TH1F* hPt;
        TH1F* hEta;
        TH1F* hPhi;
        TH2F* h2RigiVsDedx;
        TH2F* h2RigiVsNSigmaProton;
        TH2F* h2RigiVsNSigmaPion;
        TH2F* h2RigiVsNSigmaKaon;
        TH2F* h2RigiVsBTof1OverBeta;
        TH2F* h2RigiVsBTofMass2;
        TH2F* h2BTofMass2VsNSigmaProton;
        TH2F* h2ProtonPtY;
        TH2F* h2ProtonPtYbTOF;

        TH2F* h2EtaNHitsFit;
        TH2F* h2EtaNHitsDedx;
        TH2F* h2EtaNHitsRatio;
        TH2F* h2PhiNHitsFit;
        TH2F* h2PhiNHitsDedx;
        TH2F* h2PhiNHitsRatio;
        
        TH1F* hNEpdMipEast;
        TH1F* hNEpdMipWest;
        TH1F* hNEpdTMipEast;
        TH1F* hNEpdTMipWest;
    
        TH2F* h2RigiVsETof1OverBeta;
        TH2F* h2RigiVsETofMass2;
        TH2F* h2ETofMass2VsNSigmaProton;
        TH2F* h2ProtonPtYeTOF;

        // Profiles
        // Event-wise
        TProfile* pRunVsVz;
        TProfile* pRunVsVr;

        TProfile* pRunVsRefMult;
        TProfile* pRunVsRefMult3;
        
        TProfile* pRunVsNEpdHitsEast;
        TProfile* pRunVsNEpdHitsWest;

        TProfile* pRunvsNETofHits;
        TProfile* pRunvsNETofDigi;

        TProfile* pRunVsFXTMult_DCA1;
        TProfile* pRunVsFXTMult_DCA3;
        TProfile* pRunVsFXTMult3_DCA1;
        TProfile* pRunVsFXTMult3_DCA3;
        TProfile* pRunVsTofMult;
        TProfile* pRunVsTofMult3;
        TProfile* pRunVsEpdTnMip;
        TProfile* pRunVsTofMatch; // this one includes both btof and etof
        TProfile* pRunVsBTofMatch; // this one comes from bTof
        
        // Track-wise
        TProfile* pRunVsDca;
        TProfile* pRunVsDcaZ;
        TProfile* pRunVssDcaXY;
        TProfile* pRunVsDcaZStd;
        TProfile* pRunVssDcaXYStd;
        TProfile* pRunVsNHitsFit;
        TProfile* pRunVsNHitsDedx;
        TProfile* pRunVsPt;
        TProfile* pRunVsEta;
        TProfile* pRunVsPhi;

        ClassDef(StYQAMaker,1)
};

#endif