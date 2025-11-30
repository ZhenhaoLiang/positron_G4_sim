#ifndef DetectorMessenger_h
#define DetectorMessenger_h 1

#include "G4UImessenger.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithoutParameter.hh"

class DetectorConstruction;
class G4UIcmdWithABool;
class G4UIcmdWithADouble;
class G4UIcmdWithAString;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class DetectorMessenger: public G4UImessenger
{
  public:
  
    DetectorMessenger(DetectorConstruction* );
   ~DetectorMessenger();
    void SetNewValue(G4UIcommand*, G4String);
    void SetNewBool(G4UIcmdWithABool*, G4String);

  private:
  
    DetectorConstruction*   fDetector;
    G4UIdirectory* fRunparameter;
    G4UIcmdWithAString* fRunModel;
};
#endif

