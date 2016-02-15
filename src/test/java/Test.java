;import com.sun.jna.*;
import com.sun.jna.ptr.*;
import net.blackruffy.root.*;

public class Test {

  public static void main( String[] args ) {
    CRoot croot = CRoot.INSTANCE;
    Pointer app = croot.TApplication_newTApplication_1("app", new IntByReference(0).getPointer(), new String[]{});
    //Pointer app = croot.Global_gApplication();
    Pointer canvas = croot.TCanvas_newTCanvas_6();
    //croot.TCanvas_draw_1( canvas );
    croot.TApplication_run_1( app );
  }

}
