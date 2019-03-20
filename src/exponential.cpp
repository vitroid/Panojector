#include "plugin.hpp"

class Exponential : public Projector {
protected:
  Projector* child;
public:
  void usage( int argc, char* argv[] )
  {
    fprintf( stderr, "Usage: %s [projectors]\n", argv[0] );
    exit(1);
  }
  Exponential(int argc, char* argv[])
  {
    int c = 1;
    argv += c;
    argc -= c;
    child = plugin_load( argc, argv );
  }
  Vec3b map(complex<float> dst)
  {
    return child->map( exp(dst) );
  }
};


// the class factories
extern "C" Projector* create(int argc, char* argv[]) {
  return new Exponential(argc, argv);
}

extern "C" void destroy(Exponential* p) {
    delete p;
}
