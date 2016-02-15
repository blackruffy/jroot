import net.blackruffy.root.*;
import static net.blackruffy.root.JRoot.*;
import static net.blackruffy.root.Pointer.*;
import static java.lang.System.*;
import static java.lang.String.format;

public class ReadTNtuple {

  public static void main( String[] args ) throws Exception {

    final TFile file = newTFile("tuple.root", "READ");
    final TNtuple tree = TNtuple(file.get("tuple"));

    long nev = tree.getEntries();

    for( long ev=0; ev<nev; ev++ ) {
      tree.getEntry(ev);
      float[] xs = tree.getArgs().getFloatArray(tree.getNvar());
      out.printf("%f, %f, %f\n", xs[0], xs[1], xs[2]);
    }

    tree.delete();
    file.close();

  }

}
