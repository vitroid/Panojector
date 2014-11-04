#include "plugin.hpp"

class Load : public Projector {
protected:
  IplImage* src;
public:
  void usage( int argc, char* argv[] )
  {
    fprintf( stderr, "Usage: %s filename\n", argv[0] );
    fprintf( stderr, "Options:\n" );
    exit(1);
  }
  Load(int argc, char* argv[])
  {
    src = cvLoadImage( argv[1], CV_LOAD_IMAGE_COLOR );
    fprintf( stderr, "%s\n", argv[1] );
  }
  uchar* map(float x, float y)
  {
    int sx = x * (float)src->width / 2.0 + src->width / 2.0;
    int sy = y * (float)src->width / 2.0 + src->height / 2.0;
    sx %= src->width;
    if ( sx < 0 )
      sx += src->width;
    sy %= src->height;
    if ( sy < 0 )
      sy += src->height;
    return (uchar*)(src->imageData + src->widthStep*sy + sx*src->nChannels);
  }
};


// the class factories
extern "C" Projector* create(int argc, char* argv[]) {
  return new Load(argc, argv);
}

extern "C" void destroy(Load* p) {
    delete p;
}
