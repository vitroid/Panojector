#include "plugin.hpp"

class Prism : public Projector {
protected:
  float height,eye;
  int nface,zenith,nadir;
  Projector* child;
public:
  void usage( int argc, char* argv[] )
  {
    fprintf( stderr, "Usage: %s [-e x][-h y][-s x][-z x][-n x] [projectors]\n", argv[0] );
    fprintf( stderr, "Convert equirectangular 2:1 image to a prism.\n" );
    fprintf( stderr, "Options:\n" );
    fprintf( stderr, "\t-e 0.5\tSpecify eye point from the center of prism.\n" ); 
    fprintf( stderr, "\t-h 0.5\tSpecify height.\n" ); 
    fprintf( stderr, "\t-s 6\tSpecify number of side faces.\n" ); 
    fprintf( stderr, "\t-z 2\tSpecify where to attach the zenith cap.\n" ); 
    fprintf( stderr, "\t-n 2\tSpecify where to attach the nadir cap.\n" ); 
    exit(1);
  }
  Prism(int argc, char* argv[])
  {
    height = 0.5;
    nface = 6;
    zenith = -1;
    nadir = -1;
    eye = 0;

    int c = 1;
    while ( c < argc ){
      if ( 0 == strcmp( argv[c], "-e" )){
	c++;
	eye = atof( argv[c] );
	c++;
      }
      else if ( 0 == strcmp( argv[c], "-s" )){
	c++;
	nface = atoi( argv[c] );
	c++;
      }
      else if ( 0 == strcmp( argv[c], "-h" )){
	c++;
	height = atoi( argv[c] );
	c++;
      }
      else if ( 0 == strcmp( argv[c], "-z" )){
	c++;
	zenith = atoi( argv[c] );
	c++;
      }
      else if ( 0 == strcmp( argv[c], "-n" )){
	c++;
	nadir = atoi( argv[c] );
	c++;
      }
      else if ( argv[c][0] == '-' ){
	usage(argc, argv);
      }
      else{
	break;
      }
    }
    if ( zenith < 0 ){
      zenith = (nface-1)/2;
    }
    if ( nadir < 0 ){
      nadir = zenith;
    }
    argv += c;
    argc -= c;
    //fprintf( stderr, "%d\n", argc );
    child = plugin_load( argc, argv );
  }
  uchar* map(float dstx, float dsty)
  {
    float h,v; // horizontal and vertical angles
    //radius of the cylinder
    float r = 1 / M_PI;
    float halfa = M_PI / nface;
    float rs = r * cos(halfa);
    float halfw = r*sin(halfa);
    float theta, phi;
    //nadir
    float cx = halfw*(-nface + 2*nadir + 1);
    float cy = height/2+rs;
    float x = dstx - cx;
    float y = dsty - cy;
    float rr = sqrt(x*x + y*y);
    phi = atan(y/x) + M_PI/2 + (nadir*2-nface+1)*halfa;
    if ( x < 0 ){
      phi += M_PI;
    }
    float modulo = phi + M_PI - floor((phi + M_PI) / (halfa*2))*(halfa*2) - halfa; //phi - floor( 0.5 + phi / (2*M_PI) * nface) * (2*M_PI) / nface;
    float rm = rs / cos(modulo);
    if ( rr < rm ){
      theta = atan( (eye+height/2) / rr );
      dstx = phi / M_PI;
      dsty = theta / M_PI;
    }
    else{
      //zenith
      float cx = halfw*(-nface + 2*zenith + 1);
      float cy = -height/2-rs;
      float x = dstx - cx;
      float y = dsty - cy;
      float rr = sqrt(x*x + y*y);
      phi = -atan(y/x) + M_PI/2 + (zenith*2-nface+1)*halfa;
      if ( x < 0 ){
	phi -= M_PI;
      }
      float modulo = phi + M_PI - floor((phi + M_PI) / (halfa*2))*(halfa*2) - halfa; //phi - floor( 0.5 + phi / (2*M_PI) * nface) * (2*M_PI) / nface;
      float rm = rs / cos(modulo);
      if ( rr < rm ){
	theta = atan( (eye-height/2) / rr );
	dstx = phi / M_PI;
	dsty = theta / M_PI;
      }
      else{
	if ( ( -height / 2 <= dsty ) && ( dsty <= height / 2 ) ){
	  //sides
	  float x = dstx + nface*halfw;  // coord from left end
	  float face = floor(x / (halfw*2));
	  if ( ( 0 <= face ) && ( face < nface ) ){
	    modulo = x - (face*2+1)*halfw;
	    phi = atan( modulo / rs );
	    rm = sqrt(modulo*modulo+rs*rs);
	    phi += (2*M_PI/nface)*(face+0.5)-M_PI;
	    theta = atan( (eye+dsty) / rm );
	    dstx = phi / M_PI;
	    dsty = theta / M_PI;
	  }
	  else{
	    return 0;
	  }
	}
	else{
	  return 0;
	}
      }
    }
    return child->map(dstx,dsty);
  }
};


// the class factories
extern "C" Projector* create(int argc, char* argv[]) {
  return new Prism(argc, argv);
}

extern "C" void destroy(Prism* p) {
    delete p;
}
