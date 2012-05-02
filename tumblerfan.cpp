#include "plugin.hpp"

class TumblerFan : public Projector {
protected:
  float r0, alphamax, scal,height;
  Projector* child;
public:
  void usage( int argc, char* argv[] )
  {
    fprintf( stderr, "Usage: %s [-s wtop,wbot,height] [projectors]\n", argv[0] );
    fprintf( stderr, "Mercator to Starbucks Tumbler image.\n" );
    fprintf( stderr, "Options:\n" );
    fprintf( stderr, "\t-s 214,173,163\tDimensions of the fan in mm.\n" ); 
    exit(1);
  }
  TumblerFan(int argc, char* argv[])
  {
    float wtop, wbot;
    wtop = 214.0;
    wbot = 173.0;
    height = 163.0;

    int c = 1;
    while ( c < argc ){
      if ( 0 == strcmp( argv[c], "-s" )){
	c++;
	sscanf(argv[c],"%f,%f,%f", &wtop,&wbot,&height);
	c++;
      }
      else if ( argv[c][0] == '-' ){
	usage(argc, argv);
      }
      else{
	break;
      }
    }
    argv += c;
    argc -= c;

    r0 = wbot/(wtop-wbot)*height;
    alphamax = asin(wbot/2.0 / r0);
    scal = 2 / (r0*2.0*alphamax);
    //fprintf( stderr, "%d\n", argc );
    child = plugin_load( argc, argv );
  }
  int map(float dstx, float dsty, float& srcx, float& srcy) const
  {
    float radius = sqrt(dstx*dstx + (dsty+r0)*(dsty+r0));
    float alpha  = atan(dstx/(dsty+r0));
    if ( abs(alpha) < alphamax && radius > r0 && radius < (r0 + height) ){
      return child->map( -r0*alpha*scal, -(r0*log(radius/r0)*scal), srcx,srcy );
    }
    else{
      return 0;
    }
  }
};


// the class factories
extern "C" Projector* create(int argc, char* argv[]) {
  return new TumblerFan(argc, argv);
}

extern "C" void destroy(TumblerFan* p) {
    delete p;
}
