#include "plugin.hpp"
#include <complex>

class Invert : public Projector {
protected:
  Projector* child;
public:
  void usage( int argc, char* argv[] )
  {
    fprintf( stderr, "Usage: %s [projectors]\n", argv[0] );
    exit(1);
  }
  Invert(int argc, char* argv[])
  {
    int c = 1;
    argv += c;
    argc -= c;
    //fprintf( stderr, "%d\n", argc );
    child = plugin_load( argc, argv );
  }
  Vec3b map(complex<float> dst)
  {
    return child->map( complex<float>(1.0) / dst );
  }
};


// the class factories
extern "C" Projector* create(int argc, char* argv[]) {
  return new Invert(argc, argv);
}

extern "C" void destroy(Invert* p) {
    delete p;
}
