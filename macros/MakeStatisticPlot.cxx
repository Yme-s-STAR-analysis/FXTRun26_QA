void MakeStatisticPlot() {
    auto f = TFile::Open("QA.hadd.root");
    TH1F* hNev;

    hNev = (TH1F*)f->Get("hNev");

	gStyle->SetLabelSize(0.05, "xyz");
	gStyle->SetLabelFont(62, "xyz");

	gStyle->SetTickLength(0.03, "xyz");
	// gStyle->SetNdivisions(505, "xyz");
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

	hNev->GetXaxis()->SetTitle(" ");

	vector<double> nEv;
	vector<double> rEv;
	auto nTot = hNev->GetBinContent(1);
	for (int i=1; i<=hNev->GetNbinsX(); i++) {
		nEv.push_back((double)hNev->GetBinContent(i));
		rEv.push_back((double)hNev->GetBinContent(i) / 1.0 / nTot);
	}

	// cout << "[LOG] Total number of events: " << nTot << endl;
	// cout << "[LOG] w/ good trigger cut: " << nTrg << " (" << rTrg * 100 << "%)" << endl;
	// cout << "[LOG] w/ good vertex cut: " << nVtx << " (" << rVtx * 100 << "%)" << endl;
    auto c = new TCanvas();
    c->cd();
	gPad->SetLogy();
	hNev->Draw("hist");
	TLatex* lat = new TLatex();
	lat->SetTextSize(0.04);
	lat->SetTextAlign(21);
	for (int i=0; i<hNev->GetNbinsX(); i++) {
		lat->DrawLatex(i, nEv[i] * 1.12, Form("%.2e", nEv[i]*1.0));
	}
	lat->SetTextAlign(23);
	for (int i=0; i<hNev->GetNbinsX(); i++) {
		lat->DrawLatex(i, nEv[i] * 0.98, Form("%.1f %%", rEv[i]*100));
	}

    c->Print("stat.png");
}