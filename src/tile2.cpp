#include "plugin.hpp"
#include <cstring>
//commensurate tiling
class Tile : public Projector {
protected:
  float aspect;
  float L;
  float h;
  float offset;
  float e1x,e1y,e2x,e2y;
  Projector* child;
public:
  void usage( int argc, char* argv[] )
  {
    fprintf( stderr, "Usage: %s [-a x] [projectors]\n", argv[0] );
    fprintf( stderr, "Make a commensurate slanted tile of a long image.\n" );
    fprintf( stderr, "Options:\n" );
    fprintf( stderr, "\t-a 0.5\tSpecify aspect ratio of the original image.\n" ); 
    exit(1);
  }
  Tile(int argc, char* argv[])
  {
    aspect = 0.5;

    int c = 1;
    while ( c < argc ){
      if ( 0 == strcmp( argv[c], "-a" )){
	c++;
	if ( index( argv[c], '/' ) ){
	  float num,den;
	  sscanf(argv[c], "%f/%f", &num,&den);
	  aspect = num/den;
	}
	else
	  aspect = atof( argv[c] );
	c++;
      }
      else if ( argv[c][0] == '-' ){
	usage(argc, argv);
      }
      else{
	break;
      }
    }
    float bw = 2*aspect;
    L = sqrt(2.0*2.0 + bw*bw); // a period of the slant image
    h = 2.0*bw / L;            // height of the slant image
    offset = (L-sqrt(L*L-4*h*h))/2.0;
    //printf("%f %f %f\n", L,h,offset);
    e1x = 2.0/L;
    e1y = bw/L;
    e2x = -bw/L;
    e2y = 2.0/L;

    argv += c;
    argc -= c;
    //fprintf( stderr, "%d\n", argc );
    child = plugin_load( argc, argv );
  }
  uchar* map(float dstx, float dsty)
  {
    float sx = dstx*e1x + dsty*e1y;
    float sy = dstx*e2x + dsty*e2y;
    float dy = floor( sy / h + 0.5 );
    sy -= h*dy;
    sx += (L-offset)*dy;
    sx = sx / h * aspect * 2.0;
    sy = sy / h * aspect * 2.0;
    sx -= floor( sx/2.0 + 0.5 )*2.0;
    return child->map( sx, sy );
  }
};


// the class factories
extern "C" Projector* create(int argc, char* argv[]) {
  return new Tile(argc, argv);
}

extern "C" void destroy(Tile* p) {
    delete p;
}
