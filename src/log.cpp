#include "plugin.hpp"

class Logg : public Projector {
protected:
  Projector* child;
public:
  void usage( int argc, char* argv[] )
  {
    fprintf( stderr, "Usage: %s [projectors]\n", argv[0] );
    exit(1);
  }
  Logg(int argc, char* argv[])
  {
    int c = 1;
    argv += c;
    argc -= c;
    //fprintf( stderr, "%d\n", argc );
    child = plugin_load( argc, argv );
  }
  Vec3b map(complex<float> dst)
  {
    return child->map( log(dst) );
  }
};


// the class factories
extern "C" Projector* create(int argc, char* argv[]) {
  return new Logg(argc, argv);
}

extern "C" void destroy(Logg* p) {
    delete p;
}
