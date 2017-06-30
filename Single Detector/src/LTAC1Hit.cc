//
//
#include "LTAC1Hit.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

G4Allocator<LTAC1Hit> LTAC1HitAllocator; // I don't pretend to know much about this


LTAC1Hit::LTAC1Hit()
{}

LTAC1Hit::~LTAC1Hit() {}

//LTAC1Hit::LTAC1Hit(const LTAC1Hit& right)
//  : G4VHit()
//{
//  edep      = right.edep;			 // Energy deposited is set in LTAC1SD.cc
//  									 // Could also define TrackID, TrackSD, TrackPos etc.
//}