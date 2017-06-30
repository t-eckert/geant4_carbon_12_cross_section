
#ifndef LTAC1RunAction_h
#define LTAC1RunAction_h 1
#include "G4Timer.hh"
#include "G4UserRunAction.hh"
#include "globals.hh"
#include "LTAC1Analysis.hh"
#include "LTAC1RunActionMessenger.hh"
#include "LTAC1DetectorConstruction.hh"
#include "G4EmCalculator.hh"
#include "G4Integrator.hh"

class G4Run;
class DetectorConstruction;


class LTAC1RunAction : public G4UserRunAction
{
  public:
    LTAC1RunAction(LTAC1DetectorConstruction *);
   virtual ~LTAC1RunAction();
   G4double Birks(G4double) ;
   void LTAC1RunAction::MakeQTable();
private:
	
	G4int PrintCount;		// counter for indexing output files. Use messenger.	
	G4EmCalculator  EmCalc;
	G4int Nhistories;
	G4String ParticleName;

	G4double G1Emin;			// Make more NaI Gates as needed //
	G4double G1Emax;
	G4double G2Emin;
	G4double G2Emax;
	G4double G3Emin;
	G4double G3Emax;


	G4int NaIBins;		// Number of bins in NaI spectrum
	G4double NaIEmax;	// Energy of maximum bin in NaI spectra. Saturation peak there.
	G4double NaIEmin;	// Energy of minimum bin in NaI spectra.

	G4double NaIBins2D;	// Now do same for 2D spectrum.
	G4double NaIEmin2D;
	G4double NaIEmax2D;
;

	bool OkToSave2D;

	LTAC1RunActionMessenger* RunMessenger;	// run messenger. Set gates etc.
	LTAC1DetectorConstruction * detector;

  public:
	G4Run* aRun;		// Use run to get number of histories to be run

	void SetXoffset(G4double offset) {detector->SetSourceXoffset(offset);}
	void SetYoffset(G4double offset) {detector->SetSourceYoffset(offset);}
	void SetZoffset(G4double offset) {detector->SetSourceZoffset(offset);}
	void SetDetectorGap(G4double theGap) {detector->SetDetectorGap(theGap);}
	void SetMaterial(const G4String & materialName) {detector->SetMaterialName(materialName);}
	G4Timer timer;		// User timer to monitor how long run takes

	void SetNaIEmax(G4double Emax)	{NaIEmax = Emax;}
	void SetNaIBins(G4int Nbins)	{NaIBins = Nbins;}
	
	G4double GetNaIEmax(){return NaIEmax;}


	void SetG1(G4double E1, G4double E2) {G1Emin = E1; G1Emax = E2;}
	void SetG2(G4double E1, G4double E2) {G2Emin = E1; G2Emax = E2;}
	void SetG3(G4double E1, G4double E2) {G3Emin = E1; G3Emax = E2;}

	G4int GetPrintCount(){return PrintCount;}
	void SetPrintCount(G4int count){PrintCount=count;}

	G4double GetG1Low(){return G1Emin;}
	G4double GetG1High(){return G1Emax;}
	G4double GetG2Low(){return G2Emin;}
	G4double GetG2High(){return G2Emax;}
	G4double GetG3Low(){return G3Emin;}
	G4double GetG3High(){return G3Emax;}

	Histogram NaI1Histogram;
	Histogram NaI2Histogram;
	Integral  NaI1Integral;


	Histogram2D TheHistogram2D; // 2D histogram
	void SetOkToSave2D(bool tf){OkToSave2D = tf;}
	bool GetOkToSave2D(){return OkToSave2D;}

	Gate Gate1; 	// Make more NaI Gates as needed.
	Gate Gate2;
	Gate Gate3;
	Window Windows[4];
	Window CoincWin[4];



	G4int GetNhistories(){return Nhistories;}
	G4String GetParticleName(){return ParticleName;}
	void SetParticleName(G4String PName){ParticleName=PName;}

	void FillCoinWin(G4double E1, G4double E2);    
	void BeginOfRunAction(const G4Run*);
    void EndOfRunAction(const G4Run*);
	void PrintWindows(G4String PN, G4int NH, Histogram *Hist1,Histogram *Hist2, Histogram2D* H2D);
};


// keep at end
#endif // keep
