// This code is used for studying particle interactions with matter.
// One should be able to switch to different materials, like H2O, Fe, Au, Pb, 
// etc., and launch different particles at different energies.

//   Date created:  10/15/2010, hexc and my students
//   Updated: removed pINT physics list, use the default
//
//   March 17, 2017:  Modified the code to use Qt for event/detector display
//
//   April 12, 2019: (X. He, Hemendra, Ernesto)
//          Changed physics process list from FTFP_BERT to QGSP_BERT_HP in order to better tracking neutron in this simulation
//      
//   Sep 4, 2020: Hexc
//          Updated the code for running multithreaded mode
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif

#include "G4UImanager.hh"
#include "Randomize.hh"

#include "pINTDetectorConstruction.hh"
#include "QGSP_BERT_HP.hh"
#include "pINTPrimaryGeneratorAction.hh"
#include "pINTRunAction.hh"
#include "pINTEventAction.hh"
#include "pINTSteppingAction.hh"
#include "pINTSteppingVerbose.hh"
#include "pINTActionInitialization.hh"

#include "pINTHistoManager.hh"

#include "pINTOutput.hh"

#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

/*
//#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
//#endif

//#ifdef G4UI_USE
#include "G4UIExecutive.hh"
//#endif
*/


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc, char** argv)
{
  // Detect interactive mode (if no arguments) and define UI session
  //
  G4UIExecutive* ui = 0;
  if ( argc == 1 ) {
    ui = new G4UIExecutive(argc, argv);
  }

    // Choose the Random engine
  //
  CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);
  
  // Create the Output file object (a singleton object)
  //  pINTOutput*  myOut = pINTOutput::instance();

  //  myOut->Fopen("pINT_Output.txt");

  // User Verbose output class
  //
  G4VSteppingVerbose::SetInstance(new pINTSteppingVerbose);
     
  // Construct the default run manager
  //
#ifdef G4MULTITHREADED
  G4MTRunManager* runManager = new G4MTRunManager;
  runManager->SetNumberOfThreads(2);
#else
  G4RunManager* runManager = new G4RunManager;
#endif

  // Detector construction
  // 
  pINTDetectorConstruction* detector = new pINTDetectorConstruction;
  runManager->SetUserInitialization(detector);
  
  //
  //  G4VModularPhysicsList* physicsList = new FTFP_BERT;   // commented out on 4/12/2019
  G4VModularPhysicsList* physicsList = new QGSP_BERT_HP;    // added on 4/12/2019
  runManager->SetUserInitialization(physicsList);

  // Create ourput ntuple
  pINTHistoManager* histo = new pINTHistoManager();


  //
  //  pINTRunAction* run_action = new pINTRunAction(histo);  
  //  runManager->SetUserAction(run_action);
  //
  //  pINTEventAction* event_action = new pINTEventAction(run_action);
  //  runManager->SetUserAction(event_action);
  //
  // G4UserSteppingAction* stepping_action =
  //   new pINTSteppingAction(detector, event_action, histo);
  //  runManager->SetUserAction(stepping_action);

  // Set mandatory initialization classes
  //

  // Set user action initialization
  //
  runManager->SetUserInitialization(new pINTActionInitialization(detector, histo));

  G4cout << "Got here!" << G4endl;
  
  // Initialize visualization
  //
  G4VisManager* visManager = new G4VisExecutive;
  // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
  // G4VisManager* visManager = new G4VisExecutive("Quiet");
  visManager->Initialize();

  // Get the pointer to the User Interface manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  // Activate score ntuple writer
  // The Root output type (Root) is selected in B3Analysis.hh.
  // The verbose level can be also set via UI commands
  // /score/ntuple/writerVerbose level
  //G4TScoreNtupleWriter<G4AnalysisManager> scoreNtupleWriter;
  //scoreNtupleWriter.SetVerboseLevel(1);

  // Process macro or start UI session
  //
  if ( ! ui ) {
    // batch mode
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UImanager->ApplyCommand(command+fileName);
  }
  else {
    // interactive mode
    UImanager->ApplyCommand("/control/execute init_vis.mac");
    ui->SessionStart();
    delete ui;
  }

  // Job termination
  // Free the store: user actions, physics_list and detector_description are
  // owned and deleted by the run manager, so they should not be deleted
  // in the main() program !

  delete visManager;
  delete runManager;
  
  return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
