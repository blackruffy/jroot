import net.blackruffy.root.*;
import static net.blackruffy.root.JRoot.*;
import static net.blackruffy.root.Pointer.*;
import static java.lang.System.*;
import static java.lang.String.format;

public class ReadTTree {

  public static void main( String[] args ) throws Exception {

    TApplication app = newTApplication("app", newInt(0), new String[]{});
    final TH1F h = newTH1F("", "h", 100, -10, 10);

    final Pointer px = newInt(0);
    final TLorentzVector vec = newTLorentzVector(0, 0, 0, 0);

    final TFile file = newTFile("tree.root", "READ");
    final TTree tree = TTree(file.get("tree"));

    long nev = tree.getEntries();
    
    tree.setBranchAddress("x", px);
    tree.setBranchAddress("vec", vec);

    double sum = 0;
    for( long ev=0; ev<nev; ev++ ) {
      tree.getEntry(ev);
      int x = px.getIntValue();
      double lx = vec.x();
      h.fill(lx);
      out.printf("%d: %d, %f\n", ev, x, lx);
    }

    tree.delete();
    file.close();

    TCanvas c = newTCanvas();
    gPad().setGrid();
    h.setFillColor(38);
    h.draw();
    c.update();
    app.run();

  }

}
