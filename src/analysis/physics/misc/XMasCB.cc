#include "XMasCB.h"
#include "base/cbtaps_display/TH2CB.h"
#include "base/std_ext/memory.h"
#include "base/std_ext/string.h"
#include "TCanvas.h"

using namespace ant;
using namespace ant::analysis::physics;
using namespace ant::analysis;
using namespace ant::analysis::data;
using namespace std;

XMasCB::XMasCB(const std::string& name, PhysOptPtr opts):
    Physics(name, opts),
    ext(opts->Get<string>("FileType", "pdf")),
    w_px(opts->Get<int>("x_px", 1024))
{
    hist = std_ext::make_unique<TH2CB>("", "", opts->Get<bool>("GluePads", true));
    hist->SetTitle("");
    c    = new TCanvas("xmascb", "XMasCB");
}

XMasCB::~XMasCB()
{}

void XMasCB::ProcessEvent(const Event& event)
{
    hist->ResetElements(0.0);

    for(const auto& c : event.Reconstructed().AllClusters()) {
        if(c.Detector & Detector_t::Type_t::CB) {
            for(const auto& hit : c.Hits) {
                for(const auto& datum : hit.Data) {
                    if(datum.Type == Channel_t::Type_t::Integral) {
                        hist->SetElement(hit.Channel, hist->GetElement(hit.Channel)+datum.Value);
                    }
                }
            }
        }
    }

    c->cd();
    c->SetLogz();
    c->SetMargin(0,0,0,0);

    const double x1 = hist->GetXaxis()->GetXmin();
    const double x2 = hist->GetXaxis()->GetXmax();
    const double y1 = hist->GetYaxis()->GetXmin();
    const double y2 = hist->GetYaxis()->GetXmax();

    const double ratio = (x2-x1)/(y2-y1);

    c->SetCanvasSize(w_px, unsigned(w_px/ratio));

    hist->Draw("col");

    std_ext::formatter f;
    f << "xmas_cb_" << n++ << "." << ext;

    c->SaveAs(f.str().c_str());

}

AUTO_REGISTER_PHYSICS(XMasCB)
