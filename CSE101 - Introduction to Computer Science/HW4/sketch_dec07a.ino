int incomingByte;
int incomingNum;
int sentNum=3;
int a=-1;
const int buttonPin = 2; //Place of the button's cable on arduino//
int buttonState = 0;
int push=0;
int b=0;
int x=0;
void setup(){
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  pinMode(buttonPin, INPUT);
}


void loop() {
  if(Serial.available()>0){
    x=0;
    incomingByte=Serial.read();
    switch(incomingByte){
      case 'a':
      digitalWrite(LED_BUILTIN, HIGH);
      break;
      
      case 'b':
      digitalWrite(LED_BUILTIN, LOW);
      break;

      case 'c':
      digitalWrite(LED_BUILTIN, LOW);
      delay(500);
      for(int i=0; i<7; i++){
        if(i==0 || i==2 || i==4){
          digitalWrite(LED_BUILTIN, HIGH);
          delay(500);
          }
        if(i==1 || i==3 || i==5){
          digitalWrite(LED_BUILTIN, LOW);
          delay(500);
          }
        }

      break;

      case 'd':
      while(a==-1){ //waits until 'a' get an input//
      a=Serial.read();
      }
      sentNum=a*a;
      Serial.println(sentNum);
      a=-1;
      break; 

      case 'e':
      while(x==0){
      buttonState=digitalRead(buttonPin);
      if(buttonState==HIGH){
        push=1;
        for(int i=0; i<85; i++){
          delay(i);
          buttonState=digitalRead(buttonPin);
          if(buttonState==LOW){
            i=101;
            }
          if(i==84){
            push=0;
            Serial.println('e');
            x=1;
            }
           
        }
          }
      
      buttonState=digitalRead(buttonPin);
      if(buttonState==LOW && push==1){ //I dont want send data when pushing button// 
        Serial.println('x');
        push=0;
        }
      
      }
  
      
      break;
      
    }
      
      
  }
}
