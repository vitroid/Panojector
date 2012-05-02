#include "plugin.hpp"

class Mercator : public Projector {
protected:
  float x,y;
  Projector* child;
public:
  void usage( int argc, char* argv[] )
  {
    fprintf( stderr, "Usage: %s [projectors]\n", argv[0] );
    fprintf( stderr, "Convert equirectangular 2:1 image into Mercator.\n" );
    exit(1);
  }
  Mercator(int argc, char* argv[])
  {
    int c = 1;
    argv += c;
    argc -= c;
    //fprintf( stderr, "%d\n", argc );
    child = plugin_load( argc, argv );
  }
  int map(float dstx, float dsty, float& srcx, float& srcy) const
  {
    float tx    = dstx;
    float ty    = 2.0*atan(exp(dsty*M_PI))/M_PI - 0.5;
    return child->map(tx,ty,srcx,srcy);
  }
};


// the class factories
extern "C" Projector* create(int argc, char* argv[]) {
  return new Mercator(argc, argv);
}

extern "C" void destroy(Mercator* p) {
    delete p;
}
