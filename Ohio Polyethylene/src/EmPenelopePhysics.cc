//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// $Id: G4EmPenelopePhysics.cc,v 1.12 2010-10-10 15:18:34 vnivanch Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// RPF Modified G4EmPenelopePhysics to include Radioactive Decay - Aug 2012

#include "EmPenelopePhysics.hh"

#include "G4ParticleDefinition.hh"

// *** Processes and models

// gamma

#include "G4PhotoElectricEffect.hh"
#include "G4PenelopePhotoElectricModel.hh"

#include "G4ComptonScattering.hh"
#include "G4PenelopeComptonModel.hh"

#include "G4GammaConversion.hh"
#include "G4PenelopeGammaConversionModel.hh"

#include "G4RayleighScattering.hh" 
#include "G4PenelopeRayleighModel.hh"

// e- and e+

#include "G4eMultipleScattering.hh"
#include "G4UniversalFluctuation.hh"

#include "G4eIonisation.hh"
#include "G4PenelopeIonisationModel.hh"

#include "G4eBremsstrahlung.hh"
#include "G4PenelopeBremsstrahlungModel.hh"

// e+ only

#include "G4eplusAnnihilation.hh"
#include "G4PenelopeAnnihilationModel.hh"

// mu

#include "G4MuMultipleScattering.hh"
#include "G4MuIonisation.hh"
#include "G4MuBremsstrahlung.hh"
#include "G4MuPairProduction.hh"

#include "G4MuBremsstrahlungModel.hh"
#include "G4MuPairProductionModel.hh"
#include "G4hBremsstrahlungModel.hh"
#include "G4hPairProductionModel.hh"

// hadrons

#include "G4hMultipleScattering.hh"
#include "G4MscStepLimitType.hh"

#include "G4hBremsstrahlung.hh"
#include "G4hPairProduction.hh"

#include "G4hIonisation.hh"
#include "G4ionIonisation.hh"
#include "G4alphaIonisation.hh"
#include "G4IonParametrisedLossModel.hh"
#include "G4NuclearStopping.hh"

// msc models
#include "G4UrbanMscModel93.hh"
#include "G4UrbanMscModel95.hh"
#include "G4GoudsmitSaundersonMscModel.hh"
#include "G4WentzelVIModel.hh"
#include "G4CoulombScattering.hh"

//

#include "G4LossTableManager.hh"
#include "G4UAtomicDeexcitation.hh"
#include "G4EmProcessOptions.hh"

// particles

#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4MuonPlus.hh"
#include "G4MuonMinus.hh"
#include "G4PionPlus.hh"
#include "G4PionMinus.hh"
#include "G4KaonPlus.hh"
#include "G4KaonMinus.hh"
#include "G4Proton.hh"
#include "G4AntiProton.hh"
#include "G4Deuteron.hh"
#include "G4Triton.hh"
#include "G4He3.hh"
#include "G4Alpha.hh"
#include "G4GenericIon.hh"

