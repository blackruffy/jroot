
void* TApplication_new( char *appname );
void TApplication_run( void *self, int retrn );
void* TCanvas_new();
void TCanvas_draw( void *self );

int main( int argc, char **argv ) {

  void* app = TApplication_new("app");
  TCanvas_draw( TCanvas_new() );
  TApplication_run( app, 0 );
  
  return 0;
}
