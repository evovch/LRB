#include "data_structures.h"

#include <TApplication.h>
#include <TCanvas.h>
#include <TH1D.h>
#include <TF1.h>
#include <TString.h>
#include <TFile.h>
#include <TTree.h>

// ----------------------------------------------------
// This block is required only for standalone compilation.
#ifndef __CLING__

void LRB1(TString inFilename);

int main(int argc, char** argv)
{
	if (argc != 2) {
		printf ("Please, specify only input file name.\n");
		return 1;
	}
	TString inFilename(argv[1]);

	//TApplication app("LRB1", &argc, argv);

	LRB1(inFilename);

	//app.Run();
	return 0;
}

#endif // __CLING__
// ----------------------------------------------------

void LRB1(TString inFilename="data1.txt")
{
	// Open the input text file
	FILE* inputTextFile = fopen(inFilename.Data(), "r");

	if (inputTextFile == NULL) {
		printf ("Error opening input file '%s'\n", inFilename.Data());
		return;
	} else {
		printf ("Succesfully opened input file '%s'\n", inFilename.Data());
	}

	struct data_from_file dataObject;
	TFile* dataFile = new TFile("outTreeFile.root", "RECREATE");
	TTree* dataTree = new TTree("dataTree", "dataTree");
	dataTree->Branch("recordedData", &dataObject.col1, "col1/F:col2:col3");	

	float col1, col2, col3;

	unsigned int readLines = 0;
	while (fscanf(inputTextFile, "%f\t%f\t%f",
	       &col1, &col2, &col3) == 3)
	{
		dataObject.col1 = col1;
		dataObject.col2 = col2;
		dataObject.col3 = col3;
		dataTree->Fill();
		readLines++;
	}
	printf ("Read %d entries\nCreated output file 'outTreeFile.root'\n", readLines);

	// Write output ROOT file with received data and close it
	dataFile->Write();
	dataFile->Close();

	return;
}