#include "plugin.hpp"

class Swap : public Projector {
protected:
  int nswap;
  Projector* child;
public:
  void usage( int argc, char* argv[] )
  {
    fprintf( stderr, "Usage: %s [-n x] [projectors]\n", argv[0] );
    fprintf( stderr, "Rotate xyz axes in the equirectangular image.\n" );
    fprintf( stderr, "Options:\n" );
    fprintf( stderr, "\t-n 1\tDo multiple swaps.\n" ); 
    exit(1);
  }
  Swap(int argc, char* argv[])
  {
    nswap = 1;

    int c = 1;
    while ( c < argc ){
      if ( 0 == strcmp( argv[c], "-n" )){
	c++;
	nswap = atoi( argv[c] );
	c++;
      }
      else if ( argv[c][0] == '-' ){
	usage(argc, argv);
      }
      else{
	break;
      }
    }
    nswap %= 3;
    if (nswap < 0)
      nswap += 3;

    argv += c;
    argc -= c;
    //fprintf( stderr, "%d\n", argc );
    child = plugin_load( argc, argv );
  }
  Vec3b map(complex<float> dst)
  {
    float dstx = dst.real();
    float dsty = dst.imag();
    if ( dsty > 0.5 || dsty < -0.5 ){
      return 0;
    }
    //map coordinate to equirectangular coordinate
    float phi   = dstx * M_PI;
    float theta = dsty * M_PI;
    float x,y,z;
    spherical2cartesian(phi, theta, x,y,z );
    //swap
    float t;
    for(int i=0; i<nswap; i++){
      t = z;    z = -y;    y = x;    x = -t;
    }
    //recover angles
    cartesian2spherical(x,y,z, phi, theta);
    return child->map( complex<float>(phi / M_PI, theta / M_PI) );
  }
};


// the class factories
extern "C" Projector* create(int argc, char* argv[]) {
  return new Swap(argc, argv);
}

extern "C" void destroy(Swap* p) {
    delete p;
}
