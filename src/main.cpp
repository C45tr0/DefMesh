//************************************************************
// This is a 
//
//
//************************************************************
#include "../lib/HashMap/HashMap.h"
#include "painlessMesh.h"
#include "../lib/plugins/Mute.h"
#include "../lib/plugins/Ping.h"
#include "../lib/plugins/Pong.h"

#define   MESH_PREFIX     "whateverYouLike"
#define   MESH_PASSWORD   "somethingSneaky"
#define   MESH_PORT       5555

#define MUTE_BTN_PIN 22
#define MUTE_LED_PIN 23

#define PING_BTN_PIN 15

#define LEVEL_1_PIN 21
#define LEVEL_2_PIN 19
#define LEVEL_3_PIN 18
#define LEVEL_4_PIN 17
#define LEVEL_5_PIN 16

// HashMap Setup
// The size of 50 was just randomly assigned..
CreateHashMap(nodeDelays, uint32_t, int32_t, 50);
uint32_t nodeIds[50] = {};

Scheduler userScheduler; // to control your personal task
painlessMesh  mesh;
Mute mute(MUTE_LED_PIN, MUTE_BTN_PIN, 50, false, true);
Ping ping(mesh, PING_BTN_PIN, 50, false, true);
Pong pong(mesh);

// User stub
void sendMessage() ; // Prototype so PlatformIO doesn't complain

Task taskSendMessage( TASK_SECOND * 1 , TASK_FOREVER, &sendMessage );

void sendMessage() {
  String msg = "Hello from node ";
  msg += mesh.getNodeId();
  mesh.sendBroadcast( msg );
  taskSendMessage.setInterval( random( TASK_SECOND * 1, TASK_SECOND * 5 ));
  Serial.printf("startHere: Sent broadcast msg=%s\n", msg.c_str());

  float_t average = 0;

  if (nodeDelays.size() > 0) {
    for(int k = 0; k < nodeDelays.size(); k++){
      average = average + nodeDelays.valueAt(k);
    }

    average = average / nodeDelays.size();

    Serial.printf("Average: %f", average);
  
    digitalWrite(LEVEL_1_PIN, average < 30000 ? HIGH : LOW);
    digitalWrite(LEVEL_2_PIN, average < 24000 ? HIGH : LOW);
    digitalWrite(LEVEL_3_PIN, average < 18000 ? HIGH : LOW);
    digitalWrite(LEVEL_4_PIN, average < 12000 ? HIGH : LOW);
    digitalWrite(LEVEL_5_PIN, average < 6000 ? HIGH : LOW);
  } else {
    digitalWrite(LEVEL_1_PIN, HIGH);
    digitalWrite(LEVEL_2_PIN, LOW);
    digitalWrite(LEVEL_3_PIN, HIGH);
    digitalWrite(LEVEL_4_PIN, LOW);
    digitalWrite(LEVEL_5_PIN, HIGH);
  }
}

// Needed for painless library
void receivedCallback( uint32_t from, String &msg ) {
  Serial.printf("Received from %u msg=%s\n", from, msg.c_str());
  pong.onMessage(msg);
  mesh.startDelayMeas(from);
}

void newConnectionCallback(uint32_t nodeId) {
  Serial.printf("New Connection, nodeId = %u\n", nodeId);
}

void changedConnectionCallback() {
  Serial.printf("Changed connections %s\n",mesh.subConnectionJson().c_str());

  auto nodes = mesh.getNodeList();

  std::fill_n(nodeIds, 50, 0);

  int i = 0;
  
  while (!nodes.empty()) {
    nodeIds[i] = nodes.front();
    nodes.pop_front();
    i = i + 1;
  }

  i = 0;

  while (i < nodeDelays.size()) {
    uint32_t nodeId = nodeDelays.keyAt(i);
    bool exists = false;

    for(int k = 0; k < 50; k++){
      if (nodeIds[k] == 0) {
        break;
      }

      if(nodeId == nodeIds[k]){
        exists = true;
        break;
      }
    }

    if (!exists) {
        nodeDelays.remove(nodeId);
    } else {
        i = i + 1;
    }
  }
}

void nodeTimeAdjustedCallback(int32_t offset) {
  Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(),offset);
}

void nodeDelayReceivedCallback(uint32_t nodeId, int32_t delay) {
  Serial.printf("Node = %u. Delay = %d\n", nodeId, delay);
  nodeDelays[nodeId] = delay;
}

void setup() {
  Serial.begin(115200);

  //mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE ); // all types on
  mesh.setDebugMsgTypes( ERROR | STARTUP );  // set before init() so that you can see startup messages

  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT );
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);
  mesh.onNodeDelayReceived(&nodeDelayReceivedCallback);

  userScheduler.addTask( taskSendMessage );
  taskSendMessage.enable();

  mute.init();
  ping.init();
  
  pinMode(LEVEL_1_PIN, OUTPUT);
  pinMode(LEVEL_2_PIN, OUTPUT);
  pinMode(LEVEL_3_PIN, OUTPUT);
  pinMode(LEVEL_4_PIN, OUTPUT);
  pinMode(LEVEL_5_PIN, OUTPUT);
}

void loop() {
  userScheduler.execute(); // it will run mesh scheduler as well
  mesh.update();

  mute.loop();
  ping.loop();

  /*
  // Could do mesh stability...
  Serial.printf("Stability = %u\n", mesh.stability);
  auto stability = mesh.stability;

  digitalWrite(LEVEL_1_PIN, stability > 200 ? HIGH : LOW);
  digitalWrite(LEVEL_2_PIN, stability > 350 ? HIGH : LOW);
  digitalWrite(LEVEL_3_PIN, stability > 500 ? HIGH : LOW);
  digitalWrite(LEVEL_4_PIN, stability > 650 ? HIGH : LOW);
  digitalWrite(LEVEL_5_PIN, stability > 800 ? HIGH : LOW);
  */
}
