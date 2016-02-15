#include <addr.h>
#include <croot.h>
#include <iostream>

using namespace std;

int main( int argc, char **argv ) {

  addr_t file = TFile_newTFile_3("clones.root", "READ");
  addr_t tree = TFile_get_1(file, "tree");

  addr_t xs = TClonesArray_newTClonesArray_2("TLorentzVector");

  TTree_setBranchAddress_100( tree, "xs", "TClonesArray", xs );
  
  long nev = TTree_getEntries_1(tree);
  for( int ev=0; ev<nev; ev++ ) {
    TTree_getEntry_2(tree, ev);
    addr_t vec = TClonesArray_at_1(xs, 0);
    std::cout << ev << " " << TLorentzVector_x_1(vec) << std::endl;
  }
  
  TFile_close_1(file);
  
}
