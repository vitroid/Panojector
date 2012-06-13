#include "plugin.hpp"

class Power : public Projector {
protected:
  float A,n;
  Projector* child;
public:
  void usage( int argc, char* argv[] )
  {
    fprintf( stderr, "Usage: %s [-a x][-n x] [projectors]\n", argv[0] );
    fprintf( stderr, "Options:\n" );
    fprintf( stderr, "\t-a 1.0\tSpecify scale factor.\n" ); 
    fprintf( stderr, "\t-n 2.0\tSpecify power factor.\n" ); 
    exit(1);
  }
  Power(int argc, char* argv[])
  {
    A=1;
    n = 2;

    int c = 1;
    while ( c < argc ){
      if ( 0 == strcmp( argv[c], "-a" )){
	c++;
	A = atof( argv[c] );
	c++;
      }
      else if ( 0 == strcmp( argv[c], "-n" )){
	c++;
	n = atof( argv[c] );
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
    complex<float> src = exp(n*log(dst)); //double the image
    return child->map( src.real(), src.imag() );
  }
};


// the class factories
extern "C" Projector* create(int argc, char* argv[]) {
  return new Power(argc, argv);
}

extern "C" void destroy(Power* p) {
    delete p;
}
