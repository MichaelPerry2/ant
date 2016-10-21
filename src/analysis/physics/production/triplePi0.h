#pragma once

#include "analysis/utils/Fitter.h"
#include "analysis/physics/Physics.h"
#include "analysis/plot/PromptRandomHist.h"
#include "utils/A2GeoAcceptance.h"

#include "base/WrapTTree.h"

#include "TLorentzVector.h"

class TH1D;

namespace ant {
namespace analysis {
namespace physics {

struct triplePi0 :  Physics {

    //===================== Settings   ========================================================


    struct settings_t
    {
        const std::string Tree_Name = "tree";

        const unsigned nPhotons = 6;

        const bool Opt_AllChannels;

        const double    Cut_CBESum     = 550;
        const unsigned  Cut_NCands     = 7;
        const IntervalD Cut_ProtonCopl = {-25,25};
        const IntervalD Cut_MM         = {850,1026};
        const double    Cut_MMAngle    = 20;
        const IntervalD Cut_EMBProb   = {0.2,1};

        const IntervalD              Range_Prompt  =   { -5,  5};
        const std::vector<IntervalD> Ranges_Random = { {-55,-10},
                                                       { 10, 55}  };

        const double fitter_ZVertex = 3;

        const unsigned Index_Data    = 0;
        const unsigned Index_Signal  = 1;
        const unsigned Index_MainBkg = 2;
        const unsigned Index_Offset  = 10;
        const unsigned Index_Unknown = 9;


        settings_t(bool allChannels):
            Opt_AllChannels(allChannels){}
    };

    const settings_t phSettings;

    ant::analysis::utils::A2SimpleGeometry geometry;

    //===================== Channels   ========================================================

    struct named_channel_t
    {
        const std::string Name;
        const ParticleTypeTree DecayTree;
        named_channel_t(const std::string& name, ParticleTypeTree tree):
            Name(name),
            DecayTree(tree){}
    };

    static const named_channel_t              signal;
    static const named_channel_t              mainBackground;
    static const std::vector<named_channel_t> otherBackgrounds;

    //===================== Histograms ========================================================

    TH1D* hist_steps        = nullptr;
    TH1D* hist_channels     = nullptr;
    TH1D* hist_channels_end = nullptr;

    //===================== KinFitting ========================================================


    std::shared_ptr<utils::UncertaintyModel> uncertModel = std::make_shared<utils::UncertaintyModels::FitterSergey>();

    utils::KinFitter kinFitterEMB;

    utils::TreeFitter fitterSig;
    std::vector<utils::TreeFitter::tree_t> pionsFitterSig;

    utils::TreeFitter fitterBkg;
    std::vector<utils::TreeFitter::tree_t> pionsFitterBkg;


    //========================  ProptRa. ============================================================

    ant::analysis::PromptRandom::Switch promptrandom;

    //========================  Storage  ============================================================

    struct PionProdTree : WrapTTree
    {
        // type: 0   data
        //       1   signal (3pi0)
        //       2   mainBkg(eta->3pi0)
        //       10+ otherBkg
        ADD_BRANCH_T(unsigned, MCTrue)

        ADD_BRANCH_T(double,   Tagg_W)
        ADD_BRANCH_T(unsigned, Tagg_Ch)
        ADD_BRANCH_T(double,   Tagg_E)

        ADD_BRANCH_T(double, CBAvgTime)
        ADD_BRANCH_T(double, CBESum)

        // best emb combination raw
        ADD_BRANCH_T(TLorentzVector,              proton)
        ADD_BRANCH_T(std::vector<TLorentzVector>, photons)
        ADD_BRANCH_T(TLorentzVector,              photonSum)
        ADD_BRANCH_T(double,                      IM6g)
        // best emb comb. emb-fitted
        ADD_BRANCH_T(TLorentzVector,              EMB_proton)
        ADD_BRANCH_T(std::vector<TLorentzVector>, EMB_photons)
        ADD_BRANCH_T(TLorentzVector,              EMB_photonSum)
        ADD_BRANCH_T(double,                      EMB_IM6g)
        ADD_BRANCH_T(double,                      EMB_Ebeam)
        ADD_BRANCH_T(double,                      EMB_prob)
        ADD_BRANCH_T(int,                         EMB_iterations)


        ADD_BRANCH_T(double, SIG_prob)
        ADD_BRANCH_T(int,                         SIG_iterations)


        ADD_BRANCH_T(double, BKG_prob)
        ADD_BRANCH_T(int,                         BKG_iterations)
    };
    PionProdTree tree;


    triplePi0(const std::string& name, OptionsPtr opts);
    virtual void ProcessEvent(const TEvent& event, manager_t& manager) override;
    virtual void Finish() override {}
    virtual void ShowResult() override;



    void FillStep(const std::string& step) {hist_steps->Fill(step.c_str(),1);}


};


}}}