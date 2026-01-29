#include "StYQAMaker.h"

#include "TMath.h"
#include "TVector3.h"

#include "phys_constants.h"
#include "StThreeVector.hh"

#include "StBTofUtil/tofPathLength.hh"
#include "TpcShiftTool/TpcShiftTool.h"

#include "DbConf.h"

static const double ybeam = 1.522;

ClassImp(StYQAMaker);

StYQAMaker::StYQAMaker(const char* name, StPicoDstMaker* picoMaker, const char* outName) : StMaker(name) {

    /*
        The construction function of this class.
        Do some of the initialization.
    */

    mPicoDstMaker = picoMaker;
    mPicoDst = nullptr;
    mPicoEvent = nullptr;
    mPicoTrack = nullptr;

    TString fileName = outName;
    mOutName = fileName + ".root";

    mEpdGeom = new StEpdGeom();
}

StYQAMaker::~StYQAMaker() {
    vpt.clear();
    veta.clear();
    vphi.clear();
    vsdca.clear();
    vnhitsdedx.clear();
    vnhitsfit.clear();
    vsdcaxy.clear();
    vsdcaz.clear();
        
    delete hNev; hNev = nullptr;
    delete h2VxVy; h2VxVy = nullptr;
    delete h2VxVyVrCut; h2VxVyVrCut = nullptr;
    delete hTpcVz; hTpcVz = nullptr;
    delete hTpcVzVrCut; hTpcVzVrCut = nullptr;
    delete hVpdVz; hVpdVz = nullptr;
    delete h2VzVpd; h2VzVpd = nullptr;
    delete hNVpdHitsEast; hNVpdHitsEast = nullptr;
    delete hNVpdHitsWest; hNVpdHitsWest = nullptr;
    delete hBbcX; hBbcX = nullptr;
    delete hZdcX; hZdcX = nullptr;
    delete hRefMult; hRefMult = nullptr;
    delete hRefMult3; hRefMult3 = nullptr;
    delete hFXTMult_DCA1; hFXTMult_DCA1 = nullptr;
    delete hFXTMult_DCA3; hFXTMult_DCA3 = nullptr;
    delete hFXTMult3_DCA1; hFXTMult3_DCA1 = nullptr;
    delete hFXTMult3_DCA3; hFXTMult3_DCA3 = nullptr;
    delete hTofMult; hTofMult = nullptr;
    delete hTofMult3; hTofMult3 = nullptr;
    delete hTofMatch; hTofMatch = nullptr;
    delete hBTofMatch; hBTofMatch = nullptr;
    delete hEpdTnMip; hEpdTnMip = nullptr;
    delete h2FXTMult3DCA11FXTMult3DCA3; h2FXTMult3DCA11FXTMult3DCA3 = nullptr;
    delete h2FXTMult3DCA1TofMult3; h2FXTMult3DCA1TofMult3 = nullptr;
    delete h2FXTMult3DCA1EpdTnMip; h2FXTMult3DCA1EpdTnMip = nullptr;
    delete h2FXTMultDCA3sDCAxy; h2FXTMultDCA3sDCAxy = nullptr;
    delete h2FXTMultDCA3sDCAz; h2FXTMultDCA3sDCAz = nullptr;
    delete hNBTofHits; hNBTofHits = nullptr;
    delete hNEpdHitsEast; hNEpdHitsEast = nullptr;
    delete hNEpdHitsWest; hNEpdHitsWest = nullptr;
    delete hNETofDigis; hNETofDigis = nullptr;
    delete hNETofHits; hNETofHits = nullptr;
    delete hNHitsFit; hNHitsFit = nullptr;
    delete hNHitsFitRatioCut; hNHitsFitRatioCut = nullptr;
    delete hNHitsDedx; hNHitsDedx = nullptr;
    delete hNHitsDedxRatioCut; hNHitsDedxRatioCut = nullptr;
    delete hNHitsRatio; hNHitsRatio = nullptr;
    delete hDca; hDca = nullptr;
    delete hDcaZ; hDcaZ = nullptr;
    delete hsDcaXY; hsDcaXY = nullptr;
    delete hPt; hPt = nullptr;
    delete hEta; hEta = nullptr;
    delete hPhi; hPhi = nullptr;
    delete h2RigiVsDedx; h2RigiVsDedx = nullptr;
    delete h2RigiVsNSigmaProton; h2RigiVsNSigmaProton = nullptr;
    delete h2RigiVsNSigmaPion; h2RigiVsNSigmaPion = nullptr;
    delete h2RigiVsNSigmaKaon; h2RigiVsNSigmaKaon = nullptr;
    delete h2RigiVsBTof1OverBeta; h2RigiVsBTof1OverBeta = nullptr;
    delete h2RigiVsBTofMass2; h2RigiVsBTofMass2 = nullptr;
    delete h2BTofMass2VsNSigmaProton; h2BTofMass2VsNSigmaProton = nullptr;
    delete h2ProtonPtY; h2ProtonPtY = nullptr;
    delete h2ProtonPtYbTOF; h2ProtonPtYbTOF = nullptr;
    delete h2EtaNHitsFit; h2EtaNHitsFit = nullptr;
    delete h2EtaNHitsDedx; h2EtaNHitsDedx = nullptr;
    delete h2EtaNHitsRatio; h2EtaNHitsRatio = nullptr;
    delete h2PhiNHitsFit; h2PhiNHitsFit = nullptr;
    delete h2PhiNHitsDedx; h2PhiNHitsDedx = nullptr;
    delete h2PhiNHitsRatio; h2PhiNHitsRatio = nullptr;
    delete hNEpdMipEast; hNEpdMipEast = nullptr;
    delete hNEpdMipWest; hNEpdMipWest = nullptr;
    delete hNEpdTMipEast; hNEpdTMipEast = nullptr;
    delete hNEpdTMipWest; hNEpdTMipWest = nullptr;
    delete h2RigiVsETof1OverBeta; h2RigiVsETof1OverBeta = nullptr;
    delete h2RigiVsETofMass2; h2RigiVsETofMass2 = nullptr;
    delete h2ETofMass2VsNSigmaProton; h2ETofMass2VsNSigmaProton = nullptr;
    delete h2ProtonPtYeTOF; h2ProtonPtYeTOF = nullptr;
    delete pRunVsVz; pRunVsVz = nullptr;
    delete pRunVsVr; pRunVsVr = nullptr;
    delete pRunVsRefMult; pRunVsRefMult = nullptr;
    delete pRunVsRefMult3; pRunVsRefMult3 = nullptr;
    delete pRunVsNEpdHitsEast; pRunVsNEpdHitsEast = nullptr;
    delete pRunVsNEpdHitsWest; pRunVsNEpdHitsWest = nullptr;
    delete pRunvsNETofHits; pRunvsNETofHits = nullptr;
    delete pRunvsNETofDigi; pRunvsNETofDigi = nullptr;
    delete pRunVsFXTMult_DCA1; pRunVsFXTMult_DCA1 = nullptr;
    delete pRunVsFXTMult_DCA3; pRunVsFXTMult_DCA3 = nullptr;
    delete pRunVsFXTMult3_DCA1; pRunVsFXTMult3_DCA1 = nullptr;
    delete pRunVsFXTMult3_DCA3; pRunVsFXTMult3_DCA3 = nullptr;
    delete pRunVsTofMult; pRunVsTofMult = nullptr;
    delete pRunVsTofMult3; pRunVsTofMult3 = nullptr;
    delete pRunVsEpdTnMip; pRunVsEpdTnMip = nullptr;
    delete pRunVsTofMatch; pRunVsTofMatch = nullptr;
    delete pRunVsBTofMatch; pRunVsBTofMatch = nullptr;
    delete pRunVsDca; pRunVsDca = nullptr;
    delete pRunVsDcaZ; pRunVsDcaZ = nullptr;
    delete pRunVssDcaXY; pRunVssDcaXY = nullptr;
    delete pRunVsDcaZStd; pRunVsDcaZStd = nullptr;
    delete pRunVssDcaXYStd; pRunVssDcaXYStd = nullptr;
    delete pRunVsNHitsFit; pRunVsNHitsFit = nullptr;
    delete pRunVsNHitsDedx; pRunVsNHitsDedx = nullptr;
    delete pRunVsPt; pRunVsPt = nullptr;
    delete pRunVsEta; pRunVsEta = nullptr;
    delete pRunVsPhi; pRunVsPhi = nullptr;

    #ifdef __RBRDEDXRIGI__
    for (auto& item : mapRBRh2RigiVsDedx) {
        delete item.second; item.second = nullptr;
    }
    #endif
}

