import net.blackruffy.root.*;
import static net.blackruffy.root.JRoot.*;
import static java.lang.System.*;
import static java.lang.String.format;

public class FillTTree {

  public static void main( String[] args ) {

    TFile f = newTFile("tree.root", "RECREATE");
    TTree tree = newTTree("tree", "tree");

    Pointer px = Pointer.newInt();
    TLorentzVector vec = newTLorentzVector(0, 0, 0, 0);

    tree.branch("x", px, "x/I");
    tree.branch("vec", "TLorentzVector", vec);

    TRandom ran = newTRandom();
    int nev = 10000;
    for( int ev=0; ev<nev; ev++ ) {
      if( ev%1000 == 0 ) out.println(ev);
      px.setIntValue((int)(ran.gaus()*100));
      vec.setX(ran.gaus());
      tree.fill();
      // f.flush();
    }

    f.write();
    f.close();

  }

}
