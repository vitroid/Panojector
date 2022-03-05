#include "plugin.hpp"
#include <iostream>


class Tilt : public Projector {
protected:
  float a;
  Projector* child;
public:
  void usage( int argc, char* argv[] )
  {
    fprintf( stderr, "Usage: %s [-a x] [projectors]\n", argv[0] );
    fprintf( stderr, "Rotate equirectangular image around x axis.\n" );
    fprintf( stderr, "Options:\n" );
    fprintf( stderr, "\t-a 0.0\tSpecify tilt angle in degree.\n" ); 
    exit(1);
  }
  Tilt(int argc, char* argv[])
  {
    a = 0.0;

    int c = 1;
    while ( c < argc ){
      if ( 0 == strcmp( argv[c], "-a" )){
	c++;
	a = atof( argv[c] );
	c++;
      }
      else if ( argv[c][0] == '-' ){
	usage(argc, argv);
      }
      else{
	break;
      }
    }
    a *= M_PI / 180.0;
    argv += c;
    argc -= c;
    //fprintf( stderr, "%d\n", argc );
    child = plugin_load( argc, argv );
  }
  Vec3b map(complex<float> dst)

  //uchar* map(float dstx, float dsty)
  {
    float psi = dst.real() * M_PI;
    float theta = dst.imag() * M_PI;
    float x = cos(theta)*cos(psi);
    float y = cos(theta)*sin(psi);
    float z = sin(theta);
    float x1 = x;
    float y1 = y*cos(a) + z*sin(a);
    float z1 =-y*sin(a) + z*cos(a);
    float theta1 = asin(z1);
    float r = sqrt(1.0-z1*z1);
    float x1r = x1/r;
    float psi1 = acos(x1r);
    if ( x1r > 1.0 || x1r < -1.0 ){
      psi1 = 0.0;
    }
    //printf("%f %f %f %f\n",psi,theta,psi1,theta1);
    if ( y1 < 0.0 ){
      psi1 = - psi1;
    }
    return child->map( complex<float>(psi1/M_PI, theta1/M_PI) );
  }
};


// the class factories
extern "C" Projector* create(int argc, char* argv[]) {
  return new Tilt(argc, argv);
}

extern "C" void destroy(Tilt* p) {
    delete p;
}
