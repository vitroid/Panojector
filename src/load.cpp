#include "plugin.hpp"


class Load : public Projector {
protected:
  Mat src;
public:
  void usage( int argc, char* argv[] )
  {
    fprintf( stderr, "Usage: %s filename\n", argv[0] );
    fprintf( stderr, "Options:\n" );
    exit(1);
  }
  Load(int argc, char* argv[])
  {
    src = imread( argv[1] );
    fprintf( stderr, "%s\n", argv[1] );
    fprintf( stderr, "Size: %d/%d\n", src.rows, src.cols );
  }
  Vec3b map(complex<float> c)
  {
    int sx = c.real() * (float)src.cols / 2.0 + src.cols / 2.0;
    int sy = c.imag() * (float)src.cols / 2.0 + src.rows / 2.0;
    sx %= src.cols;
    if ( sx < 0 )
      sx += src.cols;
    sy %= src.rows;
    if ( sy < 0 )
      sy += src.rows;
    return src.at<Vec3b>(sy,sx);
  }
};


// the class factories
extern "C" Projector* create(int argc, char* argv[]) {
  return new Load(argc, argv);
}

extern "C" void destroy(Load* p) {
    delete p;
}
