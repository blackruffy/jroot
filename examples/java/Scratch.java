import net.blackruffy.root.*;
import static net.blackruffy.root.JRoot.*;
import static net.blackruffy.root.Pointer.*;
import static java.lang.System.*;
import static java.lang.String.format;

public class Scratch {

  public static void fill() {
    TFile f = newTFile("scratch.root", "RECREATE");
    TTree tree = newTTree("tree", "tree");

    Pointer data = allocate(32);

    tree.branch("data", data, "x/I:y/F:z/D:xs[4]/I");

    TRandom ran = gRandom();
    int nev = 1000;
    for( int ev=0; ev<nev; ev++ ) {
      out.println(ev);
      data.setIntValue(ev);
      data.getPointerAt(4).setFloatValue(Double.valueOf(ran.gaus()).floatValue());
      data.getPointerAt(8).setDoubleValue(ran.gaus());
      int xs[] = new int[4];
      for( int i=0; i<4; i++ ) xs[i] = ev + i;
      data.getPointerAt(16).setIntArray(xs);
      //for( int i=0; i<4; i++ ) data.getPointerAt(16+4*i).setIntValue(ev + i);
      tree.fill();
      f.flush();
    }

    f.write();
    f.close();
  }

  public static void read() {

    final TFile file = newTFile("scratch.root", "READ");
    final TTree tree = TTree(file.get("tree"));

    long nev = tree.getEntries();
    
    Pointer data = allocate(32);

    tree.setBranchAddress("data", data);

    double sum = 0;
    for( long ev=0; ev<nev; ev++ ) {
      tree.getEntry(ev);
      int x = data.getIntValue();
      float y = data.at(4).getFloatValue();
      double z = data.at(8).getDoubleValue();
      int[] xs = data.at(16).getIntArray(4);
      out.printf("%d, %f, %f, [%d, %d, %d, %d]\n", x, y, z, xs[0], xs[1], xs[2], xs[3]);
    }

    tree.delete();
    file.close();

  }

  public static void main( String[] args ) throws Exception {
    read();
  }

}