//
#include "G4PhysicsListHelper.hh"
#include "G4BuilderType.hh"
//
#include "G4RadioactiveDecay.hh"
#include "G4RadioactiveDecayPhysics.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EmPenelopePhysics::EmPenelopePhysics(G4int ver)
  : G4VPhysicsConstructor("EmPenelopePhysics"), verbose(ver)
{
  G4LossTableManager::Instance();
  SetPhysicsType(bElectromagnetic);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EmPenelopePhysics::EmPenelopePhysics(G4int ver, const G4String&)
  : G4VPhysicsConstructor("EmPenelopePhysics"), verbose(ver)
{
  G4LossTableManager::Instance();
  SetPhysicsType(bElectromagnetic);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EmPenelopePhysics::~EmPenelopePhysics()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EmPenelopePhysics::ConstructParticle()
{
// gamma
  G4Gamma::Gamma();

// leptons
  G4Electron::Electron();
  G4Positron::Positron();
  G4MuonPlus::MuonPlus();
  G4MuonMinus::MuonMinus();

// mesons
  G4PionPlus::PionPlusDefinition();
  G4PionMinus::PionMinusDefinition();
  G4KaonPlus::KaonPlusDefinition();
  G4KaonMinus::KaonMinusDefinition();

// baryons
  G4Proton::Proton();
  G4AntiProton::AntiProton();

// ions
  G4Deuteron::Deuteron();
  G4Triton::Triton();
  G4He3::He3();
  G4Alpha::Alpha();
  G4GenericIon::GenericIonDefinition();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EmPenelopePhysics::ConstructProcess()
{
  G4PhysicsListHelper* ph = G4PhysicsListHelper::GetPhysicsListHelper();

  // Add Penelope EM Processes

  theParticleIterator->reset();

  while( (*theParticleIterator)() ){
  
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4String particleName = particle->GetParticleName();
    
    if(verbose > 1)
      G4cout << "### " << GetPhysicsName() << " instantiates for " 
	     << particleName << G4endl;

    //Applicability range for Penelope models
    //for higher energies, the Standard models are used   
    G4double PenelopeHighEnergyLimit = 1.0*GeV;

    if (particleName == "gamma") {

      //Photo-electric effect
      G4PhotoElectricEffect* thePhotoElectricEffect = new G4PhotoElectricEffect();
      G4PenelopePhotoElectricModel* thePEPenelopeModel = new 
	G4PenelopePhotoElectricModel();   
      thePEPenelopeModel->SetHighEnergyLimit(PenelopeHighEnergyLimit);
      thePhotoElectricEffect->AddEmModel(0,thePEPenelopeModel);
      ph->RegisterProcess(thePhotoElectricEffect, particle);

      //Compton scattering
      G4ComptonScattering* theComptonScattering = new G4ComptonScattering();
      G4PenelopeComptonModel* theComptonPenelopeModel = 
	new G4PenelopeComptonModel();
      theComptonPenelopeModel->SetHighEnergyLimit(PenelopeHighEnergyLimit);
      theComptonScattering->AddEmModel(0,theComptonPenelopeModel);
      ph->RegisterProcess(theComptonScattering, particle);

      //Gamma conversion
      G4GammaConversion* theGammaConversion = new G4GammaConversion();
      G4PenelopeGammaConversionModel* theGCPenelopeModel = 
	new G4PenelopeGammaConversionModel();
      theGammaConversion->AddEmModel(0,theGCPenelopeModel);
      ph->RegisterProcess(theGammaConversion, particle);

      //Rayleigh scattering
      G4RayleighScattering* theRayleigh = new G4RayleighScattering();
      G4PenelopeRayleighModel* theRayleighPenelopeModel = 
	new G4PenelopeRayleighModel();
      theRayleighPenelopeModel->SetHighEnergyLimit(PenelopeHighEnergyLimit);
      theRayleigh->AddEmModel(0,theRayleighPenelopeModel);
      ph->RegisterProcess(theRayleigh, particle);

    } else if (particleName == "e-") {

      G4eMultipleScattering* msc = new G4eMultipleScattering();
      //msc->AddEmModel(0, new G4GoudsmitSaundersonMscModel());
      msc->SetStepLimitType(fUseDistanceToBoundary);
	  msc->SetLateralDisplasmentFlag(false);
	  msc->SetRangeFactor(0.02);
	  msc->SetGeomFactor(2.5);
	  msc->SetSkin(2.0);
      ph->RegisterProcess(msc, particle);
      
      //Ionisation
      G4eIonisation* eIoni = new G4eIonisation();
      G4PenelopeIonisationModel* theIoniPenelope = 
	new G4PenelopeIonisationModel();
      theIoniPenelope->SetHighEnergyLimit(PenelopeHighEnergyLimit);     
      eIoni->AddEmModel(0,theIoniPenelope,new G4UniversalFluctuation());
      eIoni->SetStepFunction(0.2, 100*um); //     
      ph->RegisterProcess(eIoni, particle);
      
      //Bremsstrahlung
      G4eBremsstrahlung* eBrem = new G4eBremsstrahlung();
      G4PenelopeBremsstrahlungModel* theBremPenelope = new 
	G4PenelopeBremsstrahlungModel();
      theBremPenelope->SetHighEnergyLimit(PenelopeHighEnergyLimit);
      eBrem->AddEmModel(0,theBremPenelope);
      ph->RegisterProcess(eBrem, particle);

    } else if (particleName == "e+") {
    
      G4eMultipleScattering* msc = new G4eMultipleScattering();
      //msc->AddEmModel(0, new G4GoudsmitSaundersonMscModel());
      msc->SetStepLimitType(fUseDistanceToBoundary);
      ph->RegisterProcess(msc, particle);

      //Ionisation
      G4eIonisation* eIoni = new G4eIonisation();
      G4PenelopeIonisationModel* theIoniPenelope = 
	new G4PenelopeIonisationModel();
      theIoniPenelope->SetHighEnergyLimit(PenelopeHighEnergyLimit);
      eIoni->AddEmModel(0,theIoniPenelope,new G4UniversalFluctuation());
      eIoni->SetStepFunction(0.2, 100*um); //     
      ph->RegisterProcess(eIoni, particle);

       //Bremsstrahlung
      G4eBremsstrahlung* eBrem = new G4eBremsstrahlung();
      G4PenelopeBremsstrahlungModel* theBremPenelope = new 
	G4PenelopeBremsstrahlungModel();
      theBremPenelope->SetHighEnergyLimit(PenelopeHighEnergyLimit);
      eBrem->AddEmModel(0,theBremPenelope);
      ph->RegisterProcess(eBrem, particle);
      
      //Annihilation
      G4eplusAnnihilation* eAnni = new G4eplusAnnihilation();
      G4PenelopeAnnihilationModel* theAnnPenelope = new 
	G4PenelopeAnnihilationModel();
      theAnnPenelope->SetHighEnergyLimit(PenelopeHighEnergyLimit);
      eAnni->AddEmModel(0,theAnnPenelope);
      ph->RegisterProcess(eAnni, particle);

    } else if (particleName == "mu+" ||
               particleName == "mu-"    ) {

      // Identical to G4EmStandardPhysics_option3

      G4MuMultipleScattering* msc = new G4MuMultipleScattering();
      msc->AddEmModel(0, new G4WentzelVIModel());

      G4MuIonisation* muIoni = new G4MuIonisation();
      muIoni->SetStepFunction(0.2, 50*um);          

      G4MuBremsstrahlung* mub = new G4MuBremsstrahlung();
      G4MuPairProduction* mup = new G4MuPairProduction();

      ph->RegisterProcess(msc, particle);
      ph->RegisterProcess(muIoni, particle);
      ph->RegisterProcess(mub, particle);
      ph->RegisterProcess(mup, particle);
      ph->RegisterProcess(new G4CoulombScattering(), particle);

    } else if (particleName == "alpha" ||
               particleName == "He3" ) {

      // Identical to G4EmStandardPhysics_option3
      
      G4ionIonisation* ionIoni = new G4ionIonisation();
      ionIoni->SetStepFunction(0.1, 10*um);

      ph->RegisterProcess(new G4hMultipleScattering(), particle);
      ph->RegisterProcess(ionIoni, particle);
      ph->RegisterProcess(new G4NuclearStopping(), particle);

    } else if (particleName == "GenericIon") {

      // Identical to G4EmStandardPhysics_option3
      
      G4ionIonisation* ionIoni = new G4ionIonisation();
      ionIoni->SetEmModel(new G4IonParametrisedLossModel());
      ionIoni->SetStepFunction(0.1, 1*um);

      ph->RegisterProcess(new G4hMultipleScattering(), particle);
      ph->RegisterProcess(ionIoni, particle);
      ph->RegisterProcess(new G4NuclearStopping(), particle);
      ph->RegisterProcess(new G4RadioactiveDecay(),particle);

    } else if (particleName == "pi+" ||
               particleName == "pi-" ) {

      G4hIonisation* hIoni = new G4hIonisation();
      hIoni->SetStepFunction(0.2, 50*um);

      G4hBremsstrahlung* pib = new G4hBremsstrahlung();
      G4hPairProduction* pip = new G4hPairProduction();

      ph->RegisterProcess(new G4hMultipleScattering(), particle);
      ph->RegisterProcess(hIoni, particle);
      ph->RegisterProcess(pib, particle);
      ph->RegisterProcess(pip, particle);

    } else if (particleName == "kaon+" ||
               particleName == "kaon-" ) {

      G4hIonisation* hIoni = new G4hIonisation();
      hIoni->SetStepFunction(0.2, 50*um);

      G4hBremsstrahlung* kb = new G4hBremsstrahlung();
      G4hPairProduction* kp = new G4hPairProduction();

      ph->RegisterProcess(new G4hMultipleScattering(), particle);
      ph->RegisterProcess(hIoni, particle);
      ph->RegisterProcess(kb, particle);
      ph->RegisterProcess(kp, particle);

    } else if (particleName == "proton" ) {

      G4hIonisation* hIoni = new G4hIonisation();
      hIoni->SetStepFunction(0.2, 50*um);

      ph->RegisterProcess(new G4hMultipleScattering(), particle);
      ph->RegisterProcess(hIoni, particle);
      ph->RegisterProcess(new G4hBremsstrahlung(), particle);
      ph->RegisterProcess(new G4hPairProduction(), particle);

    } else if (particleName == "B+" ||
	       particleName == "B-" ||
	       particleName == "D+" ||
	       particleName == "D-" ||
	       particleName == "Ds+" ||
	       particleName == "Ds-" ||
               particleName == "anti_He3" ||
               particleName == "anti_alpha" ||
               particleName == "anti_deuteron" ||
               particleName == "anti_lambda_c+" ||
               particleName == "anti_omega-" ||
               particleName == "anti_proton" ||
               particleName == "anti_sigma_c+" ||
               particleName == "anti_sigma_c++" ||
               particleName == "anti_sigma+" ||
               particleName == "anti_sigma-" ||
               particleName == "anti_triton" ||
               particleName == "anti_xi_c+" ||
               particleName == "anti_xi-" ||
               particleName == "deuteron" ||
	       particleName == "lambda_c+" ||
               particleName == "omega-" ||
               particleName == "sigma_c+" ||
               particleName == "sigma_c++" ||
               particleName == "sigma+" ||
               particleName == "sigma-" ||
               particleName == "tau+" ||
               particleName == "tau-" ||
               particleName == "triton" ||
               particleName == "xi_c+" ||
               particleName == "xi-" ) {

      // Identical to G4EmStandardPhysics_option3
      
      ph->RegisterProcess(new G4hMultipleScattering(), particle);
      ph->RegisterProcess(new G4hIonisation(), particle);

    }
  }
    
  // Em options
  //      
  G4EmProcessOptions opt;
  opt.SetVerbose(verbose);
  
  // Multiple Coulomb scattering
  //
  //opt.SetMscStepLimitation(fUseDistanceToBoundary);
  //opt.SetMscRangeFactor(0.02);
    
  // Physics tables
  //

  opt.SetMinEnergy(100*eV);
  opt.SetMaxEnergy(10*TeV);
  opt.SetDEDXBinning(220);
  opt.SetLambdaBinning(220);

  //opt.SetSplineFlag(true);
  opt.SetPolarAngleLimit(CLHEP::pi);
    
  // Ionization
  //
  //opt.SetSubCutoff(true);    

  
  // Deexcitation
  //

  G4VAtomDeexcitation* deexcitation = new G4UAtomicDeexcitation();
  G4LossTableManager::Instance()->SetAtomDeexcitation(deexcitation);
  deexcitation->SetFluo(true);
  deexcitation->SetAuger(true);
  deexcitation->SetPIXE(true);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
