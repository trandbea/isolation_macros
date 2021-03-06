// plot_distribution: Macro that plots variables both lumi weighted and normalized to the same area.

#include <iostream>
#include <vector>

#include "TChain.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TLine.h"
#include "TString.h"
#include "TColor.h"

#include "styles.hpp"
#include "utilities.hpp"

using namespace std;
using std::cout;
using std::endl;

class hfeats {
public:
  hfeats(TString ivarname, int inbins, float iminx, float imaxx, vector<int> isamples,
	 TString ititle="", TString icuts="1", float icut=-1){
    varname = ivarname; nbins = inbins; minx = iminx; maxx = imaxx; title = ititle;
    cuts = icuts; cut = icut; samples = isamples;
    tag = ivarname+"_"+cuts; tag.ReplaceAll("_1",""); tag.ReplaceAll(".",""); 
    tag.ReplaceAll("(",""); tag.ReplaceAll("$","");  tag.ReplaceAll(")",""); 
    tag.ReplaceAll("[",""); tag.ReplaceAll("]",""); tag.ReplaceAll("||","_");
    tag.ReplaceAll("/","_"); tag.ReplaceAll("*",""); tag.ReplaceAll("&&","_");
    tag.ReplaceAll(">",""); tag.ReplaceAll("<",""); tag.ReplaceAll("=","");
    tag.ReplaceAll("+",""); 
    unit = "";
    if(title.Contains("GeV)")) unit = "GeV";
    if(title.Contains("phi")) unit = "rad";
  }
  TString title, varname, tag, cuts, unit;
  int nbins;
  float minx, maxx, cut;
  vector<int> samples;
};

class sfeats {
public:
  sfeats(vector<TString> ifile, TString ilabel, int icolor, int istyle=1, TString icut="1"){
    file = ifile; label = ilabel; cut = icut;
    color = icolor; style = istyle;
    isSig = ifile[0].Contains("T1tttt");// && ifile.Contains("1200");
    factor = "1";
    if(ifile[0].Contains("TTW")) factor = "0.36";
  }
  vector<TString> file;
  TString label, cut, factor;
  int color, style;
  bool isSig;
};