Int_t StYQAMaker::Init() {
    
    /*
        Init function.
    */
   
    std::cout << "[LOG] - From Init: " << "Reading run index of this data base." << std::endl;
    const Int_t nRuns = DbConf::mRunIdxMap.size();
    std::cout << "[LOG] - From Init: " << "There will be " << nRuns << " runs, starting with " << DbConf::mRunIdxMap.begin()->first << ", ending at " << DbConf::mRunIdxMap.rbegin()->first << "." << std::endl;

    std::cout << "[LOG] - From Init: " << "Reading valid trigger ID(s) ." << std::endl;
    const Int_t nTrgs = DbConf::mTriggers.size();
    std::cout << "[LOG] - From Init: " << "There is/are " << nTrgs << " MB trigger(s): ";
    for (const UInt_t& trg :  DbConf::mTriggers) {
        std::cout << "\t" << trg;
    }
    std::cout << "." << std::endl;

    std::cout << "[LOG] - From Init: " << "Initializing Histograms." << std::endl;
    
    hNev = new TH1D("hNev", ";Events;Counts", 7, -0.5, 6.5);
    hNev->GetXaxis()->SetBinLabel(1, "A) Full"); // 0
    hNev->GetXaxis()->SetBinLabel(2, "B) Trg. Sel."); // 1
    hNev->GetXaxis()->SetBinLabel(3, "C) Vtx. Cut"); // 2
    hNev->GetXaxis()->SetBinLabel(4, "D) Run Sel."); // 3
    hNev->GetXaxis()->SetBinLabel(5, "E1) PU Rej."); // 4 
    hNev->GetXaxis()->SetBinLabel(6, "E2) <DCAxy/z> Cut"); // 5
    hNev->GetXaxis()->SetBinLabel(7, "E1+E2)"); // 6

    h2VxVy = new TH2F(
        "hVxVy", "TPC V_{x} v.s. V_{y} w/o V_{r} cut;V_{x} (cm);V_{y} (cm);Counts", 
        200, -6.0, 2.0,
        200, -6.0, 2.0
    );
    h2VxVyVrCut = new TH2F(
        "hVxVyVrCut", "TPC V_{x} v.s. V_{y} w/ V_{r} cut;V_{x} (cm);V_{y} (cm);Counts", 
        200, -6.0, 2.0,
        200, -6.0, 2.0
    );

    hTpcVz = new TH1F(
        "hTpcVz", "TPC V_{z} w/o V_{r} cut;V_{z} (cm);Counts", 
        100, 195, 205
    );
    hTpcVzVrCut = new TH1F(
        "hTpcVzVrCut", "TPC V_{z} w/ V_{r} cut;V_{z} (cm);Counts", 
        100, 195, 205
    );
    
    hVpdVz = new TH1F(
        "hVpdVz", "VPD V_{z};V_{z} (cm);Counts", 
        100, 195, 205
    );
    h2VzVpd = new TH2F(
        "h2VzVpd", "TPC V_{z} v.s. VPD V_{z};TPC V_{z} (cm);VPD V_{z} (cm);Counts", 
        100, 195, 205,
        100, 195, 205
    );
    hNVpdHitsEast = new TH1F(
        "hNVpdHitsEast", "VPD east hits;N VPD Hits (East);Counts", 
        50, -0.5, 49.5
    );
    hNVpdHitsWest = new TH1F(
        "hNVpdHitsWest", "VPD west hits;N VPD Hits (West);Counts", 
        50, -0.5, 49.5
    );

    hBbcX = new TH1F(
        "hBbcX", "BBC coincidence rate;BBCx (Hz);Counts", 
        500, -0.5, 499.5
    );
    hZdcX = new TH1F(
        "hZdcX", "ZDC coincidence rate;BBCx (Hz);Counts", 
        100, -0.5, 99.5
    );

    hNBTofHits = new TH1F(
        "hNBTofHits", "bTOF hits;N bTOF Hits;Counts", 
        600, -0.5, 599.5
    );

    hRefMult = new TH1F(
        "hRefMult", "RefMult (raw);RefMult;Counts",
        80, -0.5, 79.5
    );
    hRefMult3 = new TH1F(
        "hRefMult3", "RefMult3 (raw);RefMult3;Counts",
        130, -0.5, 129.5
    );

    hFXTMult_DCA1 = new TH1F(
        "hFXTMult_DCA1", "FXT Mult DCA1;FXTMult_DCA1;Counts", 
        190, -0.5, 189.5
    );
    hFXTMult_DCA3 = new TH1F(
        "hFXTMult_DCA3", "FXT Mult DCA3;FXTMult_DCA3;Counts", 
        280, -0.5, 279.5
    );
    hFXTMult3_DCA1 = new TH1F(
        "hFXTMult3_DCA1", "FXT Mult3 DCA1;FXTMult3_DCA1;Counts", 
        160, -0.5, 159.5 
    );
    hFXTMult3_DCA3 = new TH1F(
        "hFXTMult3_DCA3", "FXT Mult3 DCA3;FXTMult3_DCA3;Counts", 
        250, -0.5, 249.5
    );
    hTofMult = new TH1F(
        "hTofMult", "TOF Mult;TofMult;Counts", 
        550, -0.5, 549.5
    );
    hTofMult3 = new TH1F(
        "hTofMult3", "TOF Mult3;TofMult3;Counts", 
        60, -0.5, 59.5
    );
    hTofMatch = new TH1F(
        "hTofMatch", "TOF Match;n TofMatches;Counts", 
        120, -0.5, 119.5
    );
    hBTofMatch = new TH1F(
        "hBTofMatch", "bTOF Match;n bTofMatches;Counts", 
        200, -0.5, 199.5
    );
    hEpdTnMip = new TH1F(
        "hEpdTnMip", "EPD truncated nMIP;TnMIP;Counts", 
        650, -0.5, 649.5
    );

    // pile-up and mean-DCA related 2D histograms
    h2FXTMult3DCA11FXTMult3DCA3 = new TH2F(
        "h2FXTMult3DCA11FXTMult3DCA3", 
        "FXTMult3_DCA1 v.s. _DCA3;FXTMult3_DCA1;FXTMult3_DCA3;Counts", 
        160, -0.5, 159.5,
        250, -0.5, 249.5
    );
    h2FXTMult3DCA1TofMult3 = new TH2F(
        "h2FXTMult3DCA1TofMult3", 
        "FXTMult3_DCA1 v.s. TofMult3;FXTMult3_DCA1;TofMult3;Counts", 
        160, -0.5, 159.5,
        60, -0.5, 59.5
    );
    h2FXTMult3DCA1EpdTnMip = new TH2F(
        "h2FXTMult3DCA1EpdTnMip", 
        "FXTMult3_DCA1 v.s. EPD TnMIP;FXTMult3_DCA1;TnMIP;Counts", 
        160, -0.5, 159.5,
        650, -0.5, 649.5
    );

    h2FXTMultDCA3sDCAxy = new TH2F(
        "h2FXTMultDCA3sDCAxy", "FXTMult DCA3 v.s. sDCAxy;FXTMult DCA3;sDCAxy (cm);Counts", 
        280, -0.5, 279.5,
        400, -2.0, 2.0
    );
    h2FXTMultDCA3sDCAz = new TH2F(
        "h2FXTMultDCA3sDCAz", "FXTMult DCA3 v.s. DCAz;FXTMult DCA3;DCAz (cm);Counts", 
        280, -0.5, 279.5,
        400, -2.0, 2.0
    );

    hNEpdHitsEast = new TH1F(
        "hNEpdHitsEast", "EPD east hits;N EPD Hits (East);Counts", 
        600, -0.5, 599.5
    );
    hNEpdHitsWest = new TH1F(
        "hNEpdHitsWest", "EPD west hits;N EPD Hits (West);Counts", 
        600, -0.5, 599.5
    );

    hNETofDigis = new TH1F(
        "hNETofDigis", "eTOF digis;N eTOF Digi;Counts", 
        1600, -0.5, 1599.5
    );
    hNETofHits = new TH1F(
        "hNETofHits", "eTOF hits;N eTOF Hits;Counts", 
        800, -0.5, 799.5
    );
    
    hNHitsFit = new TH1F(
        "hNHitsFit", "nHitsFit;nHitsFit/q;Counts", 
        200, -100.5, 99.5
    );
    hNHitsFitRatioCut = new TH1F(
        "hNHitsFitRatioCut", "nHitsFit w/ nHitsRatio cut;nHitsFit/q;Counts", 
        200, -100.5, 99.5
    );
    
    hNHitsDedx = new TH1F(
        "hNHitsDedx", "nHitsDedx;nHitsDedx;Counts", 
        100, -0.5, 99.5
    );
    hNHitsDedxRatioCut = new TH1F(
        "hNHitsDedxRatioCut", "nHitsDedx w/ nHitsRatio cut;nHitsDedx;Counts", 
        100, -0.5, 99.5
    );

    hNHitsRatio = new TH1F(
        "hNHitsRatio", "nHitsRatio;nHitsRatio;Counts", 
        120, -0.1, 1.1
    );

    hDca = new TH1F(
        "hDca", "DCA;DCA (cm);Counts", 
        300, 0.0, 6.0
    );
    hDcaZ = new TH1F(
        "hDcaZ", "DCAz;DCAz (cm);Counts", 
        100, -5.0, 5.0
    );
    hsDcaXY = new TH1F(
        "hsDcaXY", "sDCAxy;sDCAxy (cm);Counts", 
        100, -5.0, 5.0
    );

    hPt = new TH1F(
        "hPt", "p_{T};p_{T} (GeV/c);Counts", 
        500, 0.0, 5.0
    );
    hEta = new TH1F(
        "hEta", "#eta;#eta;Counts", 
        260, -2.6, 0.0
    );
    hPhi = new TH1F(
        "hPhi", "#phi;#phi;Counts", 
        // 360, -TMath::Pi(), TMath::Pi()
        400, -4.0, 4.0
    );

    h2RigiVsDedx = new TH2F(
        "h2RigiVsDedx", "Rigidity v.s. dE/dx;p/q (GeV/c);dE/dx (keV/cm);Counts", 
        250, -5.0, 5.0,
        250, 0.0, 25.0
    );
    h2RigiVsNSigmaProton = new TH2F(
        "h2RigiVsNSigmaProton", "Rigidity v.s. n#sigma (proton);p/q (GeV/c);n#sigma (proton);Counts", 
        100, -5.0, 5.0,
        100, -10.0, 10.0
    );
    h2RigiVsNSigmaPion = new TH2F(
        "h2RigiVsNSigmaPion", "Rigidity v.s. n#sigma (Pion);p/q (GeV/c);n#sigma (Pion);Counts", 
        100, -5.0, 5.0,
        100, -10.0, 10.0
    );
    h2RigiVsNSigmaKaon = new TH2F(
        "h2RigiVsNSigmaKaon", "Rigidity v.s. n#sigma (Kaon);p/q (GeV/c);n#sigma (Kaon);Counts", 
        100, -5.0, 5.0,
        100, -10.0, 10.0
    );
    h2RigiVsBTof1OverBeta = new TH2F(
        "h2RigiVsBTof1OverBeta", "Rigidity v.s. bTOF 1/#beta;p/q (GeV/c);1/#beta;Counts", 
        100, -5.0, 5.0,
        100, 0.7, 2.7
    );
    h2RigiVsBTofMass2 = new TH2F(
        "h2RigiVsBTofMass2", "Rigidity v.s. bTOF m^{2};p/q (GeV/c);m^{2} (GeV^{2}/c^{4});Counts", 
        250, -5.0, 5.0,
        250, -0.2, 2.3
    );

    h2BTofMass2VsNSigmaProton = new TH2F(
        "h2BTofMass2VsNSigmaProton", "bTOF m^{2} v.s. n#sigma;m^{2} (GeV^{2}/c^{4});n#sigma (proton);Counts", 
        250, -0.2, 2.3, 
        250, -10, 10
    );

    h2ProtonPtY = new TH2F(
        "h2ProtonPtY", "Proton Acceptance w/o TOF;y (proton);p_{T} (GeV/c);Counts", 
        230, -1.8, 0.5, 
        200, 0.0, 4.0
    );
    h2ProtonPtYbTOF = new TH2F(
        "h2ProtonPtYbTOF", "Proton Acceptance w/ bTOF;y (proton);p_{T} (GeV/c);Counts", 
        230, -1.8, 0.5, 
        200, 0.0, 4.0
    );

    h2EtaNHitsFit = new TH2F(
        "h2EtaNHitsFit", "#eta v.s. nHitsFit w/o nHitsRatio cut;#eta;nHitsFit;Counts",
        260, -2.6, 0.0,
        100, -0.5, 99.5
    );
    h2EtaNHitsDedx = new TH2F(
        "h2EtaNHitsDedx", "#eta v.s. nHitsDedx w/o nHitsRatio cut;#eta;nHitsDedx;Counts",
        260, -2.6, 0.0,
        100, -0.5, 99.5
    );
    h2EtaNHitsRatio = new TH2F(
        "h2EtaNHitsRatio", "#eta v.s. nHitsRatio#eta;#eta;nHitsRatio;Counts",
        260, -2.6, 0.0,
        120, -0.1, 1.1
    );

    h2PhiNHitsFit = new TH2F(
        "h2PhiNHitsFit", "#Phi v.s. nHitsFit w/o nHitsRatio cut;#Phi;nHitsFit;Counts",
        400, -4.0, 4.0,
        100, -0.5, 99.5
    );
    h2PhiNHitsDedx = new TH2F(
        "h2PhiNHitsDedx", "#Phi v.s. nHitsDedx w/o nHitsRatio cut;#Phi;nHitsDedx;Counts",
        400, -4.0, 4.0,
        100, -0.5, 99.5
    );
    h2PhiNHitsRatio = new TH2F(
        "h2PhiNHitsRatio", "#Phi v.s. nHitsRatio;#Phi;nHitsRatio;Counts",
        400, -4.0, 4.0,
        120, -0.1, 1.1
    );
    
    hNEpdMipEast = new TH1F(
        "hNEpdMipEast", "EPD east nMIP;N EPD nMIP (East);Counts", 
        50, -0.5, 49.5
    );
    hNEpdMipWest = new TH1F(
        "hNEpdMipWest", "EPD west nMIP;N EPD nMIP (West);Counts", 
        50, -0.5, 49.5
    );
    hNEpdTMipEast = new TH1F(
        "hNEpdTMipEast", "EPD east TnMIP;N EPD TnMIP (East);Counts", 
        50, -0.5, 49.5
    );
    hNEpdTMipWest = new TH1F(
        "hNEpdTMipWest", "EPD west TnMIP;N EPD TnMIP (West);Counts", 
        50, -0.5, 49.5
    );
    
    h2RigiVsETof1OverBeta = new TH2F(
        "h2RigiVsETof1OverBeta", "Rigidity v.s. eTOF 1/#beta;p/q (GeV/c);1/#beta;Counts", 
        100, -5.0, 5.0,
        100, 0.7, 2.7
    );
    h2RigiVsETofMass2 = new TH2F(
        "h2RigiVsETofMass2", "Rigidity v.s. eTOF m^{2};p/q (GeV/c);m^{2} (GeV^{2}/c^{4});Counts", 
        100, -5.0, 5.0,
        100, -0.2, 2.3
    );
    h2ETofMass2VsNSigmaProton = new TH2F(
        "h2ETofMass2VsNSigmaProton", "eTOF m^{2} v.s. n#sigma;m^{2} (GeV^{2}/c^{4});n#sigma (proton);Counts", 
        100, -0.2, 2.3, 
        100, -10, 10
    );
    h2ProtonPtYeTOF = new TH2F(
        "h2ProtonPtYeTOF", "Proton Acceptance w/ eTOF;y (proton);p_{T} (GeV/c);Counts", 
        230, -1.8, 0.5, 
        200, 0.0, 4.0
    );
    
    std::cout << "[LOG] - From Init: " << "Initializing Profiles." << std::endl;
    
    pRunVsVz = new TProfile("pRunVsVz", ";Run ID;<V_{z}> (cm)", nRuns, -0.5, nRuns-0.5);
    pRunVsVr = new TProfile("pRunVsVr", ";Run ID;<V_{r}> (cm)", nRuns, -0.5, nRuns-0.5);

    pRunVsRefMult = new TProfile("pRunVsRefMult", ";Run ID;<RefMult>", nRuns, -0.5, nRuns-0.5);
    pRunVsRefMult3 = new TProfile("pRunVsRefMult3", ";Run ID;<RefMult3>", nRuns, -0.5, nRuns-0.5);

    pRunvsNETofHits = new TProfile("pRunvsNETofHits", ";Run ID;<eTOF Hits>", nRuns, -0.5, nRuns-0.5);
    pRunvsNETofDigi = new TProfile("pRunvsNETofDigi", ";Run ID;<eTOF Digi>", nRuns, -0.5, nRuns-0.5);
    pRunVsFXTMult_DCA1 = new TProfile("pRunVsFXTMult_DCA1", ";Run ID;<FXTMult_DCA1>", nRuns, -0.5, nRuns-0.5);
    pRunVsFXTMult_DCA3 = new TProfile("pRunVsFXTMult_DCA3", ";Run ID;<FXTMult_DCA3>", nRuns, -0.5, nRuns-0.5);
    pRunVsFXTMult3_DCA1 = new TProfile("pRunVsFXTMult3_DCA1", ";Run ID;<FXTMult3_DCA1>", nRuns, -0.5, nRuns-0.5);
    pRunVsFXTMult3_DCA3 = new TProfile("pRunVsFXTMult3_DCA3", ";Run ID;<FXTMult3_DCA3>", nRuns, -0.5, nRuns-0.5);
    pRunVsTofMult = new TProfile("pRunVsTofMult", ";Run ID;<TofMult>", nRuns, -0.5, nRuns-0.5);
    pRunVsTofMult3 = new TProfile("pRunVsTofMult3", ";Run ID;<TofMult3>", nRuns, -0.5, nRuns-0.5);
    pRunVsEpdTnMip = new TProfile("pRunVsEpdTnMip", ";Run ID;<TnMip>", nRuns, -0.5, nRuns-0.5);
    pRunVsTofMatch = new TProfile("pRunVsTofMatch", ";Run ID;<TofMatch>", nRuns, -0.5, nRuns-0.5);
    pRunVsBTofMatch = new TProfile("pRunVsBTofMatch", ";Run ID;<bTofMatch>", nRuns, -0.5, nRuns-0.5);

    pRunVsNEpdHitsEast = new TProfile("pRunVsNEpdHitsEast", ";Run ID;<EPD Hits (East)>", nRuns, -0.5, nRuns-0.5);
    pRunVsNEpdHitsWest = new TProfile("pRunVsNEpdHitsWest", ";Run ID;<EPD Hits (West)>", nRuns, -0.5, nRuns-0.5);
    
    pRunVsNHitsFit = new TProfile("pRunVsNHitsFit", ";Run ID;<nHitsFit>", nRuns, -0.5, nRuns-0.5);
    pRunVsNHitsDedx = new TProfile("pRunVsNHitsDedx", ";Run ID;<nHitsDedx>", nRuns, -0.5, nRuns-0.5);
    pRunVsDca = new TProfile("pRunVsDca", ";Run ID;<DCA> (cm)", nRuns, -0.5, nRuns-0.5);
    pRunVsDcaZ = new TProfile("pRunVsDcaZ", ";Run ID;<DCAz> (cm)", nRuns, -0.5, nRuns-0.5);
    pRunVssDcaXY = new TProfile("pRunVssDcaXY", ";Run ID;<sDCAxy> (cm)", nRuns, -0.5, nRuns-0.5);
    pRunVsDcaZStd = new TProfile("pRunVsDcaZStd", ";Run ID;<DCAz std> (cm)", nRuns, -0.5, nRuns-0.5);
    pRunVssDcaXYStd = new TProfile("pRunVssDcaXYStd", ";Run ID;<sDCAxy std> (cm)", nRuns, -0.5, nRuns-0.5);
    pRunVsPt = new TProfile("pRunVsPt", ";Run ID;<p_{T}> (GeV/c)", nRuns, -0.5, nRuns-0.5);
    pRunVsEta = new TProfile("pRunVsEta", ";Run ID;<#eta>", nRuns, -0.5, nRuns-0.5);
    pRunVsPhi = new TProfile("pRunVsPhi", ";Run ID;<#phi>", nRuns, -0.5, nRuns-0.5);

    #ifdef __RBRDEDXRIGI__
    for (auto& runId : DbConf::mRunIdxMap) {
        TH2F* h2Tmp = new TH2F(
            Form("h2RigiVsDedx_%d", runId.first), Form("Rigidity v.s. dE/dx [%d];p/q (GeV/c);dE/dx (keV/cm);Counts", runId.first), 
            250, -5.0, 5.0,
            250, 0.0, 25.0
        );
        mapRBRh2RigiVsDedx[runId.second] = h2Tmp; // the key of this map is redcued ID
    }
    #endif
    
    std::cout << "[LOG] - From Init: " << "Initializing StFxtMult!" << std::endl;
    mtMult = new StFxtMult();
    // TpcShiftTool* mtShift = new TpcShiftTool();
    // mtShift->Init();
    // mtMult->ImportShiftTool(mtShift);
    mtMult->IgnoreShift();
    
    std::cout << "[LOG] - From Init: " << "Initializing BadRunTool!" << std::endl;
    mtRun = new BadRunTool();
    mtRun->Init();
    
    std::cout << "[LOG] - From Init: " << "Initializing MeanDcaTool!" << std::endl;
    mtDca = new MeanDcaTool();
    mtDca->ReadParams();
    
    std::cout << "[LOG] - From Init: " << "Initializing VtxShiftTool!" << std::endl;
    mtVtx = new VtxShiftTool();

    std::cout << "[LOG] - From Init: " << "Initializing PileUpTool!" << std::endl;
    mtPu = new PileUpTool();
    mtPu->ReadParams();

    std::cout << "[LOG] - From Init: " << "This is the end of Init() function." << std::endl;

    return kStOK;
}

