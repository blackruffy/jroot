import net.blackruffy.root.*;
import static net.blackruffy.root.JRoot.*;
import static net.blackruffy.root.Pointer.*;
import static java.lang.System.*;
import static java.lang.String.format;

public class DrawTF1 {

  public static void main( String[] args ) {

    TApplication app = newTApplication( "app", newInt(0), new String[]{} );
    
    TCanvas c = newTCanvas( "c", "c", 800, 600 );

    TF1 f = newTF1("f", "sin(x)", 0.0, Math.PI*2);
    f.draw();

    c.update();
    
    app.run();

  }

}
