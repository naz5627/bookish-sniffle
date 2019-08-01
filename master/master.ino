//master00
#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>

#define IR 5
#define led 4

RF24 radio(9, 10);               // nRF24L01 (CE,CSN)
RF24Network network(radio);      // Include the radio in the network
const uint8_t this_node = 0;   // Address of this node in Octal format ( 04,031, etc)
const uint8_t node01 = 1;      // Address of the other node in Octal format
const uint8_t node02 = 2;

int statLED;    //LED current status ON or OFF

void setup() 
{
  Serial.begin(9600);
  SPI.begin();
  radio.begin();
  network.begin(90, this_node);  //(channel, node address)
  radio.setDataRate(RF24_2MBPS);
  
  pinMode(IR, INPUT);
  pinMode(led, OUTPUT);

  digitalWrite(led, HIGH);
  statLED = 1;
}

void loop() 
{
  network.update();
  
  //===== Receiving =====//
  while ( network.available() ) 
  {     
    // Is there any incoming data?
    RF24NetworkHeader header;
    int incomingData;
    network.read(header, &incomingData, sizeof(incomingData)); // Read the incoming data
    digitalWrite(led, incomingData); 

    statLED = incomingData;   //set LED status as incomingData
    
    delay(100);   
  }

  //===== Sending =====//
  int potValue = digitalRead(5);
   
  if (statLED != 1)
  {
    //do nothing
  }
  else if ((digitalRead(5) == LOW) && (statLED = 1))
  {
    int randNum = random(1000);
    Serial.println(randNum);
      
    if (randNum <= 500)
    {
      int receivingAddress = 1;    
      int receivingAddressOFF = 2;

      int ledBrightness = 1;
      RF24NetworkHeader header(receivingAddress);   // (Address where the data is going)
      bool ok = network.write(header, &ledBrightness, sizeof(ledBrightness)); // Send the data
      delay(30);
         
      int Brightness = 0;
      RF24NetworkHeader header1(receivingAddressOFF);   // (Address where the data is going)
      bool ok1 = network.write(header1, &Brightness, sizeof(ledBrightness)); // Send the data    
      delay(30);

      digitalWrite(led, LOW);
      statLED = 0;
    }
        
    else
    {
      int receivingAddress = 2;
      int receivingAddressOFF = 1;
      
      int ledBrightness = 1;
      RF24NetworkHeader header(receivingAddress);   // (Address where the data is going)
      bool ok = network.write(header, &ledBrightness, sizeof(ledBrightness)); // Send the data
      delay(30);
      
      int Brightness = 0;
      RF24NetworkHeader header1(receivingAddressOFF);   // (Address where the data is going)
      bool ok1 = network.write(header1, &Brightness, sizeof(ledBrightness)); // Send the data     
      delay(30);

      digitalWrite(led, LOW);
      statLED = 0;
    }
 }  

 Serial.println(statLED);   //check LED status
}
