void BetterProfile() {
    const char* fNameIn = "QA.hadd.root";
    const char* fNameOut = "QA.TProfiles.root";
    
    bool showBadRunID = true;
    vector<int> runMap;
    const char* runMapFileName = "BadRunQA/Jan28.run.list";
    
    if (showBadRunID && strlen(runMapFileName) > 0) {
        ifstream inputFile(runMapFileName);
        if (!inputFile.is_open()) {
            cerr << "Error: Cannot open file " << runMapFileName << endl;
        } else {
            int runID;
            while (inputFile >> runID) {
                runMap.push_back(runID);
            }
            inputFile.close();
            cout << "Loaded " << runMap.size() << " run IDs from " << runMapFileName << endl;
        }
    }

    auto const fIn = TFile::Open(fNameIn);
    TIter next(fIn->GetListOfKeys());
	TKey* key;
    vector<TProfile*> tps;
	while ((key = (TKey*)next())) {
		const TString className = key->GetClassName();
		const TString name = key->GetName();
        if (className.BeginsWith("TProfile")) {
            auto const tp1 = (TProfile*)fIn->Get(name);
            auto tp2 = (TProfile*)tp1->Clone();
            for (int i = 1; i < tp2->GetNbinsX() + 1; i ++) {
                if (tp2->GetBinContent(i) == 0.0) { 
                    tp2->SetBinEntries(i, 0); 
                    if (showBadRunID) {
                        cout << "[BAD RUN] " << runMap[i-1] << " reason: " << tp2->GetName() << endl;
                    }
                }
            }
            tps.push_back(tp2);
        }
    }

    auto fOut = TFile::Open(fNameOut, "recreate");
    fOut->cd();
    for (const auto& item : tps) { item->Write(); }
}