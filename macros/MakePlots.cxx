void MakePlots() {
	bool savePDF = false;
	bool savePNG = true;
	bool drawHistogram = true;
	bool drawProfiles = true;

	if (!savePDF && !savePNG) { savePNG = true; } // at least we save png file
	if (!drawHistogram && !drawProfiles) { drawHistogram = true; } // at least we draw histograms

	gStyle->SetLabelSize(0.05, "xyz");
	gStyle->SetLabelFont(62, "xyz");

	gStyle->SetTickLength(0.03, "xyz");
	gStyle->SetNdivisions(505, "xyz");
	gStyle->SetPadTickX(1);
	gStyle->SetPadTickY(1);

	gStyle->SetTitleSize(0.05, "xyz");
	gStyle->SetTitleFont(62, "xyz");
	gStyle->SetTitleOffset(1.2, "xyz");

	gStyle->SetFrameLineWidth(2);
	gStyle->SetHistLineWidth(2);
	gStyle->SetFuncWidth(2);

	gStyle->SetCanvasBorderMode(1);
	gStyle->SetFrameBorderMode(1);
	gStyle->SetPadBorderMode(1);

	gStyle->SetPadTopMargin(0.10);
	gStyle->SetPadBottomMargin(0.14);
	gStyle->SetPadLeftMargin(0.14);
	gStyle->SetPadRightMargin(0.04);

	gStyle->SetPadGridX(true);
	gStyle->SetPadGridY(true);

	gStyle->SetGridColor(kGray+1);
	gStyle->SetGridStyle(3);
	gStyle->SetGridWidth(1);

	gStyle->SetOptStat(0);

	TCanvas* const c1 = new TCanvas("c1", "", 0, 0, 800, 800);
	c1->SetLogy();
	TCanvas* const c2 = new TCanvas("c2", "", 0, 0, 800, 800);
	c2->SetLogz();
	TCanvas* const c3 = new TCanvas("c3", "", 0, 0, 800, 800);

	TFile* const fIn = new TFile("QA.hadd.root");
	const TList* const keyList = fIn->GetListOfKeys();
	for (const TObject* const& keyObject : *keyList) {
		const TKey* const key = (TKey*)keyObject;
		if (TString(key->GetClassName()).BeginsWith("TH1")) {
			c1->cd();
			gPad->SetLogy(0);
			TH1* const histogram = (TH1*)fIn->Get(key->GetName());
			histogram->UseCurrentStyle();
			histogram->Draw();
			const TString name = histogram->GetName();
			if (savePDF) { c1->SaveAs("fig/"+name+".pdf"); }
			if (savePNG) { c1->SaveAs("fig/"+name+".png"); }
			gPad->SetLogy(1);
			if (savePDF) { c1->SaveAs("fig/logy"+name+".pdf"); }
			if (savePNG) { c1->SaveAs("fig/logy"+name+".png"); }
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
