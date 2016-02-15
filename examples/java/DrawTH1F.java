import net.blackruffy.root.*;
import static net.blackruffy.root.JRoot.*;

public class DrawTH1F {

  public static void main( String[] args ) {

    TApplication app = newTApplication( "app", Pointer.newInt(0), new String[]{} );

    TH1F h = newTH1F( "h", "h", 100, -10., 10. );
    
    h.fillRandom("gaus", 10000);
    
    TCanvas c = newTCanvas( "c", "c", 800, 600 );
    gPad().setGrid();

    h.setFillColor(38);
    h.draw();

    c.update();
    
    app.run();
    
    c.destroy();
    h.destroy();
  }

}
