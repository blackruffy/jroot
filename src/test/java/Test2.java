import com.sun.jna.*;
import com.sun.jna.ptr.*;
import net.blackruffy.root.*;
import static net.blackruffy.root.JRoot.*;

public class Test2 {

  public static void main( String[] args ) {
    //TH1F h = TH1F.newTH1F("", "h", 100, 0, 100);
    TApplication app = newTApplication(
      "app", new IntByReference(0).getPointer(), new String[]{}
    );
    TCanvas c = newTCanvas();
    app.run();
  }

}
