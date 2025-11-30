#include "SteppingAction.hh"
#include "EventAction.hh"
#include "DetectorConstruction.hh"

#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"
#include "G4EventManager.hh"
#include "g4root.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::SteppingAction(EventAction* eventAction)
: fEventAction(eventAction), fGammaCheck(false), fNGplan(false)  
{}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::~SteppingAction()
{}


void SteppingAction::UserSteppingAction(const G4Step* step)
{
    G4Track* track = step->GetTrack();
    G4String particleName = track->GetDefinition()->GetParticleName();
    G4StepPoint* preStepPoint = step->GetPreStepPoint();
    G4String processName = "unknown";

    if (preStepPoint->GetProcessDefinedStep()) {
        processName = preStepPoint->GetProcessDefinedStep()->GetProcessName();
    }
    G4int parentID = track->GetParentID();
    
    if (particleName.find("Xe") != std::string::npos) {
        if (parentID > 0 && processName == "hadElastic") {
            track->SetTrackStatus(fAlive);
        } else if (track->GetCurrentStepNumber() > 1) {
            track->SetTrackStatus(fStopAndKill);
        }
    }

    G4String volumeName = track->GetVolume()->GetName();
    G4bool falg = (volumeName == "Xecylinder" || volumeName == "Scintor");
    if (falg)
    {
        auto analysisManager = G4AnalysisManager::Instance();
        G4float px = preStepPoint->GetPosition().x();
        G4float py = preStepPoint->GetPosition().y();
        G4float pz = preStepPoint->GetPosition().z();
        G4float xp = step->GetPostStepPoint()->GetPosition().x();
        G4float yp = step->GetPostStepPoint()->GetPosition().y();
        G4float zp = step->GetPostStepPoint()->GetPosition().z();
        G4float timeP = step->GetPostStepPoint()->GetGlobalTime();
        
        G4String endprosName = "unknown";
        if (step->GetPostStepPoint()->GetProcessDefinedStep()) {
            endprosName = step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();
        }
        
        G4String creatprosName = "unknown";
        if (preStepPoint->GetProcessDefinedStep()) {
            creatprosName = preStepPoint->GetProcessDefinedStep()->GetProcessName();
        }
        
        G4String tag; 
        if (volumeName == "Xecylinder")
            tag = "Xe"; 
        else if (volumeName == "Scintor")
            tag = "scintor";
        else
            tag = "default";
        G4int copyNo = -1;
        if (volumeName == "Scintor") {
            copyNo = preStepPoint->GetTouchable()->GetCopyNumber();
        }

        G4String particlename = particleName;
        G4double energy = 1000 * preStepPoint->GetKineticEnergy();  // keV
        G4double dE = 1000 * step->GetTotalEnergyDeposit();          // keV
        G4int eventID = G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetEventID();
        G4int trackID = track->GetTrackID();

        analysisManager->FillNtupleDColumn(0, energy);
        analysisManager->FillNtupleDColumn(1, px);
        analysisManager->FillNtupleDColumn(2, py);
        analysisManager->FillNtupleDColumn(3, pz);
        analysisManager->FillNtupleDColumn(4, xp);
        analysisManager->FillNtupleDColumn(5, yp);
        analysisManager->FillNtupleDColumn(6, zp);
        analysisManager->FillNtupleSColumn(7, particlename);
        analysisManager->FillNtupleIColumn(8, eventID);
        analysisManager->FillNtupleIColumn(9, trackID);
        analysisManager->FillNtupleIColumn(10, parentID);
        analysisManager->FillNtupleDColumn(11, dE);
        analysisManager->FillNtupleSColumn(12, creatprosName);
        analysisManager->FillNtupleSColumn(13, endprosName);
        analysisManager->FillNtupleSColumn(14, tag);
        analysisManager->FillNtupleIColumn(15, copyNo);
        analysisManager->FillNtupleDColumn(16, timeP);  
        analysisManager->AddNtupleRow();
    }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

