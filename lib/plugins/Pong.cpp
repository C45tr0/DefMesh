//************************************************************
//************************************************************
#include "Pong.h"

Pong::Pong(painlessMesh &mesh)
  : _mesh(mesh) {
}

void Pong::onMessage(String message) {
  if (!message.startsWith("PING-")) {
    return;
  }

  Serial.println(message.substring(5));
  uint32_t from = atoll(message.substring(5).c_str());

  String msg = "PONG-";
  msg += _mesh.getNodeId();
  _mesh.sendSingle(from, msg);
  Serial.println("PONG");
}
