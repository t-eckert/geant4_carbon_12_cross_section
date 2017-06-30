
#ifndef LTAC1Hit_h
#define LTAC1Hit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4ParticleDefinition.hh"

class LTAC1Hit : public G4VHit
{
  public:

      LTAC1Hit();
     ~LTAC1Hit();
  //    LTAC1Hit(const LTAC1Hit&);
 
      inline void* operator new(size_t);
      inline void  operator delete(void*);


  public:
      void SetEdep(G4double de) { edep = de; };			// Energy Set in LTAC1SD.cc
      G4double GetEdep() { return edep; };				// Energy Retrieved in LTAC1EventAction.cc
	  void SetChunkNumber(G4int cn) { ChunkNumber = cn; };			// Energy Set in LTAC1SD.cc
      G4int GetChunkNumber() { return ChunkNumber; };			// Energy Retrieved in LTAC1EventAction.cc
      void SetParentID(G4int ID) { parentID = ID; };	// Energy Set in LTAC1SD.cc
      G4double GetParentID() { return parentID; };		// Energy Retrieved in LTAC1EventAction.cc
	  void SetTrackID(G4int ID) { trackID = ID; };		//  Set Track ID, and MaxTrackID, if max for this event.
      G4double GetTrackID() { return trackID; };		// Energy Retrieved in LTAC1EventAction.cc
      void SetTrackDef(G4ParticleDefinition * DEF) 
	  { trackDef = DEF; trackName=DEF->GetParticleName();};					// Set particle name and definition
      G4ParticleDefinition * GetTrackDef() { return trackDef; };			// Energy Retrieved in LTAC1EventAction.cc
     
      G4String GetTrackName() { return trackDef->GetParticleName(); };			// Get Particle name


  private:
      G4double edep;	// Energy deposited by track during hit.
	  G4int parentID;
	  G4int trackID;
	  G4ParticleDefinition * trackDef;
	  G4String trackName;
	  G4int ChunkNumber;


};

// I have no idea. See Geant4 manual. //

typedef G4THitsCollection<LTAC1Hit> LTAC1HitsCollection;

extern G4Allocator<LTAC1Hit> LTAC1HitAllocator;


inline void* LTAC1Hit::operator new(size_t)
{
  void *aHit;
  aHit = (void *) LTAC1HitAllocator.MallocSingle();
  return aHit;
}


inline void LTAC1Hit::operator delete(void *aHit)
{
  LTAC1HitAllocator.FreeSingle((LTAC1Hit*) aHit);
}

// Leave at the end
#endif //
