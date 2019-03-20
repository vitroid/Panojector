#include "plugin.hpp"

class Slide : public Projector {
protected:
  complex<float> displace;
  Projector* child;
public:
  void usage( int argc, char* argv[] )
  {
    fprintf( stderr, "Usage: %s [-x x][-y y] [projectors]\n", argv[0] );
    fprintf( stderr, "Options:\n" );
    fprintf( stderr, "\t-x 0.0\tSpecify slide to the left.\n" ); 
    fprintf( stderr, "\t-y 0.0\tSpecify slide below.\n" ); 
    exit(1);
  }
  Slide(int argc, char* argv[])
  {
    float x=0;
    float y=0;

    int c = 1;
    while ( c < argc ){
      if ( 0 == strcmp( argv[c], "-x" )){
	c++;
	x = atof( argv[c] );
	c++;
      }
      else if ( 0 == strcmp( argv[c], "-y" )){
	c++;
	y = atof( argv[c] );
	c++;
      }
      else if ( argv[c][0] == '-' ){
	usage(argc, argv);
      }
      else{
	break;
      }
    }
    displace= complex<float>(x,y);
    argv += c;
    argc -= c;
    //fprintf( stderr, "%d\n", argc );
    child = plugin_load( argc, argv );
  }
  Vec3b map( complex<float> dst )
  {
    return child->map(dst+displace);
  }
};


// the class factories
extern "C" Projector* create(int argc, char* argv[]) {
  return new Slide(argc, argv);
}

extern "C" void destroy(Slide* p) {
    delete p;
}
