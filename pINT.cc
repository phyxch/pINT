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
//   Nov 5, 2021: Hexc
//          Fixing the main function for running multithreaded mode
//
//   Apr 26, 2022: Hexc and Thomas Mulkey
//          Added a scoring manager to the run manager. 
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


//#ifdef G4MULTITHREADED
//#include "G4MTRunManager.hh"
//#else
//#include "G4RunManager.hh"
//#endif

#include "G4RunManagerFactory.hh"

#include "Randomize.hh"
#include "pINTDetectorConstruction.hh"
#include "QGSP_BERT_HP.hh"
#include "pINTPrimaryGeneratorAction.hh"
#include "pINTRunAction.hh"
#include "pINTEventAction.hh"
#include "pINTSteppingAction.hh"
#include "pINTSteppingVerbose.hh"
#include "pINTActionInitialization.hh"

#include "pINTOutput.hh"

#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "G4ScoringManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

namespace {
  void PrintUsage() {
    G4cerr << " Usage: " << G4endl;
    G4cerr << " pINT [-m macro ] [-u UIsession] [-t nThreads]" << G4endl;
    G4cerr << "   note: -t option is available only for multi-threaded mode."
           << G4endl;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc, char** argv)
{
  // Evaluate arguments
  //
  if ( argc > 7 ) {
    PrintUsage();
    return 1;
  }
  
  // Detect interactive mode (if no arguments) and define UI session
  //
  //  G4UIExecutive* ui = 0;
  //if ( argc == 1 ) {
  //  ui = new G4UIExecutive(argc, argv);
  //}

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
  /*
    #ifdef G4MULTITHREADED
    G4MTRunManager* runManager = new G4MTRunManager;
    G4cout << "<<<<<<Running multi-threaded mode.>>>>>>" << G4endl;
    runManager->SetNumberOfThreads(2);
    #else
    G4RunManager* runManager = new G4RunManager;
    G4cout << "<<<<<<Running single-threaded mode.>>>>>>" << G4endl;
    #endif
  */

  G4String macro;
  G4String session;
#ifdef G4MULTITHREADED
  G4int nThreads = 6;
#endif
  for ( G4int i=1; i<argc; i=i+2 ) {
    if      ( G4String(argv[i]) == "-m" ) macro = argv[i+1];
    else if ( G4String(argv[i]) == "-u" ) session = argv[i+1];
#ifdef G4MULTITHREADED
    else if ( G4String(argv[i]) == "-t" ) {
      nThreads = G4UIcommand::ConvertToInt(argv[i+1]);
    }
#endif
    else {
      PrintUsage();
      return 1;
    }
  }

  // Detect interactive mode (if no macro provided) and define UI session
  //
  G4UIExecutive* ui = nullptr;
  if ( ! macro.size() ) {
    ui = new G4UIExecutive(argc, argv, session);
  }

  // Construct the default run manager
  //
  auto* runManager =
    G4RunManagerFactory::CreateRunManager(G4RunManagerType::Default);
#ifdef G4MULTITHREADED
  if ( nThreads > 0 ) { 
    runManager->SetNumberOfThreads(nThreads);
  }  
#endif

  // Add a scoring manager
  //
  auto scoring = G4ScoringManager::GetScoringManager();
    
  // Set mandatory initialization classes
  //
  // Detector construction
  // 
  auto detector = new pINTDetectorConstruction;
  runManager->SetUserInitialization(detector);
  
  //
  //  G4VModularPhysicsList* physicsList = new FTFP_BERT;   // commented out on 4/12/2019
  auto physicsList = new QGSP_BERT_HP;    // added on 4/12/2019
  runManager->SetUserInitialization(physicsList);

  // Set user action initialization
  //
  auto actionInitialization = new pINTActionInitialization(detector);
  runManager->SetUserInitialization(actionInitialization);

  //  G4cout << "Got here!" << G4endl;
  
  // Initialize visualization
  //
  auto visManager = new G4VisExecutive;
  // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
  // G4VisManager* visManager = new G4VisExecutive("Quiet");
  visManager->Initialize();

  // Get the pointer to the User Interface manager
  auto UImanager = G4UImanager::GetUIpointer();

  // Process macro or start UI session
  //
  if ( macro.size() ) {
    // batch mode
    G4String command = "/control/execute ";
    UImanager->ApplyCommand(command+macro);
  }
  else  {  
    // interactive mode : define UI session
    UImanager->ApplyCommand("/control/execute init_vis.mac");
    if (ui->IsGUI()) {
      UImanager->ApplyCommand("/control/execute gui.mac");
    }
    ui->SessionStart();
    delete ui;
  }
  
  // Activate score ntuple writer
  // The Root output type (Root) is selected in B3Analysis.hh.
  // The verbose level can be also set via UI commands
  // /score/ntuple/writerVerbose level
  //G4TScoreNtupleWriter<G4AnalysisManager> scoreNtupleWriter;
  //scoreNtupleWriter.SetVerboseLevel(1);

  // Process macro or start UI session
  //
  /*
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
  */


  delete visManager;
  delete runManager;
  
  return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