Int_t StYQAMaker::Finish() {

    /*
        Finish function, write histograms or profiles into root file.
        And save the file.
    */

    std::cout << "[LOG] - From Finish: " << "Creating root file." << std::endl;
    TFile* tfout = new TFile(mOutName, "recreate");
    std::cout << "[LOG] - From Finish: " << mOutName.Data() << std::endl;
    tfout->cd();

    // Event-wise
    hNev->Write();
    h2VxVy->Write();
    h2VxVyVrCut->Write();
    hTpcVz->Write();
    hTpcVzVrCut->Write();
    hVpdVz->Write();
    h2VzVpd->Write();
    hNVpdHitsEast->Write();
    hNVpdHitsWest->Write();
    hBbcX->Write();
    hZdcX->Write();
    hRefMult->Write();
    hRefMult3->Write();
    hNBTofHits->Write();
    hNEpdHitsEast->Write();
    hNEpdHitsWest->Write();
    hNETofDigis->Write();
    hNETofHits->Write();

    // FXT/TOF multiplicities
    hFXTMult_DCA1->Write();
    hFXTMult_DCA3->Write();
    hFXTMult3_DCA1->Write();
    hFXTMult3_DCA3->Write();
    hTofMult->Write();
    hTofMult3->Write();
    hTofMatch->Write();
    hBTofMatch->Write();
    hEpdTnMip->Write();

    h2FXTMult3DCA11FXTMult3DCA3->Write();
    h2FXTMult3DCA1TofMult3->Write();
    h2FXTMult3DCA1EpdTnMip->Write();

    h2FXTMultDCA3sDCAxy->Write();
    h2FXTMultDCA3sDCAz->Write();

    // Track-wise
    hNHitsFit->Write();
    hNHitsFitRatioCut->Write();
    hNHitsDedx->Write();
    hNHitsDedxRatioCut->Write();
    hNHitsRatio->Write();
    hDca->Write();
    hDcaZ->Write();
    hsDcaXY->Write();
    hPt->Write();
    hEta->Write();
    hPhi->Write();
    h2RigiVsDedx->Write();
    h2RigiVsNSigmaProton->Write();
    h2RigiVsNSigmaPion->Write();
    h2RigiVsNSigmaKaon->Write();
    h2RigiVsBTof1OverBeta->Write();
    h2RigiVsBTofMass2->Write();
    h2BTofMass2VsNSigmaProton->Write();
    h2ETofMass2VsNSigmaProton->Write();
    h2ProtonPtY->Write();
    h2ProtonPtYbTOF->Write();
    h2EtaNHitsFit->Write();
    h2EtaNHitsDedx->Write();
    h2EtaNHitsRatio->Write();
    h2PhiNHitsFit->Write();
    h2PhiNHitsDedx->Write();
    h2PhiNHitsRatio->Write();
    hNEpdMipEast->Write();
    hNEpdMipWest->Write();
    hNEpdTMipEast->Write();
    hNEpdTMipWest->Write();
    h2RigiVsETof1OverBeta->Write();
    h2RigiVsETofMass2->Write();
    h2ProtonPtYeTOF->Write();

    // Profiles
    // Event-wise
    pRunVsVz->Write();
    pRunVsVr->Write();
    pRunVsRefMult->Write();
    pRunVsRefMult3->Write();
    pRunVsNEpdHitsEast->Write();
    pRunVsNEpdHitsWest->Write();
    pRunvsNETofHits->Write();
    pRunvsNETofDigi->Write();

    pRunVsFXTMult_DCA1->Write();
    pRunVsFXTMult_DCA3->Write();
    pRunVsFXTMult3_DCA1->Write();
    pRunVsFXTMult3_DCA3->Write();
    pRunVsTofMult->Write();
    pRunVsTofMult3->Write();
    pRunVsEpdTnMip->Write();
    pRunVsTofMatch->Write();
    pRunVsBTofMatch->Write();
    
    // Track-wise
    pRunVsNHitsFit->Write();
    pRunVsNHitsDedx->Write();
    pRunVsDca->Write();
    pRunVsDcaZ->Write();
    pRunVsDcaZStd->Write();
    pRunVssDcaXY->Write();
    pRunVssDcaXYStd->Write();
    pRunVsPt->Write();
    pRunVsEta->Write();
    pRunVsPhi->Write();

    #ifdef __RBRDEDXRIGI__
    for (auto& item : mapRBRh2RigiVsDedx) {
        item.second->Write();
    }
    #endif

    tfout->Close();
    std::cout << "[LOG] - From Finish: " << ".root file saved." << std::endl;

    return kStOK;
}

