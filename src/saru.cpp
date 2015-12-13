#include "plugin.hpp"

int sarux = 16;
int saruy = 16;
int saru[]={0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,
            1,1,0,0,0,0,1,1,1,0,0,0,0,1,0,0,
            1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
            1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
            1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,
            1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,
            1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
            1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
            1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,
            1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,
            1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
            1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
            1,1,0,0,0,0,1,1,1,0,0,0,1,1,0,0,
            0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,
            0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,};
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
    int depth = 5;
    for(int i=0; i<depth; i++){
        float vx = (x+1) * (sarux/2) - 0.5;
        float vy = (y+1) * (saruy/2) - 0.5;
        float fx = floor(vx);
        float fy = floor(vy);
        int sx = fx;
        int sy = fy;
        sx %= sarux;
        if (sx < 0 ) sx += sarux;
        sy %= sarux;
        if (sy < 0 ) sy += saruy;
        if ( saru[sx+sy*sarux] == 0){
            return White;
        }
        x = vx - fx; // 0.0 .. 1.0
        x = x*2 - 1; //-1.0 .. 1.0
        y = vy - fy; // 0.0 .. 1.0
        y = y*2 - 1; //-1.0 .. 1.0
    }
    return Black;
  }
};


// the class factories
extern "C" Projector* create(int argc, char* argv[]) {
  return new Load(argc, argv);
}

extern "C" void destroy(Load* p) {
    delete p;
}
