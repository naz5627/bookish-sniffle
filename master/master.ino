//master00
#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>

#define IR 5
#define led 4
#define total_unit 4 //5-1(unit 0 to 4 is 5unit)

RF24 radio(9, 10);               // nRF24L01 (CE,CSN)
RF24Network network(radio);      // Include the radio in the network
const uint8_t this_node = 0;   // Address of Master node in Octal format ( 04,031, etc)
const uint8_t node01 = 1;// Address of the Slave_1 node in Octal format
const uint8_t node02 = 2;// Address of the Slave_2 node in Octal format
const uint8_t node03 = 3;// Address of the Slave_3 node in Octal format
const uint8_t node04 = 4;// Address of the Slave_4 node in Octal format

int statLED=0;    //LED current status ON or OFF

void all_off(void);
void setup() 
{
  Serial.begin(9600);
  SPI.begin();
  radio.begin();
  network.begin(90, this_node);  //(channel, node address)
  radio.setDataRate(RF24_2MBPS);
  
  pinMode(IR, INPUT);
  pinMode(led, OUTPUT);

  //turn on master node only
  if(this_node == 0)
  {
    statLED = 1;
    digitalWrite(led, HIGH);
  }
  else
  {
    statLED= 0;
  }
}

void loop() 
{
  int on_target_address;
  //if led is currently off, always do receiving 
  if (statLED != 1)
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
  }
  else if ((digitalRead(5) == LOW) && (statLED = 1))
  {
    int randNum = random(total_unit);
    Serial.println(randNum);
 
    //send all off
    all_off();
    
      //===== Sending =====//
      switch (randNum)
      {
        case 1:
        on_target_address = node01;
        statLED = 0;
        break;
        case 2:
        on_target_address = node02;
        statLED = 0;
        break;
        case 3:
        on_target_address = node03;
        statLED = 0;
        break;
        case 4:
        on_target_address = node04;
        statLED = 0;
        break;
        default:
        //todo
        //off for 1 second then self on again
        break;
      }

      int ledBrightness = 1;
      RF24NetworkHeader header(on_target_address);   // (Address where the data is going)
      bool ok = network.write(header, &ledBrightness, sizeof(ledBrightness)); // Send the data
      delay(30);

      digitalWrite(led, LOW);
      
    }
 }  

 Serial.println(statLED);   //check LED status
}

void all_off(void)
{
  //todo
  //use for loop to send all off
  //int Brightness = 0;
      //RF24NetworkHeader header1(receivingAddressOFF);   // (Address where the data is going)
      //bool ok1 = network.write(header1, &Brightness, sizeof(ledBrightness)); // Send the data    
      //delay(30);
}
