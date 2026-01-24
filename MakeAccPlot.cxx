void MakeAccPlot() {
    auto f = TFile::Open("QA.hadd.root");
    TH2F* tpc;
    TH2F* b;
    TH2F* e;
    f->GetObject("h2ProtonPtY", tpc);
    f->GetObject("h2ProtonPtYbTOF", b);
    f->GetObject("h2ProtonPtYeTOF", e);

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

    auto c = new TCanvas();
    c->cd();
    gPad->SetLogz();
    auto frame = (TH2F*)gPad->DrawFrame(-1.8, -0.1, 0.2, 2.5);
    frame->SetTitle("Run26 Au+Au 4.5 GeV;Proton Rapidity (y);p_{T} (GeV/c)");
    tpc->Draw("colsame");
    b->Draw("colsame");
    e->Draw("colsame");

    TLine* line = new TLine();
    line->SetLineStyle(2);
    line->SetLineWidth(2);
    line->SetLineColor(kRed);
    line->DrawLine(-0.5, 0.4, -0.5, 2.0);
    line->DrawLine(0., 0.4, 0., 2.0);
    line->DrawLine(-0.5, 0.4, 0., 0.4);
    line->DrawLine(-0.5, 2.0, 0., 2.0);
    c->Print("acc.png");



}