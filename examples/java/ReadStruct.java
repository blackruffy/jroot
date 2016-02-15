import net.blackruffy.root.*;
import static net.blackruffy.root.JRoot.*;
import static net.blackruffy.root.Pointer.*;
import static java.lang.System.*;
import static java.lang.String.format;

public class ReadStruct {

  public static void main( String args[] ) {

    final TFile file = newTFile("struct.root", "READ");
    final TTree tree = TTree(file.get("tree"));

    long nev = tree.getEntries();

    // allocate memory to store data in branches.
    // you need 32 bytes(int, float, double, int array with 4 elements).
    Pointer data = allocate(32);

    tree.setBranchAddress("data", data);

    double sum = 0;
    for( long ev=0; ev<nev; ev++ ) {
      tree.getEntry(ev);
      // get int value at the first address of pointer.
      int x = data.getIntValue();
      // get float value at 4 bytes from address of pointer.
      float y = data.at(4).getFloatValue();
      double z = data.at(8).getDoubleValue();
      // get int array with 4 element at 16 bytes from address of pointer.
      int[] xs = data.at(16).getIntArray(4);

      out.printf("%d, %f, %f, [%d, %d, %d, %d]\n", x, y, z, xs[0], xs[1], xs[2], xs[3]);
    }

    tree.delete();
    file.close();

  }

}
