#ifndef __YQAMAKER__
#define __YQAMAKER__

#include <iostream>
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TProfile.h"
#include "TString.h"

/*

    Define some detector macros here

*/

#define __WITH_EPD__ 

#define __WITH_ETOF__ 


#include "StChain/StMaker.h"

#include "StPicoDstMaker/StPicoDstMaker.h"
#include "StPicoEvent/StPicoDst.h"
#include "StPicoEvent/StPicoEvent.h"
#include "StPicoEvent/StPicoTrack.h"

// always use bTOF
#include "StPicoEvent/StPicoBTofPidTraits.h"

#ifdef __WITH_EPD__
    #include "StEpdUtil/StEpdGeom.h"
    #include "StPicoEvent/StPicoEpdHit.h"
#endif

#ifdef __WITH_ETOF__
    #include "StPicoEvent/StPicoETofPidTraits.h"
#endif

class StPicoDst;
class StPicoTrack;
class StPicoEvent;
class StPicoDstMaker;
class TH1D;
class TH1F;
class TH2F;
class TProfile;
class StPicoBTofPidTraits;
class StEpdGeom;
class StPicoEpdHit;
class StPicoETofPidTraits;

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

        Double_t GetBTofBeta();

        #ifdef __WITH_EPD__
            Int_t MakeEpdHits(const Int_t iHit);
        #endif

        #ifdef __WITH_ETOF__
            Double_t GetETofBeta();
        #endif

    private:

        TString mOutName;
        
        StPicoDstMaker* mPicoDstMaker;
        StPicoDst* mPicoDst;
        StPicoEvent* mPicoEvent;
        StPicoTrack* mPicoTrack;

        StPicoBTofPidTraits* mBTofPidTraits;

        #ifdef __WITH_EPD__
	        StEpdGeom* mEpdGeom;
            StPicoEpdHit* mEpdHit;
        #endif

        #ifdef __WITH_ETOF__
            StPicoETofPidTraits* mETofPidTraits;
        #endif
        
        Int_t mRunId;
        Int_t mRefMult;
        Int_t mRefMult3;
        Double_t mBField;
        Double_t vx, vy, vz;

        #ifdef __WITH_EPD__
            Int_t mNEpdHitsEast;
            Int_t mNEpdHitsWest;
        #endif
        
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

        TH1F* hNBTofHits;
        TH1F* hNBTofMatch;

        #ifdef __WITH_EPD__
            TH1F* hNEpdHitsEast;
            TH1F* hNEpdHitsWest;
        #endif

        #ifdef __WITH_ETOF__
            TH1F* hNETofDigis;
            TH1F* hNETofHits;
        #endif

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
        
        #ifdef __WITH_EPD__
            TH1F* hNEpdMipEast;
            TH1F* hNEpdMipWest;
        #endif

        #ifdef __WITH_ETOF__
            TH2F* h2RigiVsETof1OverBeta;
            TH2F* h2RigiVsETofMass2;
            TH2F* h2ETofMass2VsNSigmaProton;
            TH2F* h2ProtonPtYeTOF;
        #endif

        // Profiles

        // Event-wise

        TProfile* pRunVsVx;
        TProfile* pRunVsVy;
        TProfile* pRunVsVz;
        TProfile* pRunVsVr;
        TProfile* pRunVsBbcX;
        TProfile* pRunVsZdcX;
        TProfile* pRunvsNBTofHits;
        TProfile* pRunvsNBTofMatch;

        TProfile* pRunVsRefMult;
        TProfile* pRunVsRefMult3;
        
        #ifdef __WITH_EPD__
            TProfile* pRunVsNEpdHitsEast;
            TProfile* pRunVsNEpdHitsWest;
        #endif

        #ifdef __WITH_ETOF__
            TProfile* pRunvsNETofHits;
            TProfile* pRunvsNETofDigi;
        #endif

        // Track-wise

        TProfile* pRunVsNHitsFit;
        TProfile* pRunVsNHitsRatio;
        TProfile* pRunVsNHitsDedx;
        TProfile* pRunVsDca;
        TProfile* pRunVsDcaZ;
        TProfile* pRunVssDcaXY;
        TProfile* pRunVsPt;
        TProfile* pRunVsEta;
        TProfile* pRunVsPhi;
        TProfile* pRunVsBTof1OverBeta;
        
        #ifdef __WITH_EPD__
            TProfile* pRunVsNMipEast;
            TProfile* pRunVsNMipWest;
            TProfile* pRunVsTNMipEast;
            TProfile* pRunVsTNMipWest;
        #endif

        #ifdef __WITH_ETOF__
            TProfile* pRunVsETof1OverBeta;
        #endif

        ClassDef(StYQAMaker,1)
};

#endif

