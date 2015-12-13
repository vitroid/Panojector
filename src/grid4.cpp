#include "plugin.hpp"

int gridx = 1000;
int gridy = 1000;
int thick = 4;
int thin  = 2;

uchar White[] = {255,255,255,};
uchar Black[] = {0,0,0,};

class Load : public Projector {
//protected:
//  IplImage* src;
public:
  void usage( int argc, char* argv[] )
  {
    fprintf( stderr, "Usage: %s\n", argv[0] );
    fprintf( stderr, "Options:\n" );
    exit(1);
  }
  Load(int argc, char* argv[])
  {
    //src = cvLoadImage( argv[1], CV_LOAD_IMAGE_COLOR );
    //fprintf( stderr, "%s\n", argv[1] );
  }
  uchar* map(float x, float y)
  {
        float vx = (x+1) * (gridx/2);
        float vy = (y+1) * (gridy/2);
        float fx = floor(vx);
        float fy = floor(vy);
        int sx = fx;
        int sy = fy;
        sx %= gridx;
        if (sx < 0 ) sx += gridx;
        sy %= gridy;
        if (sy < 0 ) sy += gridy;
        int b = 0;
        for ( int i = 0; i <= gridx; i += gridx / 8 ){
            if ( ( i-thin/2 <= sx ) && ( sx < i+thin/2 ) ){
                b = 1;
            }
        }
        for ( int i = 0; i <= gridy; i += gridy / 8 ){
            if ( ( i-thin/2 <= sy ) && ( sy < i+thin/2 ) ){
                b = 1;
            }
        }
        for ( int i = 0; i <= gridx; i += gridx / 2 ){
            if ( ( i-thick/2 <= sx ) && ( sx < i+thick/2 ) ){
                b = 1;
            }
        }
        for ( int i = 0; i <= gridy; i += gridy / 2 ){
            if ( ( i-thick/2 <= sy ) && ( sy < i+thick/2 ) ){
                b = 1;
            }
        }
        if ( b == 1 ){    
            return Black;
        }
        return White;
  }
};


// the class factories
extern "C" Projector* create(int argc, char* argv[]) {
  return new Load(argc, argv);
}

extern "C" void destroy(Load* p) {
    delete p;
}
