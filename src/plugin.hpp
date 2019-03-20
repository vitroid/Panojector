#ifndef PLUGIN_HPP
#define PLUGIN_HPP
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <ctype.h>
#include <dlfcn.h>
#include <complex>
using namespace std;
using namespace cv;

class Projector
{
public:
  Projector(){};
  virtual Vec3b map(complex<float> dst) = 0;
};



// the types of the class factories
typedef Projector* create_t(int argc, char* argv[]);
typedef void destroy_t(Projector*);

//common utility functions
Projector* plugin_load(int argc, char* argv[]);
//void getpixel( IplImage* const src, float x,float y, IplImage* dst, int dx, int dy );
//Same coordinate as the screen. Origin is at the corner.
//void white( IplImage* dst, int dx, int dy );
//Same coordinate as the screen. Origin is at the corner.
void spherical2cartesian(float phi, float theta, float& x, float& y, float& z);
void cartesian2spherical(float x, float y, float z, float& phi, float& theta);

#endif
