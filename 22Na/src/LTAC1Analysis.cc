
//
#include "LTAC1Analysis.hh"
#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"
#include <fstream>
#include <iostream>
#include "G4ios.hh"
#include "G4UnitsTable.hh"
#include <ctime>
using namespace std;

int Histogram::nHistograms=0;	// Initialize total number of histograms made.

void PrintExtrapolation(Gate Gates[],int nGates, G4String PN, G4int NH)
// PRINT to output files the LS Integral and Gates inefficiencies.
{
Integral * LSInteg = Gates[0].GetNaI1Integral();
int NChannels = Gates[0].GetNChannels();


G4String sep1 = "\t";
G4String path = "out/";
G4String ext = ".csv";
G4String name = LSInteg->GetHistName();
G4String fname = name + ext;	// Name output file from Histogram name, with .csv extension
time_t rawtime;
struct tm  timeinfo;
time (&rawtime);
char timetxt [80];
timeinfo = *localtime ( &rawtime) ;
strftime (timetxt,80,"_%Y-%m-%d-%H_%M_%S",&timeinfo);

G4String gname = path + name + "_" + PN + timetxt + ext;
ofstream fileout[2];
fileout[0].open(fname);			// Write data to a file in running directory. 
fileout[1].open(gname);			// Also write data to a Timestamped file in "out" directory.
strftime (timetxt,80,"%m/%d/%Y %H:%M:%S",&timeinfo);
fileout[0].precision(6);
fileout[1].precision(6);
fileout[0].setf(ios::fixed,ios::floatfield); 
fileout[1].setf(ios::fixed,ios::floatfield); 
for (int j = 0; j<=1; j++) // for both output files
  {
	  fileout[j] << "#" << "LTAC1" << "\t" <<  timetxt << "\t"; //header
	for (int k = 0; k<nGates;k++)
	{fileout[j] <<  int(Gates[k].GetElow()/keV) << " to " << int(Gates[k].GetEhigh()/keV) << " keV\t";}
	fileout[j] << G4endl;

	fileout[j] << "#" << PN << "\t"<< NH << "\t"; 
	for (int k = 0; k<nGates;k++) {fileout[j] << Gates[k].GetGtotal() << "\t";}
	fileout[j] << G4endl;
   
	fileout[j] << "#E_(MeV)\tNLS\tY1\tY2\tY3" << G4endl;

    // print Energy, LS Integral, Y1, Y2, Y3
    for ( int i=(NChannels-1); i >= 0 ; i-- ) 
	{
	  fileout[j] << LSInteg->GetEbin(i)<< "\t" << LSInteg->GetFrequency(i)*1.0/(NH*1.0) << "\t"; 
	    for (int k = 0; k < nGates; k++ ) {fileout[j] << Gates[k].GetY(i) << "\t";}
	  fileout[j]<<G4endl;	
	}

    fileout[j].close();
  }
}




// Default Histogram Constructor
// After constructing, you will want to use BUILD to set up a histogram
//
Histogram::Histogram(){
	TotalCounts = 0;
	name = "temp";				// If name is still temp, then BUILD needs to be called.
	NChannels = 100;
	Ei = 0;
	Ef = 3000*keV;
	nHistograms++;
	dE = (Ef-Ei)/NChannels;		// will be recalculated by BUILD
	Frequency = new G4int[NChannels];
	Ebin = new G4double[NChannels];

	// Zero The Histogram, and Label Ebins starting with Ebin[0] = 0
	for ( int i=0; i < NChannels; i++ ) 
	{
		Frequency[i] = 0;		// zero histogram
		Ebin[i] = i * dE;		// set bin energies
	}
}


Histogram::~Histogram() {
	delete [] Ebin;			// be neat and delete.
	delete [] Frequency;	// be neat and delete.
}

void Histogram::Build(G4String HName, G4int NChan, G4double Eni, G4double Enf)
{
	name = HName;
	CountsinWin = 0;
	NChannels = NChan;
	Ei = Eni;
	Ef = Enf;
	dE = (Ef-Ei)/NChannels;				// bin width
	Frequency = new G4int[NChannels];	// number of counts in histogram channel
	Ebin = new G4double[NChannels];		// energy of lower bound of bin

	for ( int i=0; i < NChannels; i++ ) 
	{
		Frequency[i] = 0;		// zero histogram
		Ebin[i] = i * dE;		// set bin energies
	}
}

void Histogram::Clear()
{
	for ( int i=0; i < NChannels; i++ ) { Frequency[i] = 0; } // zero histogram
}

