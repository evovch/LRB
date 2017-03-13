#include "data_structures.h"

#include <TApplication.h>
#include <TCanvas.h>
#include <TH1D.h>
#include <TGraph.h>
#include <TF1.h>
#include <TString.h>
#include <TFile.h>
#include <TTree.h>
#include <TStyle.h>
#include <TText.h>
#include <TLine.h>
#include <TBox.h>
#include <Math/DistFunc.h>

#define NPARAMS 6

// ----------------------------------------------------
// This block is required only for standalone compilation.
#ifndef __CLING__

void LRB2(TString inFilename);

int main(int argc, char** argv)
{
	if (argc != 2) {
		printf ("Please, specify only input file name.\n");
		return 1;
	}
	TString inFilename(argv[1]);

	TApplication app("LRB2", &argc, argv);

	LRB2(inFilename);

	app.Run();
	return 0;
}

#endif // __CLING__
// ----------------------------------------------------

// Lognormal distribution 1
Double_t fitFunction1 (Double_t *x, Double_t *par) {
	return par[0] * ROOT::Math::lognormal_pdf(x[0], par[1], par[2]);
}

// Lognormal distribution 2
Double_t fitFunction2 (Double_t *x, Double_t *par) {
	return par[0] * ROOT::Math::lognormal_pdf(x[0], par[1], par[2]);
}

// Geometrical distribution
Double_t fitFunction3 (Double_t *x, Double_t *par) {
	return par[0] * (pow(1.-par[1], x[0]) * par[1]);
}

Double_t fitFunctionSum (Double_t *x, Double_t *par)
{
	return fitFunction1(x, par) + fitFunction2(x, &par[3]) /*+ fitFunction3(x, &par[6])*/;
}

