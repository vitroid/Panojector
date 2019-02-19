#include "plugin.hpp"

class Equ2Cyl : public Projector {
protected:
  float x,y;
  Projector* child;
public:
  void usage( int argc, char* argv[] )
  {
    fprintf( stderr, "Usage: %s [projectors]\n", argv[0] );
    fprintf( stderr, "Convert equirectangular image into cylindrical image.\n" );
    exit(1);
  }
  Equ2Cyl(int argc, char* argv[])
  {
    int c = 1;
    while ( c < argc ){
      if ( argv[c][0] == '-' ){
	usage(argc, argv);
      }
      else{
	break;
      }
    }
    argv += c;
    argc -= c;
    //fprintf( stderr, "%d\n", argc );
    child = plugin_load( argc, argv );
  }
  uchar* map(float dstx, float dsty)
  {
    //dstx : -0.5 .. +0.5
    //dsty : -1 .. +1
    float tx,ty;
    float theta = dsty * M_PI;
    tx    = dstx;
    ty    = atan(theta) / M_PI;
    return child->map(tx,ty);
  }
};


// the class factories
extern "C" Projector* create(int argc, char* argv[]) {
  return new Equ2Cyl(argc, argv);
}

extern "C" void destroy(Equ2Cyl* p) {
    delete p;
}
