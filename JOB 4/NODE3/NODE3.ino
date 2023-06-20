#include <painlessMesh.h>
#include <AsyncTCP.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define LED 2 
#define BLINK_PERIOD 3000 
#define BLINK_DURATION 100 
#define MESH_SSID "JOB4"
#define MESH_PASSWORD "1234567890"
#define MESH_PORT 5555
#define mq2  32 // Pin analog untuk sensor MQ-2

LiquidCrystal_I2C lcd(0x27, 16, 2);
// Prototypes
void sendMessage(); 

void receivedCallback(uint32_t from, String & msg);
void newConnectionCallback(uint32_t nodeId);
void changedConnectionCallback(); 
void nodeTimeAdjustedCallback(int32_t offset); 
void delayReceivedCallback(uint32_t from, int32_t delay);
Scheduler userScheduler; // to control your personal task
painlessMesh mesh;
bool calc_delay = false;
SimpleList<uint32_t> nodes;
void sendMessage() ; // Prototype
Task taskSendMessage( TASK_SECOND * 1, TASK_FOREVER, 
&sendMessage ); // start with a one second interval
// Task to blink the number of nodes
Task blinkNoNodes;
bool onFlag = false;
void setup() {
 Serial.begin(9600);
 pinMode(mq2, INPUT);
  lcd.begin(16, 2);
  lcd.init();
  lcd.backlight();
  // dht.begin();
  lcd.print(" Sensor ");
  lcd.setCursor(0, 1);
  lcd.print("Reading...");
  lcd.clear();
 pinMode(LED, OUTPUT);
 mesh.setDebugMsgTypes(ERROR | DEBUG); // set before init() so that you can see error messages
 mesh.init(MESH_SSID, MESH_PASSWORD, &userScheduler, MESH_PORT);
 mesh.onReceive(&receivedCallback);
 mesh.onNewConnection(&newConnectionCallback);
 mesh.onChangedConnections(&changedConnectionCallback);
 mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);
 mesh.onNodeDelayReceived(&delayReceivedCallback);
 userScheduler.addTask( taskSendMessage );
 taskSendMessage.enable();
 blinkNoNodes.set(BLINK_PERIOD, (mesh.getNodeList().size() + 1) * 2, []() {
 // If on, switch off, else switch on

 if (onFlag)
 onFlag = false;
 else
 onFlag = true;
 blinkNoNodes.delay(BLINK_DURATION);
 if (blinkNoNodes.isLastIteration()) {
 // Finished blinking. Reset task for next run 
 // blink number of nodes (including this node) times
 blinkNoNodes.setIterations((mesh.getNodeList().size() + 1) * 2);
 // Calculate delay based on current mesh time and 
 BLINK_PERIOD;
 // This results in blinks between nodes being synced
 blinkNoNodes.enableDelayed (BLINK_PERIOD - (mesh.getNodeTime() % (BLINK_PERIOD*1000))/1000);
 }
 });
 userScheduler.addTask(blinkNoNodes);
 blinkNoNodes.enable();
 randomSeed(analogRead(A0));
}
void loop() {
 mesh.update();
 digitalWrite(LED, !onFlag);
}
void sendMessage() {
  int sensorValue = analogRead(mq2); // Membaca nilai sensor analog

  float voltage = sensorValue * (5.0 / 1023.0); // Mengonversi nilai sensor menjadi tegangan (5V adalah tegangan referensi Arduino)
  
  // Menghitung konsentrasi gas menggunakan rumus yang sesuai dengan sensor MQ-2
  float gasResistance = ((5.0 - voltage) / voltage) * 10.0; // Menggunakan faktor 10.0 untuk mengkoreksi nilai
  { 
  // Menampilkan hasil ke Serial Monitor
  Serial.print("Sensor Value: ");
  Serial.println(sensorValue);

    lcd.setCursor(0, 0);
  lcd.print(" Value: ");
  lcd.print(sensorValue);
  
  Serial.print("Voltage: ");
  Serial.print(voltage);
  Serial.println(" V");
  
 
  Serial.print("Gas Resistance: ");
  Serial.print(gasResistance);
  Serial.println(" KΩ ");

   lcd.setCursor(0, 1);
  lcd.print(" Gas OHM : ");
  lcd.print(gasResistance);

}

 String msg = " NODE 3 ";
 msg += mesh.getNodeId ();
 msg += String(ESP.getFreeHeap()) +  " Value = "  +  String(sensorValue)  +  " Gas : "  + String(gasResistance) +  (" Ω ")  ;
 mesh.sendBroadcast(msg);
 if (calc_delay) {
 SimpleList<uint32_t>::iterator node = nodes.begin();
 while (node != nodes.end()) {
 mesh.startDelayMeas(*node);

 node++;
 }
 calc_delay = false;
 }
 Serial.printf("Sending message: %s\n", msg.c_str());
 taskSendMessage.setInterval( random(TASK_SECOND * 1, TASK_SECOND * 5)); // between 1 and 5 seconds
}
void receivedCallback(uint32_t from, String & msg) {
 Serial.printf("startHere: Received from %u msg=%s\n", from, msg.c_str());
}
void newConnectionCallback(uint32_t nodeId) {
 // Reset blink task
 onFlag = false;
 blinkNoNodes.setIterations((mesh.getNodeList().size() + 1) * 2);
 blinkNoNodes.enableDelayed(BLINK_PERIOD -
(mesh.getNodeTime() % (BLINK_PERIOD*1000))/1000);
 Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
 Serial.printf("--> startHere: New Connection, %s\n", mesh.subConnectionJson(true).c_str());
}
void changedConnectionCallback() {
 Serial.printf("Changed connections\n");
 // Reset blink task
 onFlag = false;
 blinkNoNodes.setIterations((mesh.getNodeList().size() + 1) * 2);
 blinkNoNodes.enableDelayed(BLINK_PERIOD - (mesh.getNodeTime() % (BLINK_PERIOD*1000))/1000);

 nodes = mesh.getNodeList();
 Serial.printf("Num nodes: %d\n", nodes.size());
 Serial.printf("Connection list:");
 SimpleList<uint32_t>::iterator node = nodes.begin();
 while (node != nodes.end()) {
 Serial.printf(" %u", *node);
 node++;
 }
 Serial.println();
 calc_delay = true;
}
void nodeTimeAdjustedCallback(int32_t offset) {
 Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(), offset);
}
void delayReceivedCallback(uint32_t from, int32_t delay) {
 Serial.printf("Delay to node %u is %d us\n", from, delay);
}