void StYQAMaker::Clear(Option_t* option) {

    /*
        Clear function.
        Set pointers to null.
    */
   
    mPicoEvent = nullptr;
    mPicoTrack = nullptr;
    mBTofPidTraits = nullptr;
    mEpdHit = nullptr;
    mETofPidTraits = nullptr;

    vpt.clear();
    veta.clear();
    vphi.clear();
    vsdca.clear();
    vnhitsdedx.clear();
    vnhitsfit.clear();
    vsdcaxy.clear();
    vsdcaz.clear();
}

Int_t StYQAMaker::Make() {

    /*
        To make an event.
    */

    mPicoEvent = mPicoDst->event();
    if (!mPicoEvent) {
        LOG_WARN << "[WARNING] - From Make: " << "Can not open PicoDst event, skip this." << endm;
        return kStWarn;
    }
    
    hNev->Fill(0);

    if (!IsGoodTrigger()) { // only use MB triggers
        return kStOK;
    }
    hNev->Fill(1);

    Int_t runRawID = mPicoEvent->runId();
    if (DbConf::mRunIdxMap.count(runRawID) == 0) {
        LOG_WARN << "[WARNING] - From Makes: " << runRawID << " is not a valid run from the run list." << endm;
        return kStWarn;
    }
    mRunId = DbConf::mRunIdxMap.at(runRawID);

    TVector3 vertex = mPicoEvent->primaryVertex();
    vx = vertex.X();
    vy = vertex.Y();
    vz = vertex.Z();
    // shift the vertex
    auto vr = mtVtx->GetShiftedVr(vx, vy);
    Float_t vpd_vz = mPicoEvent->vzVpd();

    h2VxVy->Fill(vx, vy);
    hTpcVz->Fill(vz);
    hVpdVz->Fill(vpd_vz);
    h2VzVpd->Fill(vz, vpd_vz);
    hNVpdHitsEast->Fill(mPicoEvent->nVpdHitsEast());
    hNVpdHitsWest->Fill(mPicoEvent->nVpdHitsWest());

    pRunVsVz->Fill(mRunId, vz);
    pRunVsVr->Fill(mRunId, vr);

    // if (vr > 2 || vz < 199 || vz > 200.5) { return kStOK; }
    if (!mtVtx->IsGoodVertex(vx, vy, vz)) { return kStOK; }
    h2VxVyVrCut->Fill(vx, vy);
    hTpcVzVrCut->Fill(vz);

    hNev->Fill(2);

    if (mtRun->IsBadRun(runRawID)) { return kStOK; }
    hNev->Fill(3);

    Float_t BBCx = mPicoEvent->BBCx();    
    Float_t ZDCx = mPicoEvent->ZDCx(); 

    hBbcX->Fill(BBCx);
    hZdcX->Fill(ZDCx);
    hRefMult->Fill(mPicoEvent->refMult());
    hRefMult3->Fill(mPicoEvent->refMult3());
    pRunVsRefMult->Fill(mRunId, mPicoEvent->refMult());
    pRunVsRefMult3->Fill(mRunId, mPicoEvent->refMult3());
    hNBTofHits->Fill(mPicoEvent->btofTrayMultiplicity());
    hNETofHits->Fill(mPicoEvent->etofHitMultiplicity());
    hNETofDigis->Fill(mPicoEvent->etofDigiMultiplicity());
    pRunvsNETofHits->Fill(mRunId, mPicoEvent->etofHitMultiplicity());
    pRunvsNETofDigi->Fill(mRunId, mPicoEvent->etofDigiMultiplicity());

    mBField = mPicoEvent->bField(); // for dca

    // Make TPC and TOF issue
    const int numberOfTracks = mPicoDst->numberOfTracks();
    for (Int_t iTrack=0; iTrack<numberOfTracks; iTrack++) {
        MakeTrack(iTrack);
    }

    // Calculate event-wise mean value for track-wise quantities
    Double_t ptM, etaM, phiM, dcaM, nhitsDedxM, nhitsFitM, sDCAxyM, sDCAzM, sDCAxyS, sDCAzS;
    // compute means for track-wise vectors
    if (!vpt.empty()) {
        double sum = 0;
        for (double x : vpt) sum += x;
        ptM = sum / vpt.size();
        pRunVsPt->Fill(mRunId, ptM);
    } else ptM = 0.0;

    if (!veta.empty()) {
        double sum = 0;
        for (double x : veta) sum += x;
        etaM = sum / veta.size();
        pRunVsEta->Fill(mRunId, etaM);
    } else etaM = 0.0;

    if (!vphi.empty()) {
        double sum = 0;
        for (double x : vphi) sum += x;
        phiM = sum / vphi.size();
        pRunVsPhi->Fill(mRunId, phiM);
    } else phiM = 0.0;

    if (!vsdca.empty()) {
        double sum = 0;
        for (double x : vsdca) sum += x;
        dcaM = sum / vsdca.size();
        pRunVsDca->Fill(mRunId, dcaM);
    } else dcaM = 0.0;

    if (!vnhitsdedx.empty()) {
        double sum = 0;
        for (double x : vnhitsdedx) sum += x;
        nhitsDedxM = sum / vnhitsdedx.size();
        pRunVsNHitsDedx->Fill(mRunId, nhitsDedxM);
    } else nhitsDedxM = 0.0;

    if (!vnhitsfit.empty()) {
        double sum = 0;
        for (double x : vnhitsfit) sum += x;
        nhitsFitM = sum / vnhitsfit.size();
        pRunVsNHitsFit->Fill(mRunId, nhitsFitM);
    } else nhitsFitM = 0.0;

    // now we use mean DCA tool to get the mean and sigma of DCA xy/z
    bool E2Flag = false; // false means: IS a bad mean DCA event -> we will fill the entry when true
    if (!mtDca->Make(mPicoDst)) {
        sDCAzM = 0;
        sDCAzS = 0;
        sDCAxyM = 0;
        sDCAxyS = 0;
    } else {
        sDCAzM = mtDca->GetMeanZ();
        sDCAzS = mtDca->GetStdDevZ();
        sDCAxyM = mtDca->GetMeanXY();
        sDCAxyS = mtDca->GetStdDevXY();
        pRunVssDcaXY->Fill(mRunId, sDCAxyM);
        pRunVssDcaXYStd->Fill(mRunId, sDCAxyS);
        pRunVsDcaZ->Fill(mRunId, sDCAzM);
        pRunVsDcaZStd->Fill(mRunId, sDCAzS);
        if(mtDca->IsBadMeanDcaXYEvent(mPicoDst) || mtDca->IsBadMeanDcaZEvent(mPicoDst)) { E2Flag = false; }
        else { E2Flag = true; }
    }

    // Multiplicity-related
    bool E1Flag = false; // for pile-up check
    if (mtMult->make(mPicoDst)) {
        hFXTMult_DCA1->Fill(mtMult->mFXTMult_DCA1);
        hFXTMult_DCA3->Fill(mtMult->mFXTMult_DCA3);
        hFXTMult3_DCA1->Fill(mtMult->mFXTMult3_DCA1);
        hFXTMult3_DCA3->Fill(mtMult->mFXTMult3_DCA3);

        hTofMult->Fill(mtMult->mTofMult);
        hTofMult3->Fill(mtMult->mTofMult3);
        hTofMatch->Fill(mtMult->mTofMatch);
        hBTofMatch->Fill(mtMult->mBTofMatch);
        hEpdTnMip->Fill(mtMult->mEpdTnMip);

        // pile-up correlation plots
        h2FXTMult3DCA11FXTMult3DCA3->Fill(mtMult->mFXTMult3_DCA1, mtMult->mFXTMult3_DCA3);
        h2FXTMult3DCA1TofMult3->Fill(mtMult->mFXTMult3_DCA1, mtMult->mTofMult3);
        h2FXTMult3DCA1EpdTnMip->Fill(mtMult->mFXTMult3_DCA1, mtMult->mEpdTnMip);

        // mean dca 2D heatmap
        h2FXTMultDCA3sDCAxy->Fill(mtMult->mFXTMult_DCA3, sDCAxyM);
        h2FXTMultDCA3sDCAz->Fill(mtMult->mFXTMult_DCA3, sDCAzM);

        // profiles
        pRunVsFXTMult_DCA1->Fill(mRunId, mtMult->mFXTMult_DCA1);
        pRunVsFXTMult_DCA3->Fill(mRunId, mtMult->mFXTMult_DCA3);
        pRunVsFXTMult3_DCA1->Fill(mRunId, mtMult->mFXTMult3_DCA1);
        pRunVsFXTMult3_DCA3->Fill(mRunId, mtMult->mFXTMult3_DCA3);

        pRunVsTofMult->Fill(mRunId, mtMult->mTofMult);
        pRunVsTofMult3->Fill(mRunId, mtMult->mTofMult3);
        pRunVsTofMatch->Fill(mRunId, mtMult->mTofMatch);
        pRunVsBTofMatch->Fill(mRunId, mtMult->mBTofMatch);
        pRunVsEpdTnMip->Fill(mRunId, mtMult->mEpdTnMip);

        if (mtPu->IsPileUp(
            mtMult->mFXTMult3_DCA1, 
            mtMult->mFXTMult3_DCA3,
            mtMult->mEpdTnMip,
            mtMult->mTofMult3
        )) { E1Flag = false; }
        else {E1Flag = true; }
    }

    if (E1Flag) { hNev->Fill(4); }
    if (E2Flag) { hNev->Fill(5); }
    if (E1Flag && E2Flag) { hNev->Fill(6); }

    // EPD-related
    const int numberOfEpdHits = mPicoDst->numberOfEpdHits();
    mNEpdHitsEast = 0;
    mNEpdHitsWest = 0;
    for (Int_t iHit=0; iHit<numberOfEpdHits; iHit++) {
        MakeEpdHits(iHit);
    }

    hNEpdHitsEast->Fill(mNEpdHitsEast);
    hNEpdHitsWest->Fill(mNEpdHitsWest);
    pRunVsNEpdHitsEast->Fill(mRunId, mNEpdHitsEast);
    pRunVsNEpdHitsWest->Fill(mRunId, mNEpdHitsWest);

    return kStOK;

}

