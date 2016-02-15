#include <addr.h>
#include <croot.h>
#include <iostream>
#include <TLorentzVector.h>

int main( int argc, char **argv ) {

  addr_t file = TFile_newTFile_3("tree.root", "RECREATE");
  addr_t tree = TTree_newTTree_1("tree", "tree");

  addr_t vec = TLorentzVector_newTLorentzVector_4(9, 8, 7, 6);

  addr_t px = Pointer_allocate(4);
  TTree_branch_16(tree, "x", px, "x/I");
  TTree_branch_13(tree, "vec", "TLorentzVector", vec);

  addr_t random = TRandom_newTRandom_2();

  int nev = 10000;
  for( int ev=0; ev<nev; ev++ ) {
    double r = TRandom_gaus_1(random);
    Pointer_setIntValue( px, (int)(r*100) );
    TLorentzVector_setXYZT_1(vec, ev, ev, ev, ev);
    TTree_fill_1(tree);
  }
  
  TFile_write_1(file);
  TFile_close_1(file);
  
}
