#pragma once

#include "analysis/physics/Physics.h"
class TH1D;
class TTree;

namespace ant {
namespace analysis {
namespace physics {

class Etap3pi0 : public Physics {

protected:

     struct ParticleVars {
        double E;
        double Theta;
        double Phi;
        double IM;
        ParticleVars(const TLorentzVector& lv, const ParticleTypeDatabase::Type& type) noexcept;
        ParticleVars(const data::Particle& p) noexcept;
        ParticleVars(double e=0.0, double theta=0.0, double phi=0.0, double im=0.0) noexcept:
            E(e), Theta(theta), Phi(phi), IM(im) {}
        ParticleVars(const ParticleVars&) = default;
        ParticleVars(ParticleVars&&) = default;
        ParticleVars& operator=(const ParticleVars&) =default;
        ParticleVars& operator=(ParticleVars&&) =default;
        void SetBranches(TTree* tree, const std::string& name);
    };


    std::string dataset;

    TH1D* hNgamma;
    TH1D* hNgammaMC;

    TH1D* h2g;
    TH1D* h6g;

    TH1D* IM_etap;
    TH1D* IM_pi0;

    TH2D* falseNgamma;

    TTree* tree;

    std::vector<ParticleVars> p0cands;
    std::vector<ParticleVars> p0best;
    ParticleVars MMproton;
    ParticleVars etaprime;


public:
    Etap3pi0(PhysOptPtr opts);
    virtual void ProcessEvent(const data::Event& event) override;
    virtual void ShowResult() override;
};


}}}
