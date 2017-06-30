
#include "G4Timer.hh"
#include "LTAC1RunAction.hh"
#include "G4Run.hh"
#include "G4UnitsTable.hh"
#include "G4ios.hh"
#include "LTAC1RunActionMessenger.hh"
#include "G4EmCalculator.hh"
#include "G4Electron.hh"
#include "LTAC1DetectorConstruction.hh"
#include "G4Integrator.hh"
using namespace std;

LTAC1RunAction::LTAC1RunAction(LTAC1DetectorConstruction * det)
	:detector(det), PrintCount(0)
{	
	timer.Start();		// Start timer
	RunMessenger = new LTAC1RunActionMessenger(this); // Define user commands
	G1Emin = 1.0*MeV;	// Gate can be changed during run time, eg. "/LTAC/run/SetGate1Low 1000. keV"
	G1Emax = 1.4*MeV;	// Gate can be changed during run time, eg. "/LTAC/run/SetGate1High 1400. keV"
	G2Emin = 2.0*MeV;	// Gate can be changed during run time.
	G2Emax = 3.0*MeV;	// Gate can be changed during run time.	
	G3Emin = 3.0*MeV;	// Gate can be changed during run time.
	G3Emax = 5.0*MeV;	// Gate can be changed during run time. 

	NaIEmax = .20*MeV;	// Max LS Energy for binning. Saturation goes in max energy bin.
	NaIEmin = 0.0*MeV;	// Min LS Energy for binning. Keep this at zero.
	NaIBins = 2000;		// Number of bins in NaI spectra. Does not affect gating resolution.

	OkToSave2D = true;		// Shall we save the 2D spectra? (few MB each).


}


LTAC1RunAction::~LTAC1RunAction()
{  
	delete RunMessenger;
}

void LTAC1RunAction::BeginOfRunAction(const G4Run* aRun)
{
	Nhistories = aRun->GetNumberOfEventToBeProcessed();    // used by event handler for progress bar
//==..==..==..==..==..==..==..==..==..==..==..==..==..==..==..==..==..==..==..//
// CREATE SPECTRA: They will be filled at each event in LTAC1EventAction.cc  //
//==..==..==..==..==..==..==..==..==..==..==..==..==..==..==..==..==..==..==..//

	NaI1Histogram.Build("NaI1Hist", NaIBins,NaIEmin,NaIEmax);		// Set name, number of energy of bins
	NaI2Histogram.Build("NaI2Hist", NaIBins,NaIEmin,NaIEmax);		// *TO DO _ VARIABLES Set name, number of energy of bins
	TheHistogram2D.Build("Hist2D",NaIBins, NaIEmin, NaIEmax, NaIBins, NaIEmin, NaIEmax);

	
	Windows[0].Build("Win1+2",0, 0, 0);								// Win0 is sum of other windows' counts
	Windows[1].Build("Win1",G1Emin, G1Emax, 0);					// Set windows from user input
	Windows[2].Build("Win2",G2Emin, G2Emax, 0);
	Windows[3].Build("Win1V2",G3Emin, G3Emax, 0);


	for (int i = 0; i<=3; i++)
	{
	NaI1Histogram.Windows[i]=Windows[i];
	NaI2Histogram.Windows[i]=Windows[i];							// Copy windows to both histograms
	TheHistogram2D.Windows[i]=Windows[i];
	}
	TheHistogram2D.Windows[1].Build("Win1",G1Emin, G1Emax,0);	
}
 

void LTAC1RunAction::EndOfRunAction(const G4Run*)
{

  G4String PN = GetParticleName();	// This is found and set during the first event, in LTAC1EventAction.cc
									// It is the name of the radioactive particle, eg. Co60
  NaI1Histogram.Print(PN, Nhistories);
  NaI2Histogram.Print(PN, Nhistories);

  G4cout << "NaI1 Window 1 FP " << NaI1Histogram.Windows[1].Counts << G4endl;
  G4cout << "NaI2 Window 1 FP " << NaI2Histogram.Windows[1].Counts << G4endl;
  G4cout << "NaI2D Window 1 FP " << TheHistogram2D.Windows[1].Counts << G4endl;
  G4cout << "NaI1 Window 1 Tot " << NaI1Histogram.Windows[2].Counts << G4endl;
  G4cout << "NaI2 Window 1 Tot " << NaI2Histogram.Windows[2].Counts << G4endl;
  G4cout << "NaI2D Window 1 Tot " << TheHistogram2D.Windows[2].Counts << G4endl;
  PrintWindows(PN, Nhistories, &NaI1Histogram, &NaI2Histogram, &TheHistogram2D);

  if ( OkToSave2D == true)			// if 2D saving is enabled, save 2D spectrum
  TheHistogram2D.Print(PN, Nhistories);

  timer.Stop();						// Stop timing how long the run took

  if (timer.IsValid())				// Print to screen timer data.
  {
  G4double dt = timer.GetUserElapsed()*s;
  G4cout << "Elapsed time: " <<G4BestUnit(dt,"Time") << G4endl;
  G4cout << "time per history: " <<G4BestUnit((dt/Nhistories),"Time") << G4endl;
  }
  else{G4cout << "ERROR: TIMER IS INVALID" << G4endl;} 

}
//

//
//

 void LTAC1RunAction::PrintWindows(G4String PN, G4int NH, Histogram *Hist1, 
	 Histogram *Hist2, Histogram2D* H2D)

{
G4String sep1 = "\t";
G4String path = "out/";					// Save output file in directory "out"
G4String ext = ".csv";
G4String fname = "Scalars" + ext;		// Name output file Scalars.csv
time_t rawtime;
struct tm  timeinfo;					// Get Timestamp
time (&rawtime);
char buffer [80];
timeinfo = *localtime ( &rawtime) ;
strftime (buffer,80,"_%Y-%m-%d-%H_%M_%S",&timeinfo);

G4String gname = path + "Scalars" + "_" + PN + buffer + ext;
ofstream fileout[2];
fileout[0].open(fname);			// Write data to a file in running directory. 
fileout[1].open(gname);			// Also write data to a Timestamped file in "out" directory.

for (int j = 0; j<=1; j++)
  {
    fileout[j] << "# Particle Name: " << PN << G4endl << "# NHistories: " << NH << G4endl;	// headder
    fileout[j] << sep1 << "NaI1" << sep1 << "NaI2" << sep1 << "coinc" << sep1 << "s/c" << sep1<< "c/s"<<G4endl;

    for ( int i=0; i <= 3; i++ ) {
		G4int c1 = Hist1->Windows[i].Counts;
		G4int c2 = Hist2->Windows[i].Counts;
		G4int cc = H2D->Windows[i].Counts;
		G4double ratio = (c1+c2)/2.0/cc;
		fileout[j] << Hist1->Windows[i].Name.c_str() << sep1 << c1 << sep1 << c2 << sep1 << cc << sep1
			<< ratio << sep1 << 1.0/ratio <<G4endl;}
    fileout[j].close();
  }
}

