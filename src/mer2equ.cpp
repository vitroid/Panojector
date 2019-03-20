#include "plugin.hpp"

class Equirectangular : public Projector {
protected:
  float x,y;
  Projector* child;
public:
  void usage( int argc, char* argv[] )
  {
    fprintf( stderr, "Usage: %s [projectors]\n", argv[0] );
    fprintf( stderr, "Convert Mercator image into Equirectangular.\n" );
    exit(1);
  }
  Equirectangular(int argc, char* argv[])
  {
    int c = 1;
    argv += c;
    argc -= c;
    //fprintf( stderr, "%d\n", argc );
    child = plugin_load( argc, argv );
  }
  Vec3b map(complex<float> dst)
  {
    float theta = dst.imag() * M_PI;
    float tx    = dst.real();
    float ty    = log( tan(M_PI/4.0 + theta/2.0) ) / M_PI;
    return child->map(complex<float>(tx,ty));
  }
};


// the class factories
extern "C" Projector* create(int argc, char* argv[]) {
  return new Equirectangular(argc, argv);
}

extern "C" void destroy(Equirectangular* p) {
    delete p;
}
