void MakePlots() {
	bool savePDF = false;
	bool savePNG = true;
	bool drawHistogram = true;
	bool drawProfiles = true;

	if (!savePDF && !savePNG) { savePNG = true; } // at least we save png file
	if (!drawHistogram && !drawProfiles) { drawHistogram = true; } // at least we draw histograms

	gStyle->SetTitleOffset(0.1, "Y");

	TCanvas* const c1 = new TCanvas("c1", "", 1);
	c1->SetLogy();
	TCanvas* const c2 = new TCanvas("c2", "", 1);
	c2->SetLogz();
	TCanvas* const c3 = new TCanvas("c3", "", 1);

	TFile* const fIn = new TFile("QA.hadd.root");
	const TList* const keyList = fIn->GetListOfKeys();
	for (const TObject* const& keyObject : *keyList) {
		const TKey* const key = (TKey*)keyObject;
		if (TString(key->GetClassName()).BeginsWith("TH1")) {
			c1->cd();
			TH1* const histogram = (TH1*)fIn->Get(key->GetName());
			histogram->UseCurrentStyle();
			histogram->Draw();
			const TString name = histogram->GetName();
			if (savePDF) { c1->SaveAs("fig/"+name+".pdf"); }
			if (savePNG) { c1->SaveAs("fig/"+name+".png"); }
		} else if (TString(key->GetClassName()).BeginsWith("TH2")) {
			c2->cd();
			TH2* const histogram = (TH2*)fIn->Get(key->GetName());
			histogram->UseCurrentStyle();
			histogram->Draw("colz");
			const TString name = histogram->GetName();
			if (savePDF) { c2->SaveAs("fig/"+name+".pdf"); }
			if (savePNG) { c2->SaveAs("fig/"+name+".png"); }
		} else if (TString(key->GetClassName()).BeginsWith("TProfile")) {
			c3->cd();
			TProfile* const tp = (TProfile*)fIn->Get(key->GetName());
			tp->UseCurrentStyle();
			tp->Draw();
			const TString name = tp->GetName();
			if (savePDF) { c3->SaveAs("fig/"+name+".pdf"); }
			if (savePNG) { c3->SaveAs("fig/"+name+".png"); }
		}
	}
}
