#include "plugin.hpp"
#include <cmath>

class Interpolate : public Projector {
protected:
  IplImage* src;
  uchar pixel[3];
public:
  void usage( int argc, char* argv[] )
  {
    fprintf( stderr, "Usage: %s filename\n", argv[0] );
    fprintf( stderr, "Options:\n" );
    exit(1);
  }
  Interpolate(int argc, char* argv[])
  {
    src = cvLoadImage( argv[1], CV_LOAD_IMAGE_COLOR );
    fprintf( stderr, "%s\n", argv[1] );
  }
  uchar* map(float x, float y)
  {
    float fx = x * src->width / 2.0 + src->width / 2.0;
    float fy = y * src->width / 2.0 + src->height / 2.0;
    int sx = floor(fx);
    int sy = floor(fy);
    float dx = fx - sx;
    float dy = fy - sy;
    sx %= src->width;
    if ( sx < 0 )
      sx += src->width;
    sy %= src->height;
    if ( sy < 0 )
      sy += src->height;
    int sxi = sx + 1;
    int syi = sy + 1;
    sxi %= src->width;
    syi %= src->height;
    int ws = src->widthStep;
    int nc = src->nChannels;
    for(int ch=0;ch<3;ch++){
      pixel[ch] = (1.0-dx)*((1.0-dy)*(uchar)(src->imageData[ws*sy  + sx*nc  + ch])+
			    dy      *(uchar)(src->imageData[ws*syi + sx*nc  + ch]))+
                  dx      *((1.0-dy)*(uchar)(src->imageData[ws*sy  + sxi*nc + ch])+
			    dy      *(uchar)(src->imageData[ws*syi + sxi*nc + ch]));
    }
    return pixel;
  }
};


// the class factories
extern "C" Projector* create(int argc, char* argv[]) {
  return new Interpolate(argc, argv);
}

extern "C" void destroy(Interpolate* p) {
    delete p;
}