Int_t StYQAMaker::MakeTrack(Int_t iTrack) {

    /*
        A block of processing the TPC tracks.
        Note that, the iTrack should be from looping of tracks.
    */

    mPicoTrack = mPicoDst->track(iTrack);
    if (!mPicoTrack) {
        LOG_WARN << "[WARNING] - From MakeTrack: " << "Can not open picoTrack from pidoDst, skip." << endm;
        return kStWarn;
    }

    if (!mPicoTrack->isPrimary()) {
        return kStOK;
    }

    if (mPicoTrack->isTofTrack()) {
        mBTofPidTraits = mPicoDst->btofPidTraits(mPicoTrack->bTofPidTraitsIndex());
    } else {
        mBTofPidTraits = 0;
    }
    
    if (mPicoTrack->isETofTrack()) {
        mETofPidTraits = mPicoDst->etofPidTraits(mPicoTrack->eTofPidTraitsIndex());
    } else {
        mETofPidTraits = 0;
    }

    Double_t DCA = mPicoTrack->gDCA(vx, vy, vz);
    Double_t DCAz = mPicoTrack->gDCAz(vz);
    Double_t sDCAxy = mPicoTrack->helix(mBField).geometricSignedDistance(vx, vy);
    
    hDca->Fill(DCA);
    hDcaZ->Fill(DCAz);
    hsDcaXY->Fill(sDCAxy);
    
    Int_t nHitsFit = mPicoTrack->nHitsFit();
    Int_t nHitsPoss = mPicoTrack->nHitsPoss();
    Float_t nHitsRatio = 1.0 * nHitsFit / nHitsPoss;
    Int_t nHitsDedx = mPicoTrack->nHitsDedx();
    Int_t q = mPicoTrack->charge();

    hNHitsFit->Fill(nHitsFit/q);
    hNHitsDedx->Fill(nHitsDedx);
    if (nHitsRatio > 0.51) {
        hNHitsFitRatioCut->Fill(nHitsFit/q);
        hNHitsDedxRatioCut->Fill(nHitsDedx);
    }
    hNHitsRatio->Fill(nHitsRatio);

    Double_t nSigProton = mPicoTrack->nSigmaProton();
    Double_t nSigPion = mPicoTrack->nSigmaPion();
    Double_t nSigKaon = mPicoTrack->nSigmaKaon();
    
    // push track-wise quantities to vectors
    // track quality quantities here
    vsdca.push_back(DCA);
    vnhitsdedx.push_back(nHitsDedx);
    vnhitsfit.push_back(nHitsFit);
    vsdcaxy.push_back(DCAz);
    vsdcaz.push_back(sDCAxy);

    bool cutFlag = false;
    // here use quality cut to remove bad tracks
    if (fabs(DCA) > 1.0 || nHitsFit < 20 || nHitsDedx < 5 || nHitsRatio < 0.51) {
        cutFlag = true;
    }

    TVector3 momentum = mPicoTrack->pMom();
    Double_t pt = momentum.Perp();
    Double_t eta = momentum.PseudoRapidity();
    Double_t phi = momentum.Phi();
    Double_t p = momentum.Mag();
    Double_t pz = momentum.Z();
    Double_t EP = sqrt(p*p + 0.938272 * 0.938272);
    Double_t YP = TMath::Log((EP + pz) / (EP - pz + 1e-7)) * 0.5; 
    YP = fabs(YP) - ybeam;

    if (cutFlag) {
        // only those heat maps need un-cut
        h2EtaNHitsFit->Fill(eta, nHitsFit);
        h2EtaNHitsDedx->Fill(eta, nHitsDedx);
        h2EtaNHitsRatio->Fill(eta, nHitsRatio);
        h2PhiNHitsFit->Fill(phi, nHitsFit);
        h2PhiNHitsDedx->Fill(phi, nHitsDedx);
        h2PhiNHitsRatio->Fill(phi, nHitsRatio);
        return kStOK; 
    }
    
    // kinematic quantities here
    vpt.push_back(pt);
    veta.push_back(eta);
    vphi.push_back(phi);

    hPt->Fill(pt);
    hEta->Fill(eta);
    hPhi->Fill(phi);

    h2ProtonPtY->Fill(YP, pt);

    Double_t bTofBeta = GetBTofBeta();
    Double_t eTofBeta = GetETofBeta();
    Double_t rigi = p / q;

    h2RigiVsDedx->Fill(rigi, mPicoTrack->dEdx());
    h2RigiVsNSigmaProton->Fill(rigi, nSigProton);
    h2RigiVsNSigmaPion->Fill(rigi, nSigPion);
    h2RigiVsNSigmaKaon->Fill(rigi, nSigKaon);

    #ifdef __RBRDEDXRIGI__
    mapRBRh2RigiVsDedx[mRunId]->Fill(rigi, mPicoTrack->dEdx());
    #endif

    if (bTofBeta > 1e-5) {
        Double_t m2b = p*p * (pow(bTofBeta, -2.0) - 1);
        h2RigiVsBTof1OverBeta->Fill(rigi, 1.0 / bTofBeta);
        h2RigiVsBTofMass2->Fill(rigi, m2b);
        h2BTofMass2VsNSigmaProton->Fill(m2b, nSigProton);
        h2ProtonPtYbTOF->Fill(YP, pt);
    }

    if (eTofBeta > 1e-5) {
        Double_t m2e = p*p * (pow(eTofBeta, -2.0) - 1);
        h2RigiVsETof1OverBeta->Fill(rigi, 1.0 / eTofBeta);
        h2RigiVsETofMass2->Fill(rigi, m2e);
        h2ETofMass2VsNSigmaProton->Fill(m2e, nSigProton);
        h2ProtonPtYeTOF->Fill(YP, pt);
    }

    return kStOK;
}

