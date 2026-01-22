#include "StYQAMaker.h"

#include "TMath.h"
#include "TVector3.h"

#include "phys_constants.h"
#include "StThreeVector.hh"

#include "StBTofUtil/tofPathLength.hh"

#include "DbConf.h"

ClassImp(StYQAMaker);

StYQAMaker::StYQAMaker(const char* name, StPicoDstMaker* picoMaker, const char* outName) : StMaker(name) {

    /*
        The construction function of this class.
        Do some of the initialization.
    */

    mPicoDstMaker = picoMaker;
    mPicoDst = 0;
    mPicoEvent = 0;
    mPicoTrack = 0;

    TString fileName = outName;
    mOutName = fileName + ".root";

    #ifdef __WITH_EPD__
        mEpdGeom = new StEpdGeom();
    #endif

}

StYQAMaker::~StYQAMaker() {
    // Event-wise histograms
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

#ifdef __WITH_EPD__
    delete hNEpdHitsEast; hNEpdHitsEast = nullptr;
    delete hNEpdHitsWest; hNEpdHitsWest = nullptr;
#endif

#ifdef __WITH_ETOF__
    delete hNETofDigis; hNETofDigis = nullptr;
    delete hNETofHits; hNETofHits = nullptr;
#endif

    delete hNBTofHits; hNBTofHits = nullptr;
    delete hNBTofMatch; hNBTofMatch = nullptr;

    // Track-wise histograms
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

#ifdef __WITH_EPD__
    delete hNEpdMipEast; hNEpdMipEast = nullptr;
    delete hNEpdMipWest; hNEpdMipWest = nullptr;
#endif

#ifdef __WITH_ETOF__
    delete h2RigiVsETof1OverBeta; h2RigiVsETof1OverBeta = nullptr;
    delete h2RigiVsETofMass2; h2RigiVsETofMass2 = nullptr;
    delete h2ETofMass2VsNSigmaProton; h2ETofMass2VsNSigmaProton = nullptr;
    delete h2ProtonPtYeTOF; h2ProtonPtYeTOF = nullptr;
#endif

    // Profiles
    delete pRunVsVx; pRunVsVx = nullptr;
    delete pRunVsVy; pRunVsVy = nullptr;
    delete pRunVsVz; pRunVsVz = nullptr;
    delete pRunVsVr; pRunVsVr = nullptr;
    delete pRunVsBbcX; pRunVsBbcX = nullptr;
    delete pRunVsZdcX; pRunVsZdcX = nullptr;
    delete pRunvsNBTofHits; pRunvsNBTofHits = nullptr;
    delete pRunvsNBTofMatch; pRunvsNBTofMatch = nullptr;

    delete pRunVsRefMult; pRunVsRefMult = nullptr;
    delete pRunVsRefMult3; pRunVsRefMult3 = nullptr;

#ifdef __WITH_EPD__
    delete pRunVsNEpdHitsEast; pRunVsNEpdHitsEast = nullptr;
    delete pRunVsNEpdHitsWest; pRunVsNEpdHitsWest = nullptr;
#endif

#ifdef __WITH_ETOF__
    delete pRunvsNETofHits; pRunvsNETofHits = nullptr;
    delete pRunvsNETofDigi; pRunvsNETofDigi = nullptr;
#endif

    delete pRunVsNHitsFit; pRunVsNHitsFit = nullptr;
    delete pRunVsNHitsRatio; pRunVsNHitsRatio = nullptr;
    delete pRunVsNHitsDedx; pRunVsNHitsDedx = nullptr;
    delete pRunVsDca; pRunVsDca = nullptr;
    delete pRunVsDcaZ; pRunVsDcaZ = nullptr;
    delete pRunVssDcaXY; pRunVssDcaXY = nullptr;
    delete pRunVsPt; pRunVsPt = nullptr;
    delete pRunVsEta; pRunVsEta = nullptr;
    delete pRunVsPhi; pRunVsPhi = nullptr;
    delete pRunVsBTof1OverBeta; pRunVsBTof1OverBeta = nullptr;

#ifdef __WITH_EPD__
    delete pRunVsNMipEast; pRunVsNMipEast = nullptr;
    delete pRunVsNMipWest; pRunVsNMipWest = nullptr;
    delete pRunVsTNMipEast; pRunVsTNMipEast = nullptr;
    delete pRunVsTNMipWest; pRunVsTNMipWest = nullptr;
#endif

#ifdef __WITH_ETOF__
    delete pRunVsETof1OverBeta; pRunVsETof1OverBeta = nullptr;
#endif

    // Delete objects allocated in constructor
#ifdef __WITH_EPD__
    delete mEpdGeom; mEpdGeom = nullptr;
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
    
    hNev = new TH1D("hNev", ";Events;Counts", 2, -0.5, 1.5);
    hNev->GetXaxis()->SetBinLabel(1, "Raw");
    hNev->GetXaxis()->SetBinLabel(2, "With Cut");

    h2VxVy = new TH2F(
        "hVxVy", "TPC V_{x} v.s. V_{y} w/o V_{r} cut;V_{x} [cm];V_{y} [cm];Counts", 
        200, -6.0, 2.0,
        200, -6.0, 2.0
    );
    h2VxVyVrCut = new TH2F(
        "hVxVyVrCut", "TPC V_{x} v.s. V_{y} w/ V_{r} cut;V_{x} [cm];V_{y} [cm];Counts", 
        200, -6.0, 2.0,
        200, -6.0, 2.0
    );

    hTpcVz = new TH1F(
        "hTpcVz", "TPC V_{z} w/o V_{r} cut;V_{z} [cm];Counts", 
        100, 195, 205
    );
    hTpcVzVrCut = new TH1F(
        "hTpcVzVrCut", "TPC V_{z} w/ V_{r} cut;V_{z} [cm];Counts", 
        100, 195, 205
    );
    
    hVpdVz = new TH1F(
        "hVpdVz", "VPD V_{z};V_{z} [cm];Counts", 
        100, 195, 205
    );
    h2VzVpd = new TH2F(
        "h2VzVpd", "TPC V_{z} v.s. VPD V_{z};TPC V_{z} [cm];VPD V_{z} [cm];Counts", 
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
        "hBbcX", "BBC coincidence rate;BBCx [Hz];Counts", 
        500, -0.5, 499.5
    );
    hZdcX = new TH1F(
        "hZdcX", "ZDC coincidence rate;BBCx [Hz];Counts", 
        100, -0.5, 99.5
    );

    hNBTofHits = new TH1F(
        "hNBTofHits", "bTOF hits;N bTOF Hits;Counts", 
        1600, -0.5, 1599.5
    );
    hNBTofMatch = new TH1F(
        "hNBTofMatch", "bTOF matches;N bTOF Match;Counts", 
        200, -0.5, 199.5
    );

    hRefMult = new TH1F(
        "hRefMult", "RefMult (raw);RefMult;Counts",
        100, -0.5, 99.5
    );
    hRefMult3 = new TH1F(
        "hRefMult3", "RefMult3 (raw);RefMult3;Counts",
        200, -0.5, 199.5
    );

    #ifdef __WITH_EPD__
        hNEpdHitsEast = new TH1F(
            "hNEpdHitsEast", "EPD east hits;N EPD Hits (East);Counts", 
            600, -0.5, 599.5
        );
        hNEpdHitsWest = new TH1F(
            "hNEpdHitsWest", "EPD west hits;N EPD Hits (West);Counts", 
            600, -0.5, 599.5
        );
    #endif

    #ifdef __WITH_ETOF__
        hNETofDigis = new TH1F(
            "hNETofDigis", "eTOF digis;N eTOF Digi;Counts", 
            1600, -0.5, 1599.5
        );
        hNETofHits = new TH1F(
            "hNETofHits", "eTOF hits;N eTOF Hits;Counts", 
            800, -0.5, 799.5
        );
    #endif

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
        "hDca", "DCA;DCA [cm];Counts", 
        300, 0.0, 6.0
    );
    hDcaZ = new TH1F(
        "hDcaZ", "DCAz;DCAz [cm];Counts", 
        100, -5.0, 5.0
    );
    hsDcaXY = new TH1F(
        "hsDcaXY", "sDCAxy;sDCAxy [cm];Counts", 
        100, -5.0, 5.0
    );

    hPt = new TH1F(
        "hPt", "p_{T};p_{T} [GeV/c];Counts", 
        500, 0.0, 5.0
    );
    hEta = new TH1F(
        "hEta", "#eta;#eta;Counts", 
        500, -5.0, 0.0
    );
    hPhi = new TH1F(
        "hPhi", "#phi;#phi;Counts", 
        // 360, -TMath::Pi(), TMath::Pi()
        400, -4.0, 4.0
    );

    h2RigiVsDedx = new TH2F(
        "h2RigiVsDedx", "Rigidity v.s. dE/dx;p/q [GeV/c];dE/dx [keV/cm];Counts", 
        250, -5.0, 5.0,
        250, 0.0, 25.0
    );
    h2RigiVsNSigmaProton = new TH2F(
        "h2RigiVsNSigmaProton", "Rigidity v.s. n#sigma (proton);p/q [GeV/c];n#sigma (proton);Counts", 
        100, -5.0, 5.0,
        100, -10.0, 10.0
    );
    h2RigiVsNSigmaPion = new TH2F(
        "h2RigiVsNSigmaPion", "Rigidity v.s. n#sigma (Pion);p/q [GeV/c];n#sigma (Pion);Counts", 
        100, -5.0, 5.0,
        100, -10.0, 10.0
    );
    h2RigiVsNSigmaKaon = new TH2F(
        "h2RigiVsNSigmaKaon", "Rigidity v.s. n#sigma (Kaon);p/q [GeV/c];n#sigma (Kaon);Counts", 
        100, -5.0, 5.0,
        100, -10.0, 10.0
    );
    h2RigiVsBTof1OverBeta = new TH2F(
        "h2RigiVsBTof1OverBeta", "Rigidity v.s. bTOF 1/#beta;p/q [GeV/c];1/#beta;Counts", 
        100, -5.0, 5.0,
        100, 0.7, 2.7
    );
    h2RigiVsBTofMass2 = new TH2F(
        "h2RigiVsBTofMass2", "Rigidity v.s. bTOF m^{2};p/q [GeV/c];m^{2} [GeV^{2}/c^{4}];Counts", 
        250, -5.0, 5.0,
        250, -0.2, 2.3
    );

    h2BTofMass2VsNSigmaProton = new TH2F(
        "h2BTofMass2VsNSigmaProton", "bTOF m^{2} v.s. n#sigma;m^{2} [GeV^{2}/c^{4}];n#sigma (proton);Counts", 
        250, -0.2, 2.3, 
        250, -10, 10
    );

    h2ProtonPtY = new TH2F(
        "h2ProtonPtY", "Proton Acceptance w/o TOF;y (proton);p_{T} [GeV/c];Counts", 
        200, -2.0, 0.0, 
        200, 0.0, 4.0
    );
    h2ProtonPtYbTOF = new TH2F(
        "h2ProtonPtYbTOF", "Proton Acceptance w/ bTOF;y (proton);p_{T} [GeV/c];Counts", 
        200, -2.0, 0.0, 
        200, 0.0, 4.0
    );

    #ifdef __WITH_EPD__
        hNEpdMipEast = new TH1F(
            "hNEpdMipEast", "EPD east MIP;N EPD MIP (East);Counts", 
            50, -0.5, 49.5
        );
        hNEpdMipWest = new TH1F(
            "hNEpdMipWest", "EPD west MIP;N EPD MIP (West);Counts", 
            50, -0.5, 49.5
        );
    #endif

    #ifdef __WITH_ETOF__
        h2RigiVsETof1OverBeta = new TH2F(
            "h2RigiVsETof1OverBeta", "Rigidity v.s. eTOF 1/#beta;p/q [GeV/c];1/#beta;Counts", 
            100, -5.0, 5.0,
            100, 0.7, 2.7
        );
        h2RigiVsETofMass2 = new TH2F(
            "h2RigiVsETofMass2", "Rigidity v.s. eTOF m^{2};p/q [GeV/c];m^{2} [GeV^{2}/c^{4}];Counts", 
            100, -5.0, 5.0,
            100, -0.2, 2.3
        );
        h2ETofMass2VsNSigmaProton = new TH2F(
            "h2ETofMass2VsNSigmaProton", "eTOF m^{2} v.s. n#sigma;m^{2} [GeV^{2}/c^{4}];n#sigma (proton);Counts", 
            100, -0.2, 2.3, 
            100, -10, 10
        );
        h2ProtonPtYeTOF = new TH2F(
            "h2ProtonPtYeTOF", "Proton Acceptance w/ eTOF;y (proton);p_{T} [GeV/c];Counts", 
            200, -2.0, 0.0, 
            200, 0.0, 4.0
        );
    #endif

    std::cout << "[LOG] - From Init: " << "Initializing Profiles." << std::endl;

    pRunVsVx = new TProfile("pRunVsVx", ";Run ID;<V_{x}> [cm]", nRuns, -0.5, nRuns-0.5);
    pRunVsVy = new TProfile("pRunVsVy", ";Run ID;<V_{y}> [cm]", nRuns, -0.5, nRuns-0.5);
    pRunVsVz = new TProfile("pRunVsVz", ";Run ID;<V_{z}> [cm]", nRuns, -0.5, nRuns-0.5);
    pRunVsVr = new TProfile("pRunVsVr", ";Run ID;<V_{r}> [cm]", nRuns, -0.5, nRuns-0.5);
    pRunVsBbcX = new TProfile("pRunVsBbcX", ";Run ID;<BBCx> [Hz]", nRuns, -0.5, nRuns-0.5);
    pRunVsZdcX = new TProfile("pRunVsZdcX", ";Run ID;<ZDCx> [Hz]", nRuns, -0.5, nRuns-0.5);
    pRunvsNBTofHits = new TProfile("pRunvsNBTofHits", ";Run ID;<bTOF Hits>", nRuns, -0.5, nRuns-0.5);
    pRunvsNBTofMatch = new TProfile("pRunvsNBTofMatch", ";Run ID;<bTOF Match>", nRuns, -0.5, nRuns-0.5);

    pRunVsRefMult = new TProfile("pRunVsRefMult", ";Run ID;<RefMult>", nRuns, -0.5, nRuns-0.5);
    pRunVsRefMult3 = new TProfile("pRunVsRefMult3", ";Run ID;<RefMult3>", nRuns, -0.5, nRuns-0.5);

    #ifdef __WITH_EPD__
        pRunVsNEpdHitsEast = new TProfile("pRunVsNEpdHitsEast", ";Run ID;<EPD Hits (East)>", nRuns, -0.5, nRuns-0.5);
        pRunVsNEpdHitsWest = new TProfile("pRunVsNEpdHitsWest", ";Run ID;<EPD Hits (West)>", nRuns, -0.5, nRuns-0.5);
    #endif

    #ifdef __WITH_ETOF__
        pRunvsNETofHits = new TProfile("pRunvsNETofHits", ";Run ID;<eTOF Hits>", nRuns, -0.5, nRuns-0.5);
        pRunvsNETofDigi = new TProfile("pRunvsNETofDigi", ";Run ID;<eTOF Digi>", nRuns, -0.5, nRuns-0.5);
    #endif

    pRunVsNHitsFit = new TProfile("pRunVsNHitsFit", ";Run ID;<nHitsFit>", nRuns, -0.5, nRuns-0.5);
    pRunVsNHitsRatio = new TProfile("pRunVsNHitsRatio", ";Run ID;<nHitsRatio>", nRuns, -0.5, nRuns-0.5);
    pRunVsNHitsDedx = new TProfile("pRunVsNHitsDedx", ";Run ID;<nHitsDedx>", nRuns, -0.5, nRuns-0.5);
    pRunVsDca = new TProfile("pRunVsDca", ";Run ID;<DCA> [cm]", nRuns, -0.5, nRuns-0.5);
    pRunVsDcaZ = new TProfile("pRunVsDcaZ", ";Run ID;<DCAz> [cm]", nRuns, -0.5, nRuns-0.5);
    pRunVssDcaXY = new TProfile("pRunVssDcaXY", ";Run ID;<sDCAxy> [cm]", nRuns, -0.5, nRuns-0.5);
    pRunVsPt = new TProfile("pRunVsPt", ";Run ID;<p_{T}> [GeV/c]", nRuns, -0.5, nRuns-0.5);
    pRunVsEta = new TProfile("pRunVsEta", ";Run ID;<#eta>", nRuns, -0.5, nRuns-0.5);
    pRunVsPhi = new TProfile("pRunVsPhi", ";Run ID;<#phi>", nRuns, -0.5, nRuns-0.5);
    pRunVsBTof1OverBeta = new TProfile("pRunVsBTof1OverBeta", ";Run ID;bTOF <1/#beta>", nRuns, -0.5, nRuns-0.5, -5, 5);

    #ifdef __WITH_EPD__
        pRunVsNMipEast = new TProfile("pRunVsNMipEast", ";Run ID;<EPD MIP (East)>", nRuns, -0.5, nRuns-0.5);
        pRunVsNMipWest = new TProfile("pRunVsNMipWest", ";Run ID;<EPD MIP (West)>", nRuns, -0.5, nRuns-0.5);
        pRunVsTNMipEast = new TProfile("pRunVsTNMipEast", ";Run ID;<EPD Tunk. MIP (East)>", nRuns, -0.5, nRuns-0.5);
        pRunVsTNMipWest = new TProfile("pRunVsTNMipWest", ";Run ID;<EPD Tunk. MIP (West)>", nRuns, -0.5, nRuns-0.5);
    #endif

    #ifdef __WITH_ETOF__
        pRunVsETof1OverBeta = new TProfile("pRunVsETof1OverBeta", ";Run ID;eTOF <1/#beta>", nRuns, -0.5, nRuns-0.5, -5, 5);
    #endif

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
    hNBTofMatch->Write();

    #ifdef __WITH_EPD__
        hNEpdHitsEast->Write();
        hNEpdHitsWest->Write();
    #endif

    #ifdef __WITH_ETOF__
        hNETofDigis->Write();
        hNETofHits->Write();
    #endif

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
    h2ProtonPtY->Write();
    h2ProtonPtYbTOF->Write();


    #ifdef __WITH_EPD__
        hNEpdMipEast->Write();
        hNEpdMipWest->Write();
    #endif

    #ifdef __WITH_ETOF__
        h2RigiVsETof1OverBeta->Write();
        h2RigiVsETofMass2->Write();
        h2ProtonPtYeTOF->Write();
    #endif

    // Profiles

    // Event-wise

    pRunVsVx->Write();
    pRunVsVy->Write();
    pRunVsVz->Write();
    pRunVsVr->Write();
    pRunVsBbcX->Write();
    pRunVsZdcX->Write();
    pRunvsNBTofHits->Write();
    pRunvsNBTofMatch->Write();

    pRunVsRefMult->Write();
    pRunVsRefMult3->Write();
    
    #ifdef __WITH_EPD__
        pRunVsNEpdHitsEast->Write();
        pRunVsNEpdHitsWest->Write();
    #endif

    #ifdef __WITH_ETOF__
        pRunvsNETofHits->Write();
        pRunvsNETofDigi->Write();
    #endif

    // Track-wise

    pRunVsNHitsFit->Write();
    pRunVsNHitsRatio->Write();
    pRunVsNHitsDedx->Write();
    pRunVsDca->Write();
    pRunVsDcaZ->Write();
    pRunVssDcaXY->Write();
    pRunVsPt->Write();
    pRunVsEta->Write();
    pRunVsPhi->Write();
    pRunVsBTof1OverBeta->Write();
    
    #ifdef __WITH_EPD__
        pRunVsNMipEast->Write();
        pRunVsNMipWest->Write();
        pRunVsTNMipEast->Write();
        pRunVsTNMipWest->Write();
    #endif

    #ifdef __WITH_ETOF__
        pRunVsETof1OverBeta->Write();
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
   
    mPicoEvent = 0;
    mPicoTrack = 0;

    mBTofPidTraits = 0;
    
    mBTofPidTraits = 0;

    #ifdef __WITH_EPD__
        mEpdHit = 0;
    #endif

    #ifdef __WITH_ETOF__
        mETofPidTraits = 0;
    #endif

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
    auto vr = sqrt(vx * vx + (vy + 2) * (vy + 2));
    Float_t vpd_vz = mPicoEvent->vzVpd();

    h2VxVy->Fill(vx, vy);
    hTpcVz->Fill(vz);
    hVpdVz->Fill(vpd_vz);
    h2VzVpd->Fill(vz, vpd_vz);
    hNVpdHitsEast->Fill(mPicoEvent->nVpdHitsEast());
    hNVpdHitsWest->Fill(mPicoEvent->nVpdHitsWest());

    pRunVsVx->Fill(mRunId, vx);
    pRunVsVy->Fill(mRunId, vy);
    pRunVsVz->Fill(mRunId, vz);
    pRunVsVr->Fill(mRunId, vr);

    if (vr > 1.5 || fabs(vz - 200) > 5.0) { // do the vr and vz cut
        return kStOK;
    }
    h2VxVyVrCut->Fill(vx, vy);
    hTpcVzVrCut->Fill(vz);

    hNev->Fill(1);

    Float_t BBCx = mPicoEvent->BBCx();    
    Float_t ZDCx = mPicoEvent->ZDCx(); 


    hBbcX->Fill(BBCx);
    hZdcX->Fill(ZDCx);

    pRunVsBbcX->Fill(mRunId, BBCx);
    pRunVsZdcX->Fill(mRunId, ZDCx);

    hRefMult->Fill(mPicoEvent->refMult());
    hRefMult3->Fill(mPicoEvent->refMult3());

    pRunVsRefMult->Fill(mRunId, mPicoEvent->refMult());
    pRunVsRefMult3->Fill(mRunId, mPicoEvent->refMult3());

    hNBTofHits->Fill(mPicoEvent->btofTrayMultiplicity());
    hNBTofMatch->Fill(mPicoEvent->nBTOFMatch());

    pRunvsNBTofHits->Fill(mRunId, mPicoEvent->btofTrayMultiplicity());
    pRunvsNBTofMatch->Fill(mRunId, mPicoEvent->nBTOFMatch());

    #ifdef __WITH_ETOF__
        hNETofHits->Fill(mPicoEvent->etofHitMultiplicity());
        hNETofDigis->Fill(mPicoEvent->etofDigiMultiplicity());

        pRunvsNETofHits->Fill(mRunId, mPicoEvent->etofHitMultiplicity());
        pRunvsNETofDigi->Fill(mRunId, mPicoEvent->etofDigiMultiplicity());
    #endif

    mBField = mPicoEvent->bField(); // for sDCAxy

    // Make TPC and TOF issue
    const int numberOfTracks = mPicoDst->numberOfTracks();
    for (Int_t iTrack=0; iTrack<numberOfTracks; iTrack++) {
        MakeTrack(iTrack);
    }

    // Make EPD issue
    #ifdef __WITH_EPD__
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
    #endif

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

    #ifdef __WITH_ETOF__
        if (mPicoTrack->isETofTrack()) {
            mETofPidTraits = mPicoDst->etofPidTraits(mPicoTrack->eTofPidTraitsIndex());
        } else {
            mETofPidTraits = 0;
        }
    #endif

    Double_t DCA = mPicoTrack->gDCA(vx, vy, vz);
    Double_t DCAz = mPicoTrack->gDCAz(vz);
    Double_t sDCAxy = mPicoTrack->helix(mBField).geometricSignedDistance(vx, vy);
    
    hDca->Fill(DCA);
    hDcaZ->Fill(DCAz);
    hsDcaXY->Fill(sDCAxy);

    pRunVsDca->Fill(mRunId, DCA);
    pRunVsDcaZ->Fill(mRunId, DCAz);
    pRunVssDcaXY->Fill(mRunId, sDCAxy);
    
    Int_t nHitsFit = mPicoTrack->nHitsFit();
    Int_t nHitsPoss = mPicoTrack->nHitsPoss();
    Float_t nHitsRatio = 1.0 * nHitsFit / nHitsPoss;
    Int_t nHitsDedx = mPicoTrack->nHitsDedx();
    Int_t q = mPicoTrack->charge();

    hNHitsFit->Fill(nHitsFit/q);
    hNHitsDedx->Fill(nHitsDedx);
    if (nHitsRatio > 0.52) {
        hNHitsFitRatioCut->Fill(nHitsFit/q);
        hNHitsDedxRatioCut->Fill(nHitsDedx);
    }
    hNHitsRatio->Fill(nHitsRatio);

    pRunVsNHitsFit->Fill(mRunId, nHitsFit);
    pRunVsNHitsDedx->Fill(mRunId, nHitsDedx);
    pRunVsNHitsRatio->Fill(mRunId, nHitsRatio);

    Double_t nSigProton = mPicoTrack->nSigmaProton();
    Double_t nSigPion = mPicoTrack->nSigmaPion();
    Double_t nSigKaon = mPicoTrack->nSigmaKaon();

    // here use quality cut to remove bad tracks
    if (DCA > 1.0 || nHitsFit < 20 || nHitsDedx < 5 || nHitsRatio < 0.52) {
        return kStOK;
    }

    TVector3 momentum = mPicoTrack->pMom();
    Double_t pt = momentum.Perp();
    Double_t eta = momentum.PseudoRapidity();
    Double_t phi = momentum.Phi();
    Double_t p = momentum.Mag();
    Double_t pz = momentum.Z();
    Double_t EP = sqrt(p*p + 0.938272 * 0.938272);
    Double_t YP = TMath::Log((EP + pz) / (EP - pz + 1e-7)) * 0.5; 

    hPt->Fill(pt);
    hEta->Fill(eta);
    hPhi->Fill(phi);

    h2ProtonPtY->Fill(YP, pt);

    pRunVsPt->Fill(mRunId, pt);
    pRunVsEta->Fill(mRunId, eta);
    pRunVsPhi->Fill(mRunId, phi);

    Double_t bTofBeta = GetBTofBeta();
    
    #ifdef __WITH_ETOF__
        Double_t eTofBeta = GetETofBeta();
    #endif

    Double_t rigi = p / q;

    h2RigiVsDedx->Fill(rigi, mPicoTrack->dEdx());
    h2RigiVsNSigmaProton->Fill(rigi, nSigProton);
    h2RigiVsNSigmaPion->Fill(rigi, nSigPion);
    h2RigiVsNSigmaKaon->Fill(rigi, nSigKaon);

    if (bTofBeta > 1e-5) {
        Double_t m2b = p*p * (pow(bTofBeta, -2.0) - 1);
        h2RigiVsBTof1OverBeta->Fill(rigi, 1.0 / bTofBeta);
        h2RigiVsBTofMass2->Fill(rigi, m2b);
        h2BTofMass2VsNSigmaProton->Fill(m2b, nSigProton);
        
        h2ProtonPtYbTOF->Fill(YP, pt);

        pRunVsBTof1OverBeta->Fill(mRunId, 1.0 / bTofBeta);
    }

    #ifdef __WITH_ETOF__
        if (eTofBeta > 1e-5) {
            Double_t m2e = p*p * (pow(eTofBeta, -2.0) - 1);
            h2RigiVsETof1OverBeta->Fill(rigi, 1.0 / eTofBeta);
            h2RigiVsETofMass2->Fill(rigi, m2e);
            h2ETofMass2VsNSigmaProton->Fill(m2e, nSigProton);
            
            h2ProtonPtYeTOF->Fill(YP, pt);
            
            pRunVsETof1OverBeta->Fill(mRunId, 1.0 / eTofBeta);
        }
    #endif

    return kStOK;
}