void Histogram::Fill(G4double eEvent)
{   
	for ( int i=(NChannels-1); i>=0; i--) 
	{
		if ( eEvent >=Ebin[i] ) { 
			Frequency[i]++;			// fill histogram. final channel is saturation peak.
			break;
		}
	}
}

void Histogram::FillWindows(G4double eEvent)
{   
	for ( int i=0; i<=2; i++) 
	{
		if ((eEvent >=Windows[i].Emin)&(eEvent < Windows[i].Emax)) 
		{
			Windows[i].Counts++;	// increment window counters.
		    Windows[0].Counts++;	// increment sum window
		}
	}
}



void Histogram::Print(G4String PN, G4int NH)
{
// Includes headder noting the particle name (eg. Co-60) and number of histories run

G4String sep1 = "\t";
G4String path = "out/";
G4String ext = ".csv";
G4String fname = name + ext;	// Name output file from Histogram name, with .csv extension
time_t rawtime;
struct tm  timeinfo;
time (&rawtime);
char buffer [80];
timeinfo = *localtime ( &rawtime) ;
strftime (buffer,80,"_%Y-%m-%d-%H_%M_%S",&timeinfo);

G4String gname = path + name + "_" + PN + buffer + ext;
ofstream fileout[2];
fileout[0].open(fname);			// Write data to a file in running directory. 
fileout[1].open(gname);			// Also write data to a Timestamped file in "out" directory.

for (int j = 0; j<=1; j++)
  {
    fileout[j] << "# Particle Name: " << PN << G4endl << "# NHistories: " << NH << G4endl; //headder
    fileout[j] << "#" <<G4endl << "# E(keV)" << sep1 << "N" << G4endl;
    // bottom of bin [/keV ] [TAB] frequency
    for ( int i=1; i < NChannels; i++ ) {fileout[j] << Ebin[i]/keV << sep1 << Frequency[i] << endl;}
    fileout[j].close();
  }
}

////==..==..==..==..==..==..==..==..==..==..==..==..==..==..==..==..==..==..//
//INTEGRAL is same as histogram, but integrated above threshold
//==..==..==..==..==..==..==..==..==..==..==..==..==..==..==..==..==..==..//


void Integral::Fill(G4double eEvent) // The only member that needs to be overloaded.
{
	for ( int i=(NChannels-1); i>=0; i--) 
	{
		if ( eEvent >=Ebin[i] ) { 
			Frequency[i]++;						// fill Integral. final channel is saturation peak.
		}										// no "break", fill all channels below eEvent (integrate).
	}
}


Window::Window()
{Counts=0;}
	Window::Window(G4String WinName, G4double WinEmin, G4double WinEmax, G4int WinCounts)
	{
		Name = WinName;
		Emin = WinEmin;
		Emax = WinEmax;
		Counts = WinCounts;
	}

	Window::~Window()
	{Counts = 0;}

	void Window::Build(G4String WinName, G4double WinEmin, G4double WinEmax, G4int WinCounts)
	{
	Name = WinName;
	Emin = WinEmin;
	Emax = WinEmax;
	Counts = WinCounts;
	}

////==..==..==..==..==..==..==..==..==..==..==..==..==..==..==..==..==..==..//
//                Gate has gates and Y = ACGamma/Gamma
//==..==..==..==..==..==..==..==..==..==..==..==..==..==..==..==..==..==..//
Gate::Gate()	// Boring constructor. User needs to BUILD gate after constructing.
{;}
Gate::~Gate()	// Deconstructor
{
	delete [] Frequency;
	delete [] Y;
}

bool Gate::InGate(G4double ENaI) // Is the energy within the gate?
{
	if ( (Elow < ENaI) && (ENaI < Ehigh) )
		{return true;}			// Yes
	else 
		{return false;}			// No
}

void Gate::Build(G4String gname, G4double El, G4double Eh, Integral* LSInt)
{
	name = gname;
	LSInteg = LSInt;					// Name of Integral object to which gate will apply
	NChannels = LSInt->GetNChannels();	// Number of channels in the associated Integral
	Elow = El;							// Lower level of Gate
	Ehigh = Eh;							// Upper level of Gate
	Frequency = new G4int[NChannels];	// Counts in gated spectrum at given threshold
	Y = new G4double[NChannels];		// Inefficiency (anti-coinc / total g) at given threshold
	Gtotal = 0;							// Total counts

	for ( int i=(NChannels-1); i>=0; i--) {Frequency[i]=0; Y[i]=0;} // Zero out arrays
}

