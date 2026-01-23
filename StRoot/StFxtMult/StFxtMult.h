#ifndef __STFXTMULT__
#define __STFXTMULT__

#include <limits>
#include "TVector3.h"
#include "StPicoEvent/StPicoDst.h"
#include "StPicoEvent/StPicoEvent.h"
#include "StPicoEvent/StPicoTrack.h"
#include "StPicoEvent/StPicoPhysicalHelix.h"
#include "StPicoDstMaker/StPicoDstMaker.h"
#include "StThreeVectorF.hh"
#include "StPicoEvent/StPicoBTofHit.h"
#include "StPicoEvent/StPicoETofHit.h"
#include "StPicoEvent/StPicoBTofPidTraits.h"
#include "StPicoEvent/StPicoETofPidTraits.h"
#include "StBTofUtil/tofPathLength.hh"
#include "StEpdUtil/StEpdGeom.h"
#include "StPicoEvent/StPicoEpdHit.h"
#include "phys_constants.h"

class TpcShiftTool;

class StFxtMult {
    public:

        Int_t mFXTMult_DCA1;
        Int_t mFXTMult_DCA3;
        Int_t mFXTMult3_DCA1;
        Int_t mFXTMult3_DCA3;
        Int_t mTofMult;
        Int_t mTofMult3;
        Int_t mTofMatch; // bTOF + eTOF
        Int_t mBTofMatch; // bTOF, from Pico
        Double_t mEpdTnMip; // truncated nMIP

        TpcShiftTool* mtShift;

        StFxtMult();
        ~StFxtMult(){}

        void clean();
        
        void ImportShiftTool(TpcShiftTool* shift_ptr);
        void IgnoreShift();
        bool make(StPicoDst *picoDst);
        Int_t GetETofId8(StPicoETofHit* eTofHit);
        bool IsETofMask() { return false; } // currently we do not mask any
};

#endif
