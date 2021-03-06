#include "plugin.hpp"
#include <complex>

class Invert : public Projector {
protected:
  float A;
  Projector* child;
public:
  void usage( int argc, char* argv[] )
  {
    fprintf( stderr, "Usage: %s [-a x] [projectors]\n", argv[0] );
    fprintf( stderr, "Options:\n" );
    fprintf( stderr, "\t-a 1.0\tSpecify scale factor.\n" ); 
    exit(1);
  }
  Invert(int argc, char* argv[])
  {
    A=1;

    int c = 1;
    while ( c < argc ){
      if ( 0 == strcmp( argv[c], "-a" )){
	c++;
	A = atof( argv[c] );
	c++;
      }
      else if ( argv[c][0] == '-' ){
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
    complex<float> dst(dstx/A,dsty/A);
    complex<float> one(1.0,0.0);
    complex<float> src = one / dst;
    return child->map( src.real(), src.imag());
  }
};


// the class factories
extern "C" Projector* create(int argc, char* argv[]) {
  return new Invert(argc, argv);
}

extern "C" void destroy(Invert* p) {
    delete p;
}
