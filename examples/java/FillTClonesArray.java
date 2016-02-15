import net.blackruffy.root.*;
import static net.blackruffy.root.JRoot.*;
import static net.blackruffy.root.Pointer.*;
import static java.lang.System.*;
import static java.lang.String.format;

public class FillTClonesArray {

  public static void main( String args[] ) {
    TFile file = newTFile("clones.root", "RECREATE");
    TTree tree = newTTree("tree", "tree");

    TClonesArray xs = newTClonesArray("TLorentzVector", 10);

    tree.branch("xs", "TClonesArray", xs);

    int nev = 1000;
    for( int ev=0; ev<nev; ev++ ) {
      for( int n=0; n<10; n++ ) {
        TLorentzVector v = TLorentzVector(xs.constructedAt(n));
        v.setXYZT(
          gRandom().gaus(),
          gRandom().gaus(),
          gRandom().gaus(),
          gRandom().gaus()
        );
      }
      tree.fill();
      xs.clear();
    }

    file.write();
    file.close();

  }

}
