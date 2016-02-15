import net.blackruffy.root.*;
import static net.blackruffy.root.JRoot.*;
import static net.blackruffy.root.Pointer.*;

public class DrawTGraph {

  public static void main( String[] args ) {

    final TApplication app = newTApplication( "app", newInt(0), new String[]{} );

    final int size = 100;
    final double[] xs = new double[size];
    final double[] ys = new double[size];
    for( int i=0; i<size; i++ ) {
      xs[i] = Math.PI/50*i;
      ys[i] = Math.sin(xs[i]);
    }

    TGraph g = newTGraph(size, xs, ys);

    double[] ax = g.getX().getDoubleArray(g.getN());

    for( int i=0; i<size; i++ ) {
      System.out.printf("%f, %f\n", xs[i], as[i]);
    }
    
    TCanvas c = newTCanvas( "c", "c", 800, 600 );
    c.setGrid();
    
    g.draw();

    c.update();
    
    app.run();
    
    c.destroy();
    g.destroy();
  }

}
