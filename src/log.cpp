#include "plugin.hpp"

class Logg : public Projector {
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
  Logg(int argc, char* argv[])
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
    complex<float> pi(M_PI,0.0);
    //complex<float> eye(0,1);//makes source vertical
    complex<float> src = log(dst); //   / pi; // * eye;
    return child->map( src.real(), src.imag() );
  }
};


// the class factories
extern "C" Projector* create(int argc, char* argv[]) {
  return new Logg(argc, argv);
}

extern "C" void destroy(Logg* p) {
    delete p;
}
