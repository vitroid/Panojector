#include "plugin.hpp"

class Equirectangular : public Projector {
protected:
  float x,y;
  Projector* child;
  int sides; // for prism
public:
  void usage( int argc, char* argv[] )
  {
    fprintf( stderr, "Usage: %s [projectors]\n", argv[0] );
    fprintf( stderr, "Convert cylindrical image into Equirectangular.\n" );
    exit(1);
  }
  Equirectangular(int argc, char* argv[])
  {
    sides = 0;
    int c = 1;
    while ( c < argc ){
      if ( 0 == strcmp( argv[c], "-s" )){
	c++;
	sides = atoi( argv[c] );
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
    //dstx : -0.5 .. +0.5
    //dsty : -1 .. +1
    float tx,ty;
    if ( sides == 0 ){
      float theta = dsty * M_PI;
      tx    = dstx;
      ty    = tan(theta) / M_PI;
    }
    else {
      float theta = dsty * M_PI;
      float phi   = dstx * M_PI; //-pi .. +pi
      float side  = floor( dstx * sides / 2 + 0.5 );//which side
      float side_angle = 2 * M_PI / sides;
      float phiB  = phi - side * side_angle;     //residual angle
      float r     = 1.0 / cos(phiB);
      float offs  = tan(phiB);
      float side_width = tan(M_PI/sides) * 2;
      tx    = (side_width*side + offs) / (side_width * sides) * 2;
      ty    = tan(theta)*r / M_PI;
    }
    return child->map(tx,ty);
  }
};


// the class factories
extern "C" Projector* create(int argc, char* argv[]) {
  return new Equirectangular(argc, argv);
}

extern "C" void destroy(Equirectangular* p) {
    delete p;
}
