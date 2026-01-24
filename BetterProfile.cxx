void BetterProfile() {
    const char* fNameIn = "QA.hadd.root";
    const char* fNameOut = "QA.TProfiles.root";

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
                if (tp2->GetBinContent(i) == 0.0) { tp2->SetBinEntries(i, 0); }
            }
            tps.push_back(tp2);
        }
    }

    auto fOut = TFile::Open(fNameOut, "recreate");
    fOut->cd();
    for (const auto& item : tps) { item->Write(); }
}