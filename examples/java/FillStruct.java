import net.blackruffy.root.*;
import static net.blackruffy.root.JRoot.*;
import static net.blackruffy.root.Pointer.*;
import static java.lang.System.*;
import static java.lang.String.format;

public class FillStruct {

  public static void main( String args[] ) {
    TFile f = newTFile("struct.root", "RECREATE");
    TTree tree = newTTree("tree", "tree");

    // allocate memory with 32 bytes and get its pointer.
    // in this case, you are going to save a int data, a float data, a double data and a int array with 4 data in an event.
    // to total bytes is
    // int(4 bytes) + float(4 bytes) + double(8 bytes) + int array(4*4 bytes) = 32 bytes
    Pointer data = allocate(32);

    tree.branch("data", data, "x/I:y/F:z/D:xs[4]/I");

    TRandom ran = gRandom();
    int nev = 1000;
    for( int ev=0; ev<nev; ev++ ) {
      out.println(ev);
      data.setIntValue(ev);
      // set a float value at 4 bytes from the address of pointer 'data'.
      data.at(4).setFloatValue(Double.valueOf(ran.gaus()).floatValue());
      // set a double value at 8 bytes from the address of pointer 'data'.
      data.at(8).setDoubleValue(ran.gaus());

      int xs[] = new int[4];
      for( int i=0; i<4; i++ ) xs[i] = ev + i;

      // set an array at 16 bytes from the address of pointer 'data'
      data.at(16).setIntArray(xs);
      tree.fill();
      f.flush();
    }

    f.write();
    f.close();
  }

}