void Gate::Fill(G4double ELS, G4double ENaI)
{
	if ( InGate(ENaI)==true )					// If count is in gate
	{
		Gtotal++;								// Increment total counts
		for ( int i=(NChannels-1); i>=0; i--) 
		{
			// fill histogram
			if ( ELS >=LSInteg->Ebin[i] )		
			{ 
				Frequency[i]++;					// Fill gated Integral spectrum
			}
			Y[i] = 1.0 - Frequency[i]*1.0 / Gtotal;   // Calculate Y as anti-coinc counts / all counts
		}
	}
}


void Gate::Print(G4String PN, G4int NH)	// print extrapolation plot with headder
{
G4String sep1 = "\t";
G4String path = "out/";					// Save output file in directory "out"
G4String ext = ".csv";
G4String fname = name + ext;			// Name output file from Histogram name, with .csv extension
time_t rawtime;
struct tm  timeinfo;					// Get Timestamp
time (&rawtime);
char buffer [80];
timeinfo = *localtime ( &rawtime) ;
strftime (buffer,80,"_%Y-%m-%d-%H_%M_%S",&timeinfo);

G4String gname = path + name + "_" + PN + buffer + ext;
ofstream fileout[2];
fileout[0].open(fname);			// Write data to a file in running directory. 
fileout[1].open(gname);			// Also write data to a Timestamped file in "out" directory.

for (int j = 0; j<=1; j++)
  {
    fileout[j] << "# Particle Name: " << PN << G4endl << "# NHistories: " << NH << G4endl;	// headder
    fileout[j] << "# NaI gate: " << G4BestUnit(Elow,"Energy") << " to " << G4BestUnit(Ehigh, "Energy") << G4endl;
	fileout[j] << "# Total g-ray counts in gate: " << Gtotal << G4endl;
    fileout[j] << "#" <<G4endl << "# Y" << sep1 << "NLS" << G4endl;
    // print Y [TAB] LS Integral counts
    for ( int i=0; i < NChannels; i++ ) {fileout[j] << Y[i] << sep1 << LSInteg->Frequency[i] << endl;}
    fileout[j].close();
  }
}

//=============================================================
// ==================== BEGIN HISTOGRAM2D =====================
//=============================================================

int Histogram2D::nHistograms2D = 0;
Histogram2D::Histogram2D(){
	TotalCounts = 0;
	name = "temp";				// If name is still temp, then BUILD needs to be called.

	for (int k = 0; k <= 1; k++)
	{
		NChannels[k] = 100;	
		Ei[k] = 0;	
		Ef[k] = 3000*keV;	
		nHistograms2D++;
		dE[k] = (Ef[k]-Ei[k])/NChannels[k];			// will be recalculated by BUILD
	}
	// Zero The Histogram, and Label Ebins starting with Ebin[0] = 0
	for ( int i=0; i < NChannels[0]; i++ ) 
	{
		Ebin[0][i] = i * dE[0];						// set bin energies for channel 0
		for (int j = 0; j < NChannels[1]; j++)
		{
			Frequency[i][j] = 0;					// zero histogram
			if (i == 0) Ebin[1][j] = j * dE[1];		// set bin energies for channel 1 (once)
		}
	}
}

void Histogram2D::FillWindows(G4double E1, G4double E2)
{  
       for ( int i=1; i<=2; i++)
       {
              if (((E1 >=Windows[i].Emin)&(E1 < Windows[i].Emax)) &((E2 >=Windows[i].Emin)&(E2 < Windows[i].Emax)))
              {
                     Windows[i].Counts++; // increment window counters.
              }
 
       }
              if ((((E1 >=Windows[1].Emin)&(E1 < Windows[1].Emax))&((E2 >=Windows[2].Emin)&(E2 < Windows[2].Emax)))||
                     (((E1 >=Windows[2].Emin)&(E1 < Windows[2].Emax))&((E2 >=Windows[1].Emin)&(E2 < Windows[1].Emax)))||
                     (((E1 >=Windows[1].Emin)&(E1 < Windows[1].Emax))&((E2 >=Windows[1].Emin)&(E2 < Windows[1].Emax)))||
                     (((E1 >=Windows[2].Emin)&(E1 < Windows[2].Emax))&((E2 >=Windows[2].Emin)&(E2 < Windows[2].Emax))))
              {
                  Windows[0].Counts++;   // increment sum window crossing win1 and win2
              }
              if ((((E1 >=Windows[1].Emin)&(E1 < Windows[1].Emax))&((E2 >=Windows[2].Emin)&(E2 < Windows[2].Emax)))||
                     (((E1 >=Windows[2].Emin)&(E1 < Windows[2].Emax))&((E2 >=Windows[1].Emin)&(E2 < Windows[1].Emax))))
              {
                  Windows[3].Counts++;   // increment sum window Win1 vs.Win2
              }
}
Histogram2D::~Histogram2D() 
{}


