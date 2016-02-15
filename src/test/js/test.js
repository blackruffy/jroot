var ffi = require('ffi');
var ref = require('ref');

var croot = ffi.Library('libCRoot', {
  'TCanvas_newTCanvas_6': [ 'void', [] ],
  // void* TApplication_newTApplication_1(const char* appClassName, int* argc, char** argv);
  'TApplication_newTApplication_1': [ 'pointer', [ 'string', 'pointer', 'pointer' ]],
  // void TApplication_run_1(void* self);
  'TApplication_run_1': [ 'void', ['pointer']],
  // TH1F_newTH1F_2(const char* name, const char* title, int nbinsx, double xlow, double xup);
  'TH1F_newTH1F_2': ['pointer', ['string', 'string', 'int', 'double', 'double']],
  // void TH1F_fillRandom_1(void* self, const char* fname);
  'TH1F_fillRandom_1': ['void', ['pointer', 'string']],
  // void TH1F_draw_1(void* self);
  'TH1F_draw_1': ['void', ['pointer']]
});

var app = croot.TApplication_newTApplication_1( 'app', null, null );

croot.TCanvas_newTCanvas_6();

var h = croot.TH1F_newTH1F_2('h', 'h', 100, -5, 5);

croot.TH1F_fillRandom_1( h, 'gaus');

croot.TH1F_draw_1( h );

croot.TApplication_run_1( app );
