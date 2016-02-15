import net.blackruffy.root.*;
import static net.blackruffy.root.JRoot.*;
import static java.lang.System.*;
import static java.lang.String.format;

public class FillTNtuple {

  public static void main( String[] args ) {

    TFile f = newTFile("tuple.root", "RECREATE");
    TNtuple tuple = newTNtuple("tuple", "tuple", "x:y:z");

    TRandom ran = newTRandom();

    int nev = 10000;
    for( int ev=0; ev<nev; ev++ ) {
      float x = (float)ran.gaus();
      float y = (float)ran.gaus();
      float z = (float)ran.gaus();
      tuple.fill(x, y, z);
    }

    f.write();
    f.close();

  }

}
