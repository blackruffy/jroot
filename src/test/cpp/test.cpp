#include <TApplication.h>
#include <TCanvas.h>

using namespace std;

extern "C" {

  void* TApplication_new( const char *appName, int *argc, char **argv ) {
    return new TApplication( appName, argc, argv );
  }

  void TApplication_createApplication() {
    return TApplication::CreateApplication();
  }
  
  void TApplication_run( void *self, int retrn ) {
    ((TApplication*)self)->Run( retrn );
  }

  void* Global_gApplication() {
    return gApplication;
  }
  
  void* TCanvas_new() {
    return new TCanvas();
  }

  void TCanvas_draw( void *self ) {
    ((TCanvas*)self)->Draw();
  }
  
}
