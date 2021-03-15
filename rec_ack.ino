
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(7, 8); // CE, CSN
char recieved_data[10];
int ackdata[2]={1,0};
bool newdata = false;
//char data_send[5];

const byte addresses[][6] = {"00001", "00002"};

void setup() {
Serial.begin(115200);

    Serial.println("SimpleRxAckPayload Starting");
    radio.begin();
    radio.setDataRate( RF24_2MBPS );
    radio.openWritingPipe(addresses[0]); // 00001
    radio.openReadingPipe(1, addresses[1]);
    radio.setRetries(5,5);
    radio.enableAckPayload();


}

void loop() {
  getdata();
  showData();

}

void getdata(){
  if(radio.available()){
    radio.read(&recieved_data,sizeof(recieved_data));
    newdata = true;
    
    }
}
void showData(){
  if(newdata== true){
    Serial.print("load_cell weight = ");
    Serial.println(recieved_data);
 //   Serial.print("data_sent");
//    Serial.println(ackdata);
    newdata = false;
    
    }
  
  }

//// \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
 /*void senddata(){
//  radio.enableAckPayload();
    
//    radio.startListening();

//    radio.writeAckPayload(1, &ackdata, sizeof(ackdata));*/
   
