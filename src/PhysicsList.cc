#include "PhysicsList.hh"
#include "G4DecayPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "G4EmStandardPhysics.hh"
#include "G4HadronElasticPhysicsHP.hh"
#include "G4HadronPhysicsQGSP_BIC_HP.hh"
#include "G4IonPhysics.hh"
#include "G4StoppingPhysics.hh"
#include "G4NeutronTrackingCut.hh"
#include "G4LossTableManager.hh"
#include "G4GenericIon.hh"
#include "G4UserLimits.hh"
#include "G4SystemOfUnits.hh"
using namespace CLHEP;

PhysicsList::PhysicsList() : G4VModularPhysicsList() {
    SetVerboseLevel(1);

    // 添加标准物理过程
    RegisterPhysics(new G4DecayPhysics());
    RegisterPhysics(new G4RadioactiveDecayPhysics());
    RegisterPhysics(new G4EmStandardPhysics());
    RegisterPhysics(new G4HadronElasticPhysicsHP());
    RegisterPhysics(new G4HadronPhysicsQGSP_BIC_HP()); // 处理中子非弹性散射
    RegisterPhysics(new G4IonPhysics()); // 确保离子（包括 Xe）可以正确追踪
    RegisterPhysics(new G4StoppingPhysics()); // 处理高能粒子停止
    RegisterPhysics(new G4NeutronTrackingCut()); // 允许跟踪低能中子
}

PhysicsList::~PhysicsList() {}

void PhysicsList::SetCuts() {
    // 调整默认的粒子跟踪切割能量，以确保 Xe 运动被记录
    SetCutValue(1 * mm, "gamma");
    SetCutValue(1 * mm, "e-");
    SetCutValue(1 * mm, "e+");
    SetCutValue(0.1 * mm, "proton"); // 让低能质子更容易被追踪
    SetCutValue(0.1 * mm, "neutron");
    SetCutValue(0.01 * mm, "ion"); // 关键！降低 Xe 的切割值

    // 确保 Geant4 识别所有 Xe 同位素
    G4GenericIon::GenericIonDefinition();
}

void PhysicsList::ConstructProcess() {
    G4VModularPhysicsList::ConstructProcess();

    // 确保 ion 过程不会被忽略
    G4IonPhysics* ionPhysics = new G4IonPhysics();
    ionPhysics->ConstructProcess();
}
