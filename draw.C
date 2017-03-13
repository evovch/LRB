#include "data_structures.h"

void draw(TString inFilename="outTreeFile.root")
{

	//TApplication theApp("Analysis", &argc, argv);

	// Get the file
	TFile* dataFile = new TFile(inFilename, "READ");

	// Get the only tree of the file
	TTree* dataTree = (TTree*)dataFile->Get("dataTree");

	// Get the only branch of the tree and map the branch to the structure
	struct data_from_file dataObject;
	TBranch* recDataBranch = dataTree->GetBranch("recordedData");
	recDataBranch->SetAddress(&dataObject);

	// Get the number of entries in the tree
	unsigned int numOfEntries = dataTree->GetEntries();

	// Prepare histograms
	TH1D* histo1 = new TH1D("col1", "col1;;Entries", 1000, 0., 100.);
	TH1D* histo2 = new TH1D("col2", "col2;;Entries", 1000, 0., 100.);
	TH1D* histo3 = new TH1D("col3", "col3;;Entries", 1000, 0., 100.);

	for (unsigned int i=0; i<numOfEntries; i++) {
		recDataBranch->GetEntry(i);
		histo1->Fill(dataObject.col1);
		histo2->Fill(dataObject.col2);
		histo3->Fill(dataObject.col3);
	}

	TCanvas* canv = new TCanvas("canv", "canv");
	canv->Divide(2, 2);

	canv->cd(1);
	histo1->Draw();
	
	gPad->SetGrid(1, 1);

	canv->cd(2);
	histo2->Draw();
	
	gPad->SetGrid(1, 1);

	canv->cd(3);
	histo3->Draw();

	gPad->SetGrid(1, 1);

	//theApp.Run();

	gStyle->SetOptStat(1111111);

	return;
}