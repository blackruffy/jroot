import net.blackruffy.root.*;
import static net.blackruffy.root.JRoot.*;
import static net.blackruffy.root.Pointer.*;
import static java.lang.System.*;
import static java.lang.String.format;

public class ReadTClonesArray {

  public static void main( String args[] ) {
    TH1F h = newTH1F("", "h", 100, 0, 5);

    TFile file = newTFile("clones.root", "READ");
    TTree tree = TTree(file.get("tree"));

    TClonesArray xs = newTClonesArray("TLorentzVector");

    tree.setBranchAddress("xs", xs);

    long nev = tree.getEntries();
    for( int ev=0; ev<nev; ev++ ) {
      out.printf("%d/%d\n", ev, nev);
      tree.getEntry(ev);
      for( int n=0; n<xs.getEntries(); n++ ) {
        TLorentzVector v = TLorentzVector(xs.at(n));
        h.fill(v.p());
      }
      xs.clear();
    }

    file.close();

    TApplication app = newTApplication("app", newInt(0), new String[]{});
    TCanvas c = newTCanvas();
    gPad().setGrid();
    h.draw();
    c.update();
    app.run();

  }

}
