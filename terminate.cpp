#include "plugin.hpp"

class Terminate : public Projector {
public:
  int map(float dstx, float dsty, float& srcx, float& srcy) const
  {
    srcx = dstx;
    srcy = dsty;
    return 1;
  }
};


// the class factories

extern "C" Projector* create(int argc, char* argv[]) {
  return new Terminate();
}

extern "C" void destroy(Projector* p) {
    delete p;
}