Bool_t StYQAMaker::IsGoodTrigger() {
    for (const UInt_t& trg : DbConf::mTriggers) {
        if (mPicoEvent->isTrigger(trg)) { return kTRUE; }
    }
    return kFALSE;
}

Double_t StYQAMaker::GetBTofBeta() {
    if (!mPicoTrack->isTofTrack()) { return 0; }
    if (mPicoTrack->bTofPidTraitsIndex() < 0) { return 0; }
    if (!mBTofPidTraits) { return 0; }
    if (mBTofPidTraits->btofMatchFlag() <= 0) { return 0; }
    Double_t beta = mBTofPidTraits->btofBeta();
    return beta < 1e-5 ?  0.0 : beta;
}

Double_t StYQAMaker::GetETofBeta() {
    if (!mPicoTrack->isETofTrack()) { return 0; }
    if (mPicoTrack->eTofPidTraitsIndex() < 0) { return 0; }
    if (!mETofPidTraits) { return 0; }
    if (mETofPidTraits->hitIndex() < 0) { return 0; }
    Double_t beta = mETofPidTraits->beta();
    return beta < 1e-5 ?  0.0 : beta;
}

Int_t StYQAMaker::MakeEpdHits(const Int_t iHit) {
    
    /*
        A block of processing the EPD hits.
        Note that, the iHit should be from looping of EPDhits.
    */

    mEpdHit = mPicoDst->epdHit(iHit);
    if (!mEpdHit) {
        LOG_WARN << "[WARNING] - From MakeEpdHits: " << "Can not open EPD hits frpm picoDst." << endm;
        return kStWarn;
    }

    const Bool_t IsEast = mEpdGeom->IsEast(mEpdHit->id());

    if (IsEast) { mNEpdHitsEast++; }
    else { mNEpdHitsWest++; }

    if (!mEpdHit->isGood()) { return kStOK; }

    const Int_t nMip = mEpdHit->nMIP();
    const Double_t tnMip = mEpdHit->TnMIP(4.0, 0.3); // is also the default args

    if (IsEast) { hNEpdMipEast->Fill(nMip); } 
    else { hNEpdMipWest->Fill(nMip); }
    if (IsEast) { hNEpdTMipEast->Fill(tnMip); } 
    else { hNEpdTMipWest->Fill(tnMip); }

    return kStOK;
}