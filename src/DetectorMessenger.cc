#include "DetectorMessenger.hh"
#include "DetectorConstruction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorMessenger::DetectorMessenger(DetectorConstruction * Det)
:fDetector(Det)
{ 
  fRunparameter = new G4UIdirectory("/Runmodel/");
  fRunparameter->SetGuidance("Run model choose..");

  fRunModel = new G4UIcmdWithAString("/Runmodel/ModelChoose",this);
  fRunModel->SetGuidance("Choose a model to use.");
  fRunModel->SetParameterName("Model",false);
  fRunModel->AvailableForStates(G4State_PreInit,G4State_Idle);
  fRunModel->SetDefaultValue("NaI");


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorMessenger::~DetectorMessenger()
{
  delete fRunparameter;
  delete fRunModel;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{ 
  if( command == fRunModel )
  {
    fDetector->ChooseModel(newValue);
  }
}

