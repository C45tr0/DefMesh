
#ifndef pong_h
#define pong_h
#include "painlessMesh.h"

class Pong {

public:
  Pong(painlessMesh &mesh);

  virtual void onMessage(String msg);

private:
  painlessMesh &_mesh;
};

#endif