Bool_t StYQAMaker::IsGoodTrigger() {
    for (const UInt_t& trg : DbConf::mTriggers) {
        if (mPicoEvent->isTrigger(trg)) {
            return kTRUE;
        }
    }
    return kFALSE;
}

Double_t StYQAMaker::GetBTofBeta() {
    if (!mPicoTrack->isTofTrack() || !mBTofPidTraits || mBTofPidTraits->btofMatchFlag() <= 0) {
        return 0.0;
    }
    Double_t beta = mBTofPidTraits->btofBeta();
    return beta < 1e-5 ?  0.0 : beta;
}

#ifdef __WITH_EPD__
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

        if (IsEast) {
            mNEpdHitsEast++;
        } else {
            mNEpdHitsWest++;
        }

        if (!mEpdHit->isGood()) {
            return kStOK;
        }

        const Int_t nMip = mEpdHit->nMIP();
        const Double_t tnMip = mEpdHit->TnMIP(2.0, 0.3); // is also the default args

        if (IsEast) {
            hNEpdMipEast->Fill(nMip);

            pRunVsNMipEast->Fill(mRunId, nMip);
            pRunVsTNMipEast->Fill(mRunId, tnMip);
        } else {
            hNEpdMipWest->Fill(nMip);

            pRunVsNMipWest->Fill(mRunId, nMip);
            pRunVsTNMipWest->Fill(mRunId, tnMip);
        }

        return kStOK;
    }

#endif

#ifdef __WITH_ETOF__
    Double_t StYQAMaker::GetETofBeta() {
        if (!mPicoTrack->isETofTrack() || !mETofPidTraits || mETofPidTraits->matchFlag() <= 0) {
            return 0.0;
        }
        Double_t beta = mETofPidTraits->beta();
        return beta < 1e-5 ?  0.0 : beta;
    }
#endif