void LRB2(TString inFilename="data1.txt")
{
	Double_t xLow = 2.;
	Double_t xUp = 9.;

	UInt_t lineWidth = 3;
	Float_t labelSize = 0.05;
	Float_t titleSize = 0.05;

	// Open the input text file
	FILE* inputTextFile = fopen(inFilename.Data(), "r");

	if (inputTextFile == NULL) {
		printf ("Error opening input file '%s'\n", inFilename.Data());
		return;
	} else {
		printf ("Succesfully opened input file '%s'\n", inFilename.Data());
	}

	TH1D* inHisto = new TH1D("inHisto",
		"Increase in the number of new colonies;Day;Number of new colonies", 15, 0.5, 15.5);

	TGraph* inGraph = new TGraph(8);

	float col1, col2, col3;

	unsigned int readLines = 0;
	while (fscanf(inputTextFile, "%f\t%f\t%f", &col1, &col2, &col3) == 3)
	{
		printf ("Read: x=%0f\ty=%0f\terr=%0f\n", col1, col2, col3);

		inHisto->SetBinContent(col1, col2);
		inHisto->SetBinError(col1, col3);
		inGraph->SetPoint(readLines, col1, col2);

		readLines++;
	}
	printf ("Read %d entries\n", readLines);
/*
	TCanvas* canvGraph = new TCanvas("canvGraph", "canvGraph", 1200, 800);
	canvGraph->cd();
	inGraph->Draw("AL*");
	inGraph->SetLineWidth(lineWidth);
	gPad->SetGrid(1, 1);
*/

					// ksiourmen
	gStyle->SetOptStat(111111100);

	TCanvas* canv1 = new TCanvas("canv1", "canv1", 1800, 800);
	canv1->cd();
	inHisto->Draw("*H");
	inHisto->SetLineWidth(lineWidth);
	inHisto->SetLineColor(kBlack);
	inHisto->GetYaxis()->SetRangeUser(0., 45.);
	inHisto->GetXaxis()->SetLabelSize(labelSize);
	inHisto->GetYaxis()->SetLabelSize(labelSize);
	inHisto->GetXaxis()->SetTitleSize(titleSize);
	inHisto->GetYaxis()->SetTitleSize(titleSize);
	inHisto->GetXaxis()->SetTitleOffset(0.6);
	inHisto->GetYaxis()->SetTitleOffset(0.6);
	gPad->SetGrid(1, 1);

	// Fiting -----------------------------------------------------------------

	// Define the fitting function
	TF1* funcSum = new TF1("funcSum", fitFunctionSum, xLow, xUp, NPARAMS);
	funcSum->SetLineWidth(lineWidth);
	funcSum->SetLineColor(kMagenta);

	// Set initial values
	funcSum->SetParameter(0, 70); // A1
	funcSum->SetParameter(1, 1.2); // m1
	funcSum->SetParameter(2, 0.2); // s1
	funcSum->SetParameter(3, 90); // A2
	funcSum->SetParameter(4, 1.8); // m3
	funcSum->SetParameter(5, 0.2); // s4

	funcSum->SetParameter(6, 100.);
	funcSum->SetParameter(7, 0.2); // p of the geometrical distribution

	// Perform fitting
	inHisto->Fit(funcSum, "R");

	// Draw separate components -----------------------------------------------

	Double_t fitParameters[NPARAMS];
	funcSum->GetParameters(fitParameters);

	TF1* func1 = new TF1("func1", fitFunction1, xLow, xUp, 3);
	func1->SetLineColor(kRed);
	func1->SetLineStyle(1);
	func1->SetLineWidth(lineWidth);
	func1->SetParameters(fitParameters);
	func1->Draw("sames][");

	TF1* func2 = new TF1("func2", fitFunction2, xLow, xUp, 3);
	func2->SetLineColor(kGreen);
	func2->SetLineStyle(1);
	func2->SetLineWidth(lineWidth);
	func2->SetParameters(&fitParameters[3]);
	func2->Draw("sames][");

	TF1* func3 = new TF1("func3", fitFunction3, xLow, xUp, 2);
	func3->SetLineColor(kBlue);
	func3->SetLineStyle(1);
	func3->SetLineWidth(lineWidth);

	func3->SetParameter(0, 100.);
	func3->SetParameter(1, 0.2);

	//func3->SetParameters(&fitParameters[6]);
	func3->Draw("sames][");

	// ------------------------------------------------------------------------

	Double_t xStart = 10;	// 15
	Double_t yStart = 30.;	// 10
	Double_t yStep = 5.;

	TBox* rect1 = new TBox(xStart, yStart, xStart+6.5, yStart-4*yStep);
	rect1->Draw();

	// Lines
	TLine* l1 = new TLine(xStart+0.1, yStart-yStep/3, xStart+0.4, yStart-yStep/3);
	l1->SetLineColor(kRed);
	l1->SetLineStyle(1);
	TLine* l2 = new TLine(xStart+0.1, yStart-yStep/3-yStep, xStart+0.4, yStart-yStep/3-yStep);
	l2->SetLineColor(kGreen);
	l2->SetLineStyle(1);
	TLine* l3 = new TLine(xStart+0.1, yStart-yStep/3-2*yStep, xStart+0.4, yStart-yStep/3-2*yStep);
	l3->SetLineColor(kBlue);
	l3->SetLineStyle(1);

	l1->SetLineWidth(lineWidth);
	l2->SetLineWidth(lineWidth);
	l3->SetLineWidth(lineWidth);
	l1->Draw();
	l2->Draw();
	l3->Draw();

	// Text
	TString theText;

	theText.Form("LogNorm1: A=%0.3f u=%0.3f s=%0.3f",
		func1->GetParameter(0), func1->GetParameter(1), func1->GetParameter(2));
	TText *t1 = new TText(xStart+0.5, yStart-yStep/2., theText);

	theText.Form("LogNorm2: A=%0.3f u=%0.3f s=%0.3f",
		func2->GetParameter(0), func2->GetParameter(1), func2->GetParameter(2));
	TText *t2 = new TText(xStart+0.5, yStart-yStep/2.-1*yStep, theText);

	theText.Form("Geom: A=%0.3f p=%0.3f",
		func3->GetParameter(0), func3->GetParameter(1));
	TText *t3 = new TText(xStart+0.5, yStart-yStep/2.-2*yStep, theText);

	t1->SetTextSize(0.04);
	t2->SetTextSize(0.04);
	t3->SetTextSize(0.04);
	t1->Draw();
	t2->Draw();
	t3->Draw();

	// ------------------------------------------------------------------------

	return;
}
