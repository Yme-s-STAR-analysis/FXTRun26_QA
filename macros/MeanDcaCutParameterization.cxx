void SetStyle() {
	gStyle->SetLabelSize(0.05, "xyz");
	gStyle->SetLabelFont(62, "xyz");

	gStyle->SetTickLength(0.03, "xyz");
	gStyle->SetNdivisions(505, "xyz");
	gStyle->SetPadTickX(1);
	gStyle->SetPadTickY(1);

	gStyle->SetTitleSize(0.05, "xyz");
	gStyle->SetTitleFont(62, "xyz");
	// gStyle->SetTitleOffset(1.2, "xyz");

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
}

void MeanDcaCutParameterization() {
    auto tf = new TFile("QA.hadd.root");

    auto c = new TCanvas();
    const char* energy = "4.5";

    TH2F *h2z, *h2xy;

    auto leg = new TLegend(0.6, 0.1, 0.85, 0.33);

    TF1* z_upper = new TF1("z_upper", "[0]+[1]/pow(x, [2])", 1, 600);
    TF1* xy_upper = new TF1("xy_upper", "[0]+[1]/pow(x, [2])", 1, 600);
    TF1* z_lower = new TF1("z_lower", "[0]+[1]/pow(x, [2])", 1, 600);
    TF1* xy_lower = new TF1("xy_lower", "[0]+[1]/pow(x, [2])", 1, 600);

    double init_par_upper[3] = {1.0, 1.0, 2.0};
    double init_par_lower[3] = {1.0, -1.0, 2.0};

    z_upper->SetParameters(init_par_upper);
    xy_upper->SetParameters(init_par_upper);
    z_lower->SetParameters(init_par_lower);
    xy_lower->SetParameters(init_par_lower);

    tf->GetObject("h2FXTMultDCA3sDCAz", h2z);
    tf->GetObject("h2FXTMultDCA3sDCAxy", h2xy);

    TLatex* lat = new TLatex();
    lat->SetTextFont(64);
    lat->SetTextSize(16);
    lat->SetTextColor(2);
    lat->SetTextAlign(22);

    const char* sname = "MeanDcaCuts.pdf";
    double nsig = 6;

    c->Clear();
    c->cd();
    lat->DrawLatexNDC(0.5, 0.5, Form("Au+Au @ %s GeV <sDCAxy/z> cut", energy));
    c->Print(Form("%s(", sname));

    // do fit
    const int np = 120; // depends on the FxtMult_DCA3 (i.e. X-axis)
    // make sure that np is less than number of X bins
    double mean[np];
    double sig[np];
    double upper[np];
    double lower[np];
    double x[np];

    ofstream fout;
    fout.open("MeanDcaCutParams.txt");

    c->Clear();
    c->cd();
    SetStyle();
    gPad->SetLogy(false);
    gPad->SetLogz();
    h2z->GetYaxis()->SetRangeUser(-3, 3);
    h2z->SetTitle(";FXTMult_DCA3;<sDCAz> [cm]");
    h2z->Draw("col");
    lat->SetTextAlign(11);
    lat->DrawLatexNDC(0.7, 0.8, "STAR BES-II");
    lat->DrawLatexNDC(0.7, 0.7, Form("Au+Au @ %s GeV", energy));
    for (int i=0; i<np; i++) {
        // mean[i] = tpz->GetBinContent(i+1);
        // sig[i] = tpz->GetBinError(i+1);
        TH1D* hProjY = h2z->ProjectionY(Form("proj_%d", i), i+1, i+1);
        if (hProjY->GetEntries() > 0) {
            mean[i] = hProjY->GetMean();
            sig[i] = hProjY->GetStdDev();
        } else {
            mean[i] = 0;
            sig[i] = 0;
        }
        upper[i] = mean[i] + sig[i]*nsig;
        lower[i] = mean[i] - sig[i]*nsig;
        x[i] = i+1;
    }
    TGraph* tgu = new TGraph(np, x, upper);
    TGraph* tgl = new TGraph(np, x, lower);

    tgu->SetMarkerStyle(29);
    tgu->SetMarkerColor(1);
    tgl->SetMarkerStyle(29);
    tgl->SetMarkerColor(1);
    tgu->Fit(z_upper, "RQ0");
    tgl->Fit(z_lower, "RQ0");

    tgu->Draw("psame");
    tgl->Draw("psame");
    z_upper->Draw("lsame");
    z_lower->Draw("lsame");

    leg->AddEntry(z_upper, Form("#mu#pm%.0f#sigma cut", nsig), "l");
    // leg->AddEntry(tpz, "#mu#pm#sigma", "ep");

    leg->Draw("same");

    std::cout << "[LOG] parameters: \n DCAz upper: {" << z_upper->GetParameter(0) << ", " << z_upper->GetParameter(1) << ", " << z_upper->GetParameter(2) << "}" << std::endl;
    std::cout << "[LOG] parameters: \n DCAz lower: {" << z_lower->GetParameter(0) << ", " << z_lower->GetParameter(1) << ", " << z_lower->GetParameter(2) << "}" << std::endl;

    fout << "DCAz pars:\n";
    fout << "{" << z_upper->GetParameter(0) << ", " << z_upper->GetParameter(1) << ", " << z_upper->GetParameter(2) << "}" << std::endl;
    fout << "{" << z_lower->GetParameter(0) << ", " << z_lower->GetParameter(1) << ", " << z_lower->GetParameter(2) << "}" << std::endl;

    c->Print(sname);
    c->Print("MeanDcazCut.png");

    c->Clear();
    c->cd();
    SetStyle();
    gPad->SetLogy(false);
    gPad->SetLogz();
    h2xy->GetYaxis()->SetRangeUser(-3, 3);
    h2xy->SetTitle(";FXTMult_DCA3;<sDCAxy> [cm]");
    h2xy->Draw("col");
    lat->SetTextAlign(11);
    lat->DrawLatexNDC(0.7, 0.8, "STAR BES-II");
    lat->DrawLatexNDC(0.7, 0.7, Form("Au+Au @ %s GeV", energy));
    for (int i=0; i<np; i++) {
        // mean[i] = tpxy->GetBinContent(i+1);
        // sig[i] = tpxy->GetBinError(i+1);
        TH1D* hProjY = h2xy->ProjectionY(Form("proj_%d", i), i+1, i+1);
        if (hProjY->GetEntries() > 0) {
            mean[i] = hProjY->GetMean();
            sig[i] = hProjY->GetStdDev();
        } else {
            mean[i] = 0;
            sig[i] = 0;
        }
        upper[i] = mean[i] + sig[i]*nsig;
        lower[i] = mean[i] - sig[i]*nsig;
        x[i] = i+1;
    }
    TGraph* tgu2 = new TGraph(np, x, upper);
    TGraph* tgl2 = new TGraph(np, x, lower);

    tgu2->SetMarkerStyle(29);
    tgu2->SetMarkerColor(1);
    tgl2->SetMarkerStyle(29);
    tgl2->SetMarkerColor(1);
    
    tgu2->Fit(xy_upper, "RQ0");
    tgl2->Fit(xy_lower, "RQ0");

    tgu2->Draw("psame");
    tgl2->Draw("psame");

    xy_upper->Draw("lsame");
    xy_lower->Draw("lsame");

    leg->Draw("same");

    std::cout << "[LOG] parameters: \n DCAxy upper: {" << xy_upper->GetParameter(0) << ", " << xy_upper->GetParameter(1) << ", " << xy_upper->GetParameter(2) << "}" << std::endl;
    std::cout << "[LOG] parameters: \n DCAxy lower: {" << xy_lower->GetParameter(0) << ", " << xy_lower->GetParameter(1) << ", " << xy_lower->GetParameter(2) << "}" << std::endl;

    fout << "DCAxy pars:\n";
    fout << "{" << xy_upper->GetParameter(0) << ", " << xy_upper->GetParameter(1) << ", " << xy_upper->GetParameter(2) << "}" << std::endl;
    fout << "{" << xy_lower->GetParameter(0) << ", " << xy_lower->GetParameter(1) << ", " << xy_lower->GetParameter(2) << "}" << std::endl;

    c->Print(sname);
    c->Print("MeanDcaxyCut.png");

    c->Clear();
    c->Print(Form("%s)", sname));

    fout.close();

}