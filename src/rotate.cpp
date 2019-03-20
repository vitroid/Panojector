#include "plugin.hpp"

class Rotate : public Projector {
protected:
  float th;
  Projector* child;
public:
  void usage( int argc, char* argv[] )
  {
    fprintf( stderr, "Usage: %s [-a x] [projectors]\n", argv[0] );
    fprintf( stderr, "Rotate image.\n" );
    fprintf( stderr, "Options:\n" );
    fprintf( stderr, "\t-a 0.0\tSpecify rotation angle in degree.\n" ); 
    exit(1);
  }
  Rotate(int argc, char* argv[])
  {
    th = 0.0;

    int c = 1;
    while ( c < argc ){
      if ( 0 == strcmp( argv[c], "-a" )){
	c++;
	th = atof( argv[c] );
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
    th *= M_PI / 180.0;
    //fprintf( stderr, "%d\n", argc );
    child = plugin_load( argc, argv );
  }
  Vec3b map(complex<float> dst)
  {
    return child->map( dst*complex<float>(cos(th),-sin(th)));
  }
};


// the class factories
extern "C" Projector* create(int argc, char* argv[]) {
  return new Rotate(argc, argv);
}

extern "C" void destroy(Rotate* p) {
    delete p;
}
