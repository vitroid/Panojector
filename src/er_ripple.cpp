#include "plugin.hpp"
#include <iostream>
#include <cmath>

float ripple(float r, float decay)
{
//erfc is for cutting off the progressive ropples
  return cos(2*M_PI*r)*erfc(-r*2)*exp(-r*decay);
}



void
angle2vec(float theta, float phi, float (&vec)[3])
{
  vec[2] = cos(theta);
  float r = sin(theta);
  vec[0] = r * cos(phi);
  vec[1] = r * sin(phi);
}


void
vec2angle(const float (&e)[3], float &theta, float &phi)
{
  float r = sqrt(e[0]*e[0]+e[1]*e[1]);
  phi = atan2(e[1],e[0]);
  theta = atan2(r,e[2]);
}


float dot(const float (&v1)[3], const float (&v2)[3])
{
  return v1[0]*v2[0] + v1[1]*v2[1] + v1[2]*v2[2];
}


float norm(const float (&v)[3])
{
  return sqrt(dot(v,v));
}


void
cross(const float (&v1)[3], const float (&v2)[3], float (&result)[3])
{
  result[0] = v1[1]*v2[2] - v1[2]*v2[1];
  result[1] = v1[2]*v2[0] - v1[0]*v2[2];
  result[2] = v1[0]*v2[1] - v1[1]*v2[0];
}


void
rotation_matrix(const float (&axis)[3], float& theta, float (&matrix)[3][3])
{
  /*
    Return the rotation matrix associated with counterclockwise rotation about
    the given axis by theta radians.
  */
  float e[3];
  float L = norm(axis);
  for(int i=0;i<3; i++){
    e[i] = axis[i] / L;
  }
  float a = cos(theta/2.0);
  float s = sin(theta/2.0);
  float b = -e[0]*s;
  float c = -e[1]*s;
  float d = -e[2]*s;
  float aa=a*a;
  float bb=b*b;
  float cc=c*c;
  float dd=d*d;
  float bc=b*c;
  float ad=a*d;
  float ac=a*c;
  float ab=a*b;
  float bd=b*d;
  float cd=c*d;
  matrix[0][0] = aa+bb-cc-dd;
  matrix[0][1] = 2*(bc+ad);
  matrix[0][2] = 2*(bd-ac);
  matrix[1][0] = 2*(bc-ad);
  matrix[1][1] = aa+cc-bb-dd;
  matrix[1][2] = 2*(cd+ab);
  matrix[2][0] = 2*(bd+ac);
  matrix[2][1] = 2*(cd-ab);
  matrix[2][2] = aa+dd-bb-cc;
}


class Tilt : public Projector {
protected:
  float a;
  Projector* child;
  float center[3];
  float radius;     //in radian
  float wavelen; // in radian
  float amplitude;  // magnitude of pixel shift
  float decay;      // in 1 wavelength
public:
  void usage( int argc, char* argv[] )
  {
    fprintf( stderr, "Usage: %s [-r long,lat,radius,wavelength,amp,decay] [projectors]\n", argv[0] );
    fprintf( stderr, "Add ripple on a equirectangular image.\n" );
    fprintf( stderr, "Options:\n" );
    fprintf( stderr, "\t-r a,b,c,d,e,f\tSpecify ripple parameters.\n" ); 
    exit(1);
  }
  Tilt(int argc, char* argv[])
  {
    float longitude=0, latitude=0;//in [-90,90],[180,180]
    amplitude = 5;
    radius    = 1;
    wavelen   = 0.1;
    decay     = 0.2;

    int c = 1;
    while ( c < argc ){
      if ( 0 == strcmp( argv[c], "-r" )){
	c++;
	sscanf(argv[c], "%f,%f,%f,%f,%f,%f", &longitude, &latitude, &radius, &wavelen, &amplitude, &decay );
	c++;
      }
      else if ( argv[c][0] == '-' ){
	usage(argc, argv);
      }
      else{
	break;
      }
    }
    //longitude, latitude  are zero at the center of ER picture.
    //ranges [-pi,pi] and [-pi/2,pi/2]
    //lat=pi/2 is the north pole.

    //change them. now origin is at the center top of ER
    latitude = 90 - latitude;
    latitude  *= M_PI / 180.0;
    longitude *= M_PI / 180.0;
    angle2vec(latitude, longitude, center);
    argv += c;
    argc -= c;
    //fprintf( stderr, "%d\n", argc );
    child = plugin_load( argc, argv );
  }
  uchar* map(float dstx, float dsty)
  {
    float psi = dstx * M_PI;
    float theta = (dsty+0.5) * M_PI;
    // In this plugin, theta=0 is the z-axis and is the horizontal line at y=-0.5 (a quaerter from the top)
    float point[3];
    angle2vec(theta,psi,point);
    
    float axis[3];
    cross(center,point,axis);
    float sine   = norm(axis);
    float cosine = dot(center, point);
    
    float arc;
    arc = atan2(sine, cosine); //y, x
    float ripple_height = amplitude * ripple((radius-arc)/wavelen,decay);
    float displ = arc + ripple_height;  // in radian
    float rotmat[3][3];
    rotation_matrix(axis, displ, rotmat);
    float source[3];
    for(int i=0; i<3; i++){
      float s=0;
      for(int j=0; j<3; j++){
        s += rotmat[i][j] * center[j];
      }
      source[i] = s;
    }
    float src_th, src_ph;
    vec2angle(source, src_th, src_ph);
    return child->map( src_ph/M_PI, src_th/M_PI+0.5 );
  }
};


// the class factories
extern "C" Projector* create(int argc, char* argv[]) {
  return new Tilt(argc, argv);
}

extern "C" void destroy(Tilt* p) {
    delete p;
}
