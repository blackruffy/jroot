import net.blackruffy.root.*;
import static net.blackruffy.root.JRoot.*;
import static net.blackruffy.root.Pointer.*;

public class DrawColors {

  public static void main( String[] args ) {
    TApplication app = newTApplication( "app", newInt(0), new String[]{} );
    TCanvas c = newTCanvas();
    c.divide(10, 10);
    for( int i=0; i<100; i++ ) {
      c.cd(i+1);
      gPad().setFillColor(i+1);
      gPad().draw();
      TText text = newTText(0.1, 0.1, Integer.toString(i+1));
      text.setTextSize(0.7f);
      if( i == 0 ) text.setTextColor(10);
      text.draw();
      gPad().update();
    }
    c.update();
    app.run();
  }

}
