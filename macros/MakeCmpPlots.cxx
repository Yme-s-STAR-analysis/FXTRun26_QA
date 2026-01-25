static void ProcessDir(TDirectory* d1, TDirectory* d2, TCanvas* c1, TCanvas* c2, TCanvas* c3, bool savePDF, bool savePNG) {
	TIter next(d1->GetListOfKeys());
	TKey* key;
	while ((key = (TKey*)next())) {
		const TString className = key->GetClassName();
		const TString name = key->GetName();

		if (className.BeginsWith("TH1")) {
			c1->Clear(); c1->Divide(1,2);
			c1->cd(1);
				gPad->SetLogy(0);
				TH1* const histogram1 = (TH1*)d1->Get(name);
				if (histogram1) { histogram1->SetTitle(Form("%s [%s]", histogram1->GetTitle(), "960000")); histogram1->UseCurrentStyle(); histogram1->Draw(); }
			c1->cd(2);
				gPad->SetLogy(0);
				TH1* const histogram2 = d2 ? (TH1*)d2->Get(name) : nullptr;
				if (histogram2) { histogram2->SetTitle(Form("%s [%s]", histogram2->GetTitle(), "960005")); histogram2->UseCurrentStyle(); histogram2->Draw(); }
			if (savePDF) { c1->SaveAs("cmpfig/"+name+".pdf"); }
			if (savePNG) { c1->SaveAs("cmpfig/"+name+".png"); }
			c1->cd(1);
				gPad->SetLogy(1);
			c1->cd(2);
				gPad->SetLogy(1);
			if (savePDF) { c1->SaveAs("cmpfig/logy"+name+".pdf"); }
			if (savePNG) { c1->SaveAs("cmpfig/logy"+name+".png"); }

		} else if (className.BeginsWith("TH2")) {
			c2->Clear(); c2->Divide(1,2);
			c2->cd(1);
				gPad->SetLogz();
				TH2* const histogram1 = (TH2*)d1->Get(name);
				if (histogram1) { histogram1->SetTitle(Form("%s [%s]", histogram1->GetTitle(), "960000")); histogram1->UseCurrentStyle(); histogram1->Draw("colz"); }
				c2->cd(2);
				gPad->SetLogz();
				TH2* const histogram2 = d2 ? (TH2*)d2->Get(name) : nullptr;
				if (histogram2) { histogram2->SetTitle(Form("%s [%s]", histogram2->GetTitle(), "960005")); histogram2->UseCurrentStyle(); histogram2->Draw("colz"); }
			if (savePDF) { c2->SaveAs("cmpfig/"+name+".pdf"); }
			if (savePNG) { c2->SaveAs("cmpfig/"+name+".png"); }

		} else if (className.BeginsWith("TProfile")) {
			c3->Clear(); c3->Divide(1,2);
			c3->cd(1);
				TProfile* const tp1 = (TProfile*)d1->Get(name);
				if (tp1) { tp1->SetTitle(Form("%s [%s]", tp1->GetTitle(), "960000")); tp1->UseCurrentStyle(); tp1->Draw(); }
			c3->cd(2);
				TProfile* const tp2 = d2 ? (TProfile*)d2->Get(name) : nullptr;
				if (tp2) { tp2->SetTitle(Form("%s [%s]", tp2->GetTitle(), "960005")); tp2->UseCurrentStyle(); tp2->Draw(); }
			if (savePDF) { c3->SaveAs("cmpfig/"+name+".pdf"); }
			if (savePNG) { c3->SaveAs("cmpfig/"+name+".png"); }

		} else if (className.BeginsWith("TDirectory")) {
			TDirectory* sub1 = (TDirectory*)d1->Get(name);
			TDirectory* sub2 = d2 ? (TDirectory*)d2->Get(name) : nullptr;
			if (sub1) ProcessDir(sub1, sub2, c1, c2, c3, savePDF, savePNG);
		}
	}
}

void MakeCmpPlots() {
	bool savePDF = false;
	bool savePNG = true;
	bool drawHistogram = true;
	bool drawProfiles = true;

	if (!savePDF && !savePNG) { savePNG = true; } // at least we save png file
	if (!drawHistogram && !drawProfiles) { drawHistogram = true; } // at least we draw histograms

	TStyle* myStyle = gStyle;

	myStyle->SetLabelSize(0.05, "xyz");
	myStyle->SetLabelFont(62, "xyz");

	myStyle->SetTickLength(0.03, "xyz");
	myStyle->SetNdivisions(505, "xyz");

	myStyle->SetTitleSize(0.05, "xyz");
	myStyle->SetTitleFont(62, "xyz");
	myStyle->SetTitleOffset(1.2, "xyz");

	myStyle->SetFrameLineWidth(2);
	myStyle->SetHistLineWidth(2);
	myStyle->SetFuncWidth(2);

	myStyle->SetCanvasBorderMode(1);
	myStyle->SetFrameBorderMode(1);
	myStyle->SetPadBorderMode(1);

	myStyle->SetPadTopMargin(0.10);
	myStyle->SetPadBottomMargin(0.14);
	myStyle->SetPadLeftMargin(0.14);
	myStyle->SetPadRightMargin(0.04);

	myStyle->SetOptStat(0);

	TCanvas* const c1 = new TCanvas("c1", "", 800, 1000);
	TCanvas* const c2 = new TCanvas("c2", "", 800, 1000);
	TCanvas* const c3 = new TCanvas("c3", "", 800, 1000);

	TFile* const fIn1 = new TFile("960000/QA.hadd.root");
	TFile* const fIn2 = new TFile("960005/QA.hadd.root");
	// process top-level directory and subdirectories recursively
	ProcessDir(fIn1, fIn2, c1, c2, c3, savePDF, savePNG);
}
