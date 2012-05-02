#include "plugin.hpp"
//does not work
class Cone : public Projector {
protected:
  float rbase,eye;
  Projector* child;
public:
  void usage( int argc, char* argv[] )
  {
    fprintf( stderr, "Usage: %s [-a x] [projectors]\n", argv[0] );
    fprintf( stderr, "Convert Equirectangular 2:1 image to a cone.\n" );
    fprintf( stderr, "Options:\n" );
    fprintf( stderr, "\t-e 0.0\tSpecify eye height from the base.\n" ); 
    fprintf( stderr, "\t-r 0.3\tSpecify radius of the base.\n" ); 
    exit(1);
  }
  Cone(int argc, char* argv[])
  {
    float rbase=0.3;
    float eye = 0.0;
    int c = 1;
    while ( c < argc ){
      if ( 0 == strcmp( argv[c], "-r" )){
	c++;
	rbase = atof( argv[c] );
	c++;
      }
      else if ( 0 == strcmp( argv[c], "-e" )){
	c++;
	eye = atof( argv[c] );
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
    //fprintf( stderr, "%d\n", argc );
    child = plugin_load( argc, argv );
  }
  int map(float dstx, float dsty, float& srcx, float& srcy) const
  {
    float h,v; // horizontal and vertical angles
    
    float rcone = 1.0 - rbase;
    float height = sqrt(rcone*rcone-rbase*rbase);
    float comz = height*eye;
    //cone
    float cy = -1.0 + rcone;
    float cx = 0;
    float x = dstx - cx;
    float y = dsty - cy;
    float rr = x*x + y*y;
    if ( rr < rcone*rcone ){
      rr = sqrt(rr);
      //inside cone
      //angle on the developed cone, zero is vertical
      float angle = atan(x/y);
      if ( y < 0 ){
	angle += M_PI;
      }
      float r = rbase*rr/rcone;
      float h = height*(rcone-rr)/rcone;
      h -= comz;
      float theta = atan(h/r);
      float phi = angle / rbase * rcone;
      if ( phi < - M_PI || M_PI < phi ){
	return 0;
      }
      dstx = phi / M_PI;
      dsty = -theta / M_PI;
    }
    else{
      //nadir;
      cy = 1.0 - rbase;
      cx = 0;
      x = dstx - cx;
      y = dsty - cy;
      rr = x*x + y*y;
      if ( rr < rbase*rbase ){
	rr = sqrt(rr);
	//angle on the base, zero is vertical
	float angle = atan(x/-y);
	if ( y >= 0 ){
	  angle += M_PI;
	}
	h = -comz;
	float theta = atan(h/rr);
	float phi = angle;
	dstx = phi / M_PI;
	dsty = -theta / M_PI;
      }
      else{
	return 0;
      }
    }
    return child->map( dstx, dsty, srcx, srcy );
  }
};


// the class factories
extern "C" Projector* create(int argc, char* argv[]) {
  return new Cone(argc, argv);
}

extern "C" void destroy(Cone* p) {
    delete p;
}
