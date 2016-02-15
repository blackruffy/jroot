#include <addr.h>
#include <croot.h>
#include <iostream>

using namespace std;

int main( int argc, char **argv ) {

  addr_t file = TFile_newTFile_3("tree.root", "READ");
  addr_t tree = TFile_get_1(file, "tree");
  addr_t px = Pointer_allocate(4);
  addr_t vec = TLorentzVector_newTLorentzVector_4(0, 0, 0, 0);

  TTree_setBranchAddress_1( tree, "x", px);
  TTree_setBranchAddress_100( tree, "vec", "TLorentzVector", vec );
  
  //long nev = TTree_getEntries_1(tree);
  //for( int ev=0; ev<nev; ev++ ) {
  //  TTree_getEntry_2(tree, ev);
  //  std::cout << ev << " " << TLorentzVector_x_1(vec) << " " << Pointer_getIntValue(px) << std::endl;
  //}

  TTree_printAllEvent( tree, vec );
  
  TFile_close_1(file);
  
}
