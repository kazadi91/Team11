////////////////////////////////////////////////////////////////////////
// Team 11  - ExoHand
// 
////////////////////////////////////////////////////////////////////////
#include <Servo.h>             //Servo library
Servo servo_left;        //initialize a servo object for the connected servo  
Servo servo_right;                
   
//GPIO13 CONNECTION LED STATUS
#define LED 13
#define BAUDRATE 57600
#define DEBUGOUTPUT 0

//GPIO 9 OUTPUT
#define FOCUSLED 9
#define FOCUSLED2 10

#define powercontrol 10

// checksum variables
byte generatedChecksum = 0;
byte checksum = 0; 
int payloadLength = 0;
byte payloadData[64] = {
  0};
byte poorQuality = 0;
byte attention = 0;
byte meditation = 0;

// system variables
long lastReceivedPacket = 0;
boolean bigPacket = false;

//////////////////////////
// Microprocessor Setup //
//////////////////////////
void setup() {

  servo_left.attach(7);
  servo_right.attach(7);
  pinMode(button,INPUT);
  pinMode(FOCUSLED, OUTPUT);
  pinMode(FOCUSLED2, OUTPUT);

  
  Serial.begin(BAUDRATE);           // USB

}

////////////////////////////////
// Read data from Serial UART //
////////////////////////////////
byte ReadOneByte() {
  int ByteRead;

  while(!Serial.available());
  ByteRead = Serial.read();

#if DEBUGOUTPUT  
  Serial.print((char)ByteRead);   // echo the same byte out the USB serial (for debug purposes)
#endif

  return ByteRead;
}

/////////////
//MAIN LOOP//
/////////////
void loop() {
buttonState = digitalRead(button)
if(buttonState == LOW){
 Serial.print("Exoskeleton is OFF\n");
 servo_left.write(0);              
 servo_right.write(0);
 digitalWrite(FOCUSLED, LOW);
 digitalWrite(FOCUSLED2, LOW)
 }else{
  // Look for sync bytes
  if(ReadOneByte() == 170) {
    if(ReadOneByte() == 170) {

      payloadLength = ReadOneByte();
      if(payloadLength > 169)                      //Payload length can not be greater than 169
          return;

      generatedChecksum = 0;        
      for(int i = 0; i < payloadLength; i++) {  
        payloadData[i] = ReadOneByte();            //Read payload into memory
        generatedChecksum += payloadData[i];
      }   

      checksum = ReadOneByte();                      //Read checksum byte from stream      
      generatedChecksum = 255 - generatedChecksum;   //Take one's compliment of generated checksum

        if(checksum == generatedChecksum) {    

        poorQuality = 200;
        attention = 0;
        meditation = 0;

        for(int i = 0; i < payloadLength; i++) {    // Parse the payload
          switch (payloadData[i]) {
          case 2:
            i++;            
            poorQuality = payloadData[i];
            bigPacket = true;            
            break;
          case 4:
            i++;
            attention = payloadData[i];                        
            break;
          case 5:
            i++;
            meditation = payloadData[i];
            break;
          //Adjusts smoothing
          case 0x80:
            i = i + 4;
            break;
         
          case 0x83:
            i = i + 23;      
            break;
          default:
            break;
          } // switch
        } // for loop


        // Serial Monitor

        for(test = 0; test <=100; test++) {
    
          Serial.print("test: ");
          Serial.print(test, DEC);
          Serial.print("\n");

          ///Parameters for hand movements
if(poorQuality<10){
          switch(test) {
          case 0 ... 29:          //for attention values 0 through 29
            servo_left.write(0);              
            servo_right.write(0);
            digitalWrite(FOCUSLED, LOW);
            digitalWrite(FOCUSLED2, LOW);
            delay(1000);
            break;
          case 30 ... 49:
            servo_left.write(45);              
            servo_right.write(45);
            digitalWrite(FOCUSLED, HIGH);
            digitalWrite(FOCUSLED2, LOW);
            delay(1000);
            break;
          case 50 ... 75:
            servo_left.write(90);              
            servo_right.write(90);
            digitalWrite(FOCUSLED, LOW);
            digitalWrite(FOCUSLED2, HIGH);
            delay(1000);
            break;
          case 76 ... 100:
            servo_left.write(0);              
            servo_right.write(0);
            digitalWrite(FOCUSLED, LOW);
            digitalWrite(FOCUSLED2, LOW);
            delay(1000);
            break;
          }                     
        }
      }     
        bigPacket = false;        
        } // Checksum Error
      } // end if else for checksum
    } // end if read 0xAA byte
  } // end if read 0xAA byte
}
