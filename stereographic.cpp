#include "plugin.hpp"

class Stereographic : public Projector {
protected:
  float lmax;
  Projector* child;
public:
  void usage( int argc, char* argv[] )
  {
    fprintf( stderr, "Usage: %s [-a x] [projectors]\n", argv[0] );
    fprintf( stderr, "Convert Equirectangular 2:1 image to steregraphic image.\n" );
    fprintf( stderr, "Options:\n" );
    fprintf( stderr, "\t-a 90.0\tSpecify field of view.\n" ); 
    exit(1);
  }
  Stereographic(int argc, char* argv[])
  {
    float fov = 90.0;
    int c = 1;
    while ( c < argc ){
      if ( 0 == strcmp( argv[c], "-a" )){
	c++;
	fov = atof( argv[c] );
	c++;
      }
      else if ( argv[c][0] == '-' ){
	usage(argc, argv);
      }
      else{
	break;
      }
    }
    lmax = tan(fov *M_PI / 180.0 / 4.0); 
    argv += c;
    argc -= c;
    //fprintf( stderr, "%d\n", argc );
    child = plugin_load( argc, argv );
  }
  int map(float dstx, float dsty, float& srcx, float& srcy) const
  {
    float L = 2.0*lmax*sqrt(dstx*dstx + dsty*dsty);
    float theta = M_PI/2.0 - 2.0 * atan( L / 2.0 );
    float phi;
    if ( dstx == 0 ){
      phi = M_PI / 2.0;
      if ( dsty < 0.0 ){
	phi += M_PI;
      }
    }
    else{
      phi = atan( dsty / dstx );
    }
    if ( dstx < 0.0 ){
      phi += M_PI;
    }
    phi -= floor( phi / (2.0*M_PI) + 0.5 ) * (2.0*M_PI);
    theta /= M_PI;
    phi   /= M_PI;
    return child->map( phi, theta, srcx, srcy );
  }
};


// the class factories
extern "C" Projector* create(int argc, char* argv[]) {
  return new Stereographic(argc, argv);
}

extern "C" void destroy(Stereographic* p) {
    delete p;
}
