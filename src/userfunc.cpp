#include "plugin.hpp"

class Rotate : public Projector {
protected:
  float th;
  Projector* child;
public:
  void usage( int argc, char* argv[] )
  {
    fprintf( stderr, "Usage: %s [projectors]\n", argv[0] );
    fprintf( stderr, "Projection by user-defined function.\n" );
    exit(1);
  }
  Rotate(int argc, char* argv[])
  {
    th = 0.0;

    int c = 1;
    argv += c;
    argc -= c;
    child = plugin_load( argc, argv );
  }
  Vec3b map(complex<float> dst)
  {
     //modify it
    complex<float> src = dst * complex<float>(0.5, sqrt(3)/2.0);
    return child->map( src );
  }
};


// the class factories
extern "C" Projector* create(int argc, char* argv[]) {
  return new Rotate(argc, argv);
}

extern "C" void destroy(Rotate* p) {
    delete p;
}