int main(){ 
  styles style("Paper"); style.setDefaultStyle();
  vector<hfeats> vars;
  TCanvas can;

  TColor ucsb_blue(1000, 1/255.,57/255.,166/255.);
  TColor ucsb_gold(1001, 255/255.,200/255.,47/255);
   TColor penn_red(1002, 149/255.,0/255.,26/255.);
  TColor uf_orange(1003, 255/255.,74/255.,0/255.);
  TColor uo_green(1004, 0/255.,79/255.,39/255.);
  TColor tcu_purple(1005, 52/255.,42/255.,123/255.);
  TColor tar_heel_blue(1006, 86/255.,160/255.,211/255.);
  TColor sig_teal(1007, 96/255.,159/255.,128/255.);
  TColor sig_gold(1008, 215/255.,162/255.,50/255.);
  TColor seal_brown(1010, 89/255.,38/255.,11/255.);

  TString folder="archive/15-03-03/skim/";
  //TString folder="archive/15-03-03/";
  vector<TString> s_tt;
  s_tt.push_back(folder+"*_TTJet*");
  vector<TString> s_wjets;
  s_wjets.push_back(folder+"*WJets*");
  vector<TString> s_single;
  s_single.push_back(folder+"*_T*channel*");
  vector<TString> s_ttv;
  s_ttv.push_back(folder+"*TTW*");
  s_ttv.push_back(folder+"*TTZ*");
  vector<TString> s_other;
  s_other.push_back(folder+"*QCD_HT*");
  s_other.push_back(folder+"*_ZJet*");
  s_other.push_back(folder+"*DY*");
  s_other.push_back(folder+"*WH_HToBB*");
  vector<TString> s_t1t;
  s_t1t.push_back(folder+"*T1tttt*1500_*PU20*");
  vector<TString> s_t1tc;
  s_t1tc.push_back(folder+"*T1tttt*1200_*PU20*");

  // Reading ntuples
  vector<TChain *> chain;
  vector<sfeats> Samples; 
  Samples.push_back(sfeats(s_other, "Other", 1001));
  Samples.push_back(sfeats(s_ttv, "ttV", 1002));
  Samples.push_back(sfeats(s_single, "Single top", 1005));
  Samples.push_back(sfeats(s_wjets, "W + jets", 1004));
  Samples.push_back(sfeats(s_tt, "t#bar{t}, 2 l", 1006,1,"((mc_type&0x0F00)/0x100+(mc_type&0x000F)-(mc_type&0x00F0)/0x10)>=2"));
  Samples.push_back(sfeats(s_tt, "t#bar{t}, 1 l", 1000,1,"((mc_type&0x0F00)/0x100+(mc_type&0x000F)-(mc_type&0x00F0)/0x10)<=1"));
  Samples.push_back(sfeats(s_t1t, "T1tttt(1500,100)", 2));
  Samples.push_back(sfeats(s_t1tc, "T1tttt(1200,800)", 2,2));

  for(unsigned sam(0); sam < Samples.size(); sam++){
    chain.push_back(new TChain("tree"));
    for(unsigned insam(0); insam < Samples[sam].file.size(); insam++)
      chain[sam]->Add(Samples[sam].file[insam]);
  }

  vector<int> mj_sam_lep;
  mj_sam_lep.push_back(0);
  mj_sam_lep.push_back(1);
  mj_sam_lep.push_back(2);
  mj_sam_lep.push_back(3);
  mj_sam_lep.push_back(4);
  mj_sam_lep.push_back(5);
  mj_sam_lep.push_back(6);
  mj_sam_lep.push_back(7);

  //vars.push_back(hfeats("met",20,0,800, mj_sam_lep, "MET (GeV)","ht>500&&nbm>=2&&njets>=6&&(Max$(els_pt*els_sigid*(els_miniso_tr10<0.1))>20||Max$(mus_pt*mus_sigid*(mus_miniso_tr10<0.4))>20)&&(nmus+nels)==1",250));
  //vars.push_back(hfeats("ht",35,0,3500, mj_sam_lep, "H_{T} (GeV)","met>250&&nbm>=2&&njets>=6&&(Max$(els_pt*els_sigid*(els_miniso_tr10<0.1))>20||Max$(mus_pt*mus_sigid*(mus_miniso_tr10<0.4))>20)&&(nmus+nels)==1",500));
  vars.push_back(hfeats("njets",17,-0.5,16.5, mj_sam_lep, "Number of 40 GeV jets","ht>500&&met>250&&nbm>=2&&(Max$(els_pt*els_sigid*(els_miniso_tr10<0.1))>20||Max$(mus_pt*mus_sigid*(mus_miniso_tr10<0.4))>20)&&(nmus+nels)==1",5.5));
  vars.push_back(hfeats("nbm",8,-0.5,7.5, mj_sam_lep, "Number of b-tags (CSVM)","ht>500&&met>250&&njets>=6&&(Max$(els_pt*els_sigid*(els_miniso_tr10<0.1))>20||Max$(mus_pt*mus_sigid*(mus_miniso_tr10<0.4))>20)&&(nmus+nels)==1",1.5));

  // vars.push_back(hfeats("mt",20,0,600, mj_sam_lep, "m_{T} (GeV)","ht>500&&met>250&&nbm>=2&&njets>=6&&(Max$(els_pt*els_sigid*(els_miniso_tr10<0.1))>20||Max$(mus_pt*mus_sigid*(mus_miniso_tr10<0.4))>20)&&(nmus+nels)==1",150));
  // vars.push_back(hfeats("mj",20,0,1600, mj_sam_lep, "M_{J} (GeV)","ht>500&&met>250&&nbm>=2&&njets>=6&&mt>150&&(Max$(els_pt*els_sigid*(els_miniso_tr10<0.1))>20||Max$(mus_pt*mus_sigid*(mus_miniso_tr10<0.4))>20)&&(nmus+nels)==1",600));



  TString cuts("ht>750&&met>250&&nbm>=2&&njets>=6&&(nmus+nels)==1");
  //vars.push_back(hfeats("Max$(fjets_r08_m)",40,0,800, mj_sam_lep, "Max(m_{J}^{R=0.8}) (GeV)",cuts));
  // vars.push_back(hfeats("Max$(fjets_30_m)", 40,0,800, mj_sam_lep, "Max(m_{J}^{R=1.2}) (GeV)",cuts));
  cuts ="ht>750&&met>250&&nbl>=2&&njets>=4&&mt>150&&(nmus+nels)==1";
  //vars.push_back(hfeats("Max$(fjets_r08_m)",40,0,800, mj_sam_lep, "Max(m_{J}^{R=0.8}) (GeV)",cuts));
  // vars.push_back(hfeats("Max$(fjets_30_m)", 40,0,800, mj_sam_lep, "Max(m_{J}^{R=1.2}) (GeV)",cuts));

  // vars.push_back(hfeats("mj_r08",40,0,2000, mj_sam_lep, "M{J}^{R=0.8} (GeV)",cuts));
  // vars.push_back(hfeats("mj_30",40,0,2000, mj_sam_lep, "M{J}^{R=1.2} (GeV)",cuts));


  TString luminosity="4";
  float minLog = 0.04, maxLog = 10;
  double legX = 0.6, legY = 0.89, legSingle = 0.055;
  double legW = 0.12, legH = legSingle*vars[0].samples.size();
  TLegend leg(legX, legY-legH, legX+legW, legY);
  leg.SetTextSize(0.052); leg.SetFillColor(0); leg.SetFillStyle(0); leg.SetBorderSize(0);
  leg.SetTextFont(132);

  TLine line; line.SetLineColor(28); line.SetLineWidth(4); line.SetLineStyle(2);
  vector< vector<TH1D*> > histo[2];
  vector<TH1D*> varhisto;
  vector<float> nentries;
  TString hname, pname, variable, leghisto, totCut, title, ytitle;
  for(unsigned var(0); var<vars.size(); var++){
    cout<<endl;
    // Generating vector of histograms
    title = vars[var].cuts; if(title=="1") title = "";
    title.ReplaceAll("nvmus==1&&nmus==1&&nvels==0","1 #mu");
    title.ReplaceAll("nvmus10==0&&nvels10==0", "0 leptons");  
    title.ReplaceAll("(Max$(els_pt*els_sigid*(els_miniso_tr10<0.1))>20||Max$(mus_pt*mus_sigid*(mus_miniso_tr10<0.4))>20)&&(nmus+nels)", "n_{lep}");  title.ReplaceAll("njets30","n_{jets}^{30}"); 
    title.ReplaceAll("els_pt","p^{e}_{T}");title.ReplaceAll("mus_pt","p^{#mu}_{T}");
    title.ReplaceAll("mus_reliso","RelIso"); title.ReplaceAll("els_reliso","RelIso");
    title.ReplaceAll("mus_miniso_tr15","MiniIso"); title.ReplaceAll("els_miniso_tr15","MiniIso");
    title.ReplaceAll("njets","n_{jets}");title.ReplaceAll("abs(lep_id)==13&&","");
    title.ReplaceAll(">=", " #geq "); title.ReplaceAll(">", " > "); title.ReplaceAll("&&", ", "); 
    title.ReplaceAll("met", "MET"); title.ReplaceAll("ht", "H_{T}");  title.ReplaceAll("mt", "m_{T}"); 
    title.ReplaceAll("nleps==1", "1 lepton");  title.ReplaceAll("nbm","n_{b}"); title.ReplaceAll("==", " = "); 
    title.ReplaceAll("nbl[1]","n_{b,l}");
    for(unsigned his(0); his < 2; his++){
      varhisto.resize(0);
      for(unsigned sam(0); sam < vars[var].samples.size(); sam++){
	hname = "histo"; hname += var; hname += his; hname += sam;
	varhisto.push_back(new TH1D(hname, title, vars[var].nbins, vars[var].minx, vars[var].maxx));
      }
      histo[his].push_back(varhisto);
    }

    //// Plotting lumi-weighted distributions in histo[0], and then area-normalized in histo[1] ///
    leg.Clear();
    nentries.resize(0);
    variable = vars[var].varname;
    float maxhisto(-999);
    for(unsigned sam(0); sam < vars[var].samples.size(); sam++){
      int isam = vars[var].samples[sam];
      bool isSig = Samples[isam].isSig;
      totCut = Samples[isam].factor+"*"+luminosity+"*weight*("+vars[var].cuts+"&&"+Samples[isam].cut+")"; 
      //cout<<totCut<<endl;
      chain[isam]->Project(histo[0][var][sam]->GetName(), variable, totCut);
      histo[0][var][sam]->SetBinContent(vars[var].nbins,
					  histo[0][var][sam]->GetBinContent(vars[var].nbins)+
					  histo[0][var][sam]->GetBinContent(vars[var].nbins+1));
      nentries.push_back(histo[0][var][sam]->Integral(1,vars[var].nbins));
      histo[0][var][sam]->SetXTitle(vars[var].title);
      ytitle = "Entries for "+luminosity+" fb^{-1}";
      if(vars[var].unit!="") {
	int digits(0);
	float binwidth((vars[var].maxx-vars[var].minx)/static_cast<float>(vars[var].nbins));
	if(binwidth<1) digits = 1;
	ytitle += ("/("+RoundNumber(binwidth,digits) +" "+vars[var].unit+")");
      }
      histo[0][var][sam]->SetYTitle(ytitle);
      // Cloning histos for later
      for(int bin(0); bin<=histo[0][var][sam]->GetNbinsX()+1; bin++)
	histo[1][var][sam]->SetBinContent(bin, histo[0][var][sam]->GetBinContent(bin));

      if(!isSig){ // Adding previous bkg histos
	for(int bsam(sam-1); bsam >= 0; bsam--){
	  histo[0][var][sam]->Add(histo[0][var][bsam]);
	  break;
	  // if(!Samples[vars[var].samples[bsam]].file[0].Contains("T1tttt")){
	  //   histo[0][var][sam]->Add(histo[0][var][bsam]);
	  //   break;
	  // }
	}
	histo[0][var][sam]->SetFillColor(Samples[isam].color);
	histo[0][var][sam]->SetFillStyle(1001);
	histo[0][var][sam]->SetLineColor(1);
	histo[0][var][sam]->SetLineWidth(1);
      } else {
	histo[0][var][sam]->SetLineColor(Samples[isam].color);
	histo[0][var][sam]->SetLineStyle(Samples[isam].style);
	histo[0][var][sam]->SetLineWidth(3);
      }
      if(maxhisto < histo[0][var][sam]->GetMaximum()) maxhisto = histo[0][var][sam]->GetMaximum();
    } // First loop over samples
    int firstplotted(-1);
    for(int sam(vars[var].samples.size()-1); sam >= 0; sam--){
      int isam = vars[var].samples[sam];
      leghisto = Samples[isam].label+" [N = " + RoundNumber(nentries[sam],0) + "]";
      leg.AddEntry(histo[0][var][sam], leghisto);
      bool isSig = Samples[isam].isSig;
      if(!isSig){
	if(firstplotted < 0) {
	  histo[0][var][sam]->Draw();
	  firstplotted = sam;
	} else histo[0][var][sam]->Draw("same");
      }
    }
    for(int sam(vars[var].samples.size()-1); sam >= 0; sam--){
      int isam = vars[var].samples[sam];
      bool isSig = Samples[isam].isSig;
      if(isSig) histo[0][var][sam]->Draw("same");
    }
    legH = legSingle*vars[var].samples.size(); leg.SetY1NDC(legY-legH);
    leg.Draw(); 
    if(histo[0][var][firstplotted]->GetMinimum() > minLog) histo[0][var][firstplotted]->SetMinimum(minLog);
    histo[0][var][firstplotted]->SetMinimum(minLog);
    histo[0][var][firstplotted]->SetMaximum(maxhisto*maxLog);
    if(variable=="mt" && var==vars.size()-1) {
      histo[0][var][firstplotted]->SetMinimum(0.2);
      histo[0][var][firstplotted]->SetMaximum(maxhisto*2);
    }
    if(vars[var].cut>0) line.DrawLine(vars[var].cut, 0, vars[var].cut, maxhisto*maxLog);
    can.SetLogy(1);
    pname = "plots/1d/log_lumi_"+vars[var].tag+".eps";
    can.SaveAs(pname);
    histo[0][var][firstplotted]->SetMinimum(0);
    histo[0][var][firstplotted]->SetMaximum(maxhisto*1.1);
    can.SetLogy(0);
    pname = "plots/1d/lumi_"+vars[var].tag+".eps";
    can.SaveAs(pname);

    //////////// Plotting area-normalized distributions ////////////
    leg.Clear(); maxhisto = -999;
    for(unsigned sam(0); sam < vars[var].samples.size(); sam++){
      int isam = vars[var].samples[sam];
      histo[1][var][sam]->SetLineColor(Samples[isam].color);
      histo[1][var][sam]->SetLineStyle(Samples[isam].style);
      histo[1][var][sam]->SetLineWidth(3);
      if(nentries[sam]) histo[1][var][sam]->Scale(100./nentries[sam]);
      if(maxhisto < histo[1][var][sam]->GetMaximum()) maxhisto = histo[1][var][sam]->GetMaximum();
      if(sam==0){
	histo[1][var][sam]->SetXTitle(vars[var].title);
	histo[1][var][sam]->SetYTitle("Entries (%)");
	histo[1][var][sam]->Draw();
      } else histo[1][var][sam]->Draw("same");
      leghisto = Samples[isam].label+" [#mu = ";
      int digits(1);
      leghisto += RoundNumber(histo[1][var][sam]->GetMean(),digits) + "]";
      leg.AddEntry(histo[1][var][sam], leghisto);
    } // Loop over samples
    leg.Draw(); 
    if(vars[var].cut>0) line.DrawLine(vars[var].cut, 0, vars[var].cut, maxhisto*1.1);
    histo[1][var][0]->SetMaximum(maxhisto*1.1);
    can.SetLogy(0);
    pname = "plots/1d/shapes_"+vars[var].tag+".eps";
    can.SaveAs(pname);
    histo[1][var][0]->SetMaximum(maxhisto*maxLog);
    can.SetLogy(1);
    pname = "plots/1d/log_shapes_"+vars[var].tag+".eps";
    can.SaveAs(pname);
  }// Loop over variables

  for(unsigned his(0); his < 2; his++){
    for(unsigned var(0); var<vars.size(); var++){
      for(unsigned sam(0); sam < vars[var].samples.size(); sam++)
	if(histo[his][var][sam]) histo[his][var][sam]->Delete();
    }
  }
}

