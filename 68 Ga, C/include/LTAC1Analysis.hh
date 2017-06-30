
//
#include "globals.hh"
#ifndef LTAC1Analysis_h
#define LTAC1Analysis_h 1
//
#define NBins2D 1000 // 1000 will give 1M array for 2D spectrum, at 4 B per, that's 4 MB OK
//
//
class Window
{
public:
	Window();
	~Window();
	Window(G4String WinName, G4double WinEmin, G4double WinEmax, G4int WinCounts);
	void Build(G4String WinName, G4double WinEmin, G4double WinEmax, G4int WinCounts);
	G4double Emin;
	G4double Emax;
	G4int Counts;
	G4String Name;
	void Print();
};
//
//
class Histogram		// define histogram class
{
public:
	G4String name;		// name of histogram
	G4int NChannels;	// number of channels in histogram
	G4double Ei;		// energy of initial bin
	G4double Ef;		// energy of final bin
	G4int * Frequency;	// array of number of counts in each bin
	G4double * Ebin;	// array of energies for each bin
	G4double dE;		// energy step between bins
	G4int TotalCounts;	// sum of counts in all bins
	G4int CountsinWin;	
	Window Windows[4];	// Array of 4 Windows: Window 0 is sum of counts
	Window myWindow;
public:
	static int nHistograms;	// number of histograms made
	Histogram(); // default histogram constructor
	void Build(G4String, G4int, G4double, G4double); // build a histogram to spec
	~Histogram(); // default histogram deconstructor

	G4String GetHistName()	{ return name; };
	G4int	 GetNChannels()	{ return NChannels; };
	G4double GetEi()		{ return Ei; };
	G4double GetEf()		{ return Ef; };
	G4int GetFrequency(int i){return Frequency[i];};
	G4double GetEbin(int i){return Ebin[i];};

	void Fill(G4double);				// increment histogram with a new count
	void FillWindows(G4double);			// increment windows
	void Clear();						// zero all histogram channels
	void Print();						// print histogram
	void Print(G4String PN, G4int NH);	// print histogram including headder with 
										// particle name and number of histories
};

class Integral : public Histogram		// Integral from infinity to threshold
{
public:
	void Fill(G4double);				// Fills all bins with E <= Threshold

};



class Gate	// Gates and corresponding anticoincidence extrpolation (LS rate vs. Y)
{			// Requires the pointer to an LS Integral spectrum to create gated spectrum
public:
	Gate();
	~Gate();
private:
	G4String name;			// Gate name.
	G4int NChannels;		// number of channels in gated spectrum = NChannels from LS Integral
	G4int Gtotal;			// Total number of g-rays. Used for Y = AC / G = 1 - Frequency/Gtotal
	G4double * Y;			// Anticoincident-gamma to total gamma ratio, an estimate of LS inefficiency
	G4int * Frequency;		// Gated LS Integral spectrum counts
	G4double Elow;			// Lower limit of gate
	G4double Ehigh;			// Upper limit of gate
	Integral * LSInteg;		// pointer to LS integral to use for coincidences
	bool InGate(G4double);	// Is NaI energy in gate? True or False.
public:
	G4int GetGtotal(){return Gtotal;}
	Integral * GetNaI1Integral(){return LSInteg;}
	G4double  GetY(int i){return Y[i];}
	G4int  GetFrequency(int i){return Frequency[i];}

	G4double GetElow(){return Elow;}
	G4double GetEhigh(){return Ehigh;}
	G4int GetNChannels(){return NChannels;}
	void Fill(G4double ELS, G4double ENaI);
	void Build(G4String, G4double, G4double, Integral*);
	void Print();							// basic printing
	void Print(G4String PN, G4int NH);		// printing with headder info and timestamp.
};

class Histogram2D		// define histogram class
{
public:
	G4String name;		// name of histogram
	G4int NChannels[2];	// number of channels in histogram
	G4double Ei[2];		// energy of initial bin
	G4double Ef[2];		// energy of final bin

	G4int Frequency[NBins2D][NBins2D];	// 2D array of number of counts in each bin. Limit to a reasonable size (4 MB).
	G4double  Ebin[NBins2D][NBins2D];	// 2D array of energies for each bin
	G4double dE[2];		// energy step between bins
	G4int TotalCounts;	// sum of counts in all bins
	Window Windows[4];	// Array of 4 Windows: Window 0 is sum of counts
public:
	static int nHistograms2D;	// number of 1D histograms made
	Histogram2D();				// default histogram constructor

    void Build(G4String HName, G4int NChan0, G4double Eni0, G4double Enf0, G4int NChan1, G4double Eni1, G4double Enf1); // build a histogram2D to spec
	void FillWindows(G4double, G4double);			// increment windows
	~Histogram2D();				// default histogram2D deconstructor

	G4String GetHistName()		{ return name; };
	G4int GetNChannels(int i)	{ return NChannels[i]; };
	G4double GetEi(int i)		{ return Ei[i]; };
	G4double GetEf(int i)		{ return Ef[i]; };
	G4int GetFrequency(int i, int j)  {return Frequency[i][j];};
	G4double GetEbin(int i, int j)	  {return Ebin[i][j];};

	void Fill(G4double ECh0, G4double ECh1);	// increment histogram with a new count
	void Clear();								// zero all histogram channels
	void Print();								// print histogram
	void Print(G4String PN, G4int NH);			// print histogram including headder with 
	   											// particle name and number of histories
};
void PrintExtrapolation(Gate GatesArray[],int NumGates,G4String PN, G4int NH);

// LEAVE THIS AT THE END !!!!!!/
#endif // Definition