void Histogram2D::Build(G4String HName, G4int NChan0, G4double Eni0, G4double Enf0, G4int NChan1, G4double Eni1, G4double Enf1) // build a histogram2D to spec
{
	name = HName;
	if (NChan0 > NBins2D) 
	{NChan0 = NBins2D; G4cout << "Warning: more than NBins2D max of " << NBins2D << " channels requested in 2D spectrum, ch0. Truncated to max." << G4endl;}
	if (NChan1 > NBins2D) 
	{NChan1 = NBins2D; G4cout << "Warning: more than NBins2D max of " << NBins2D << " channels requested in 2D spectrum, ch1. Truncated to max." << G4endl;}

	NChannels[0] = NChan0;					NChannels[1] = NChan1;
	Ei[0] = Eni0;							Ei[1] = Eni1;
	Ef[0] = Enf0;							Ef[1] = Enf1;
	dE[0] = (Ef[0]-Ei[0])/NChannels[0];		dE[1] = (Ef[1]-Ei[1])/NChannels[1];				

	for ( int i = 0; i < NChannels[0]; i++ ) 
	{
		Ebin[0][i] = i * dE[0];						// set bin energies for channel 0
		for (int j = 0; j < NChannels[1]; j++)
		{
			Frequency[i][j] = 0;					// zero histogram
			if (i == 0) Ebin[1][j] = j * dE[1];		// set bin energies for channel 1 (once)
		}
	}
}


void Histogram2D::Clear()
{
	for ( int i = 0; i < NChannels[0]; i++ ) 
	{
		for (int j = 0; j < NChannels[1]; j++) Frequency[i][j] = 0;	
	}
}

void Histogram2D::Fill(G4double eEventCh0, G4double eEventCh1)
{
	for ( int i = (NChannels[0]-1); i >= 0; i--)		// step down through channel 0
	{
	  if (eEventCh0 >= Ebin[0][i])						// If found bin for channel 0
	  {					
	    for ( int j = (NChannels[1]-1); j >= 0; j--)	// step down through channel 1
		{
		    if (eEventCh1 >= Ebin[1][j])				// If found bin for channel 1
			{
				Frequency[i][j]++;						// increment histogram
				break;									// break out of channel 1 loop
			}
		}
	    break;				// break out of channel 0 loop. Event has been histogrammed.
	  }
	}

}


void Histogram2D::Print(G4String PN, G4int NH)
{
// Includes headder noting the particle name (eg. Co-60) and number of histories run
G4String sep1 = "\t";
G4String path = "out/";
G4String ext = ".csv";
G4String fname = name + ext;	// Name output file from Histogram name, with .csv extension
time_t rawtime;
struct tm  timeinfo;
time (&rawtime);
char buffer [80];
timeinfo = *localtime ( &rawtime) ;
strftime (buffer,80,"_%Y-%m-%d-%H_%M_%S",&timeinfo);

G4String gname = path + name + "_" + PN + buffer + ext;
ofstream fileout;
fileout.open(fname);			// Write data to a file in running directory. 

fileout << "# Particle Name: " << PN << G4endl << "# NHistories: " << NH << G4endl; //headder


fileout << "LS\\NaI" << sep1;														// Corner headding
for ( int i = 0; i < NChannels[1]; i++ ) {fileout << Ebin[1][i]/keV << sep1 ;} 	// write energy bins for channel 1 in a row at top of file
fileout << G4endl;																// end of line character

	
for (int i = 0; i < NChannels[0]; i++)												// now write frequency array
{	fileout << Ebin[0][i]/keV << sep1 ;											// write channel 0 energy bin followed by frequency data
	for (int j = 0; j < NChannels[1]; j++)	{ fileout << Frequency[i][j] << sep1; }
	fileout << G4endl;															// end of line character
}

fileout.close();
  
ifstream source; // copy files to out folder
ofstream dest; 
dest.open(gname);

source.open(fname);
dest << source.rdbuf();
dest.close();
source.close();
}