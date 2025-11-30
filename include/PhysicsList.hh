#ifndef PHYSICSLIST_HH
#define PHYSICSLIST_HH

#include "G4VModularPhysicsList.hh"
#include "globals.hh"

class PhysicsList : public G4VModularPhysicsList {
public:
    PhysicsList();
    ~PhysicsList();

    // 设置粒子切割值
    void SetCuts() override;
    
    // 构建物理过程
    void ConstructProcess() override;
};

#endif // PHYSICSLIST_HH
