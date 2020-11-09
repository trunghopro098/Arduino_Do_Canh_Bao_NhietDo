
#include <DHT.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h> // Thư viện i2c cho lcd
LiquidCrystal_I2C lcd(0x27,16,2); //đại chỉ i2c 0x27
unsigned long time1 = 0;
const int DHTPIN = A0;
const int DHTTYPE = DHT11;
DHT dht(DHTPIN, DHTTYPE);

float t =0;
double Temp = 0;
#define minTempC 0   // about 109F
#define maxTempC 150  // about 183F
#define startTempC 32   // giá trị nhiệt độ cài ban đầu
float setTempC;  
#define buttonDN 5 // chân nút nhấn 1 nối chân số 4
#define buttonUP 4 // chân nút nhấn 2 nối chân số 5
#define role 8    // chân relay nối chân số 8
#define coi 3
byte degree[8] = { // dòng này để hiển thị kí tự độ C
  0B01110,
  0B01010,
  0B01110,
  0B00000,
  0B00000,
  0B00000,
  0B00000,
  0B00000
};

void setup() 
{
   
  Serial.begin(9600);
  pinMode( DHTPIN,INPUT_PULLUP);
  pinMode(buttonDN, INPUT_PULLUP);
  pinMode(buttonUP, INPUT_PULLUP);
  pinMode(role, OUTPUT);//KICH ROLAY CHAN 
  digitalWrite(role,HIGH);
  pinMode(coi, OUTPUT);//coi bao 
  digitalWrite(coi,HIGH);
  setTempC = startTempC;
}

void loop() 
{
 setTemperature();
  displayLCD();
  setHeater();
}



void displayLCD(){ 
  t = dht.readTemperature();
  dht.begin();  
  lcd.begin();  
  lcd.backlight();// bạt tắt màn hình LCD
  lcd.clear();
  lcd.home (); 
  Serial.print("C = ");
  Serial.println(Temp);
       
  lcd.setCursor(0,0);
  lcd.print("NHIET DO: ");
  lcd.setCursor(10,0);
  lcd.print(round(t));
  lcd.print("");
  lcd.write(1);
  lcd.print("C");
 
  lcd.setCursor(0,1);
  lcd.print("CAI : ");
  lcd.setCursor(5,1);
  lcd.print(round(setTempC));
  lcd.print("");
  lcd.write(1);
  lcd.print("C");
  lcd.createChar(1, degree);

}

void setTemperature()
{
        if (digitalRead(buttonDN)== HIGH){ // Trường hợp nút nhấn được nhấn
                        setTempC++;  //  tăng giá trị cài lên 1 đơn vị
      if(setTempC < minTempC) 
      setTempC = minTempC;
      
    }
    if (digitalRead(buttonUP)==HIGH){
                        setTempC--;//  giảm giá trị cài lên 1 đơn vị
      if(setTempC > maxTempC)
      setTempC = maxTempC;
    
    }
  }

  void setHeater()
  
{
  
     if ((setTempC) > (t + 0.5 )) {
                 digitalWrite(role, HIGH);
                 lcd.setCursor(11,1);
                 lcd.print("DONG");
                 //delay(10000); 
    
               }
           else  if ((setTempC) <= (t - 0.5)){
                 digitalWrite(role, LOW);
                 //digitalWrite(coi, LOW);
                 beep();
                 lcd.setCursor(11,1);
                 lcd.print(" MO");
               //  delay(10000);  
                                
       }
}

void beep() { // còi kêu khi nhiệt độ cài nhỏ hơn nhiệt độ hiện tại
      
            digitalWrite(coi, LOW);
            delay(800);  
            digitalWrite(coi, HIGH);
            delay(800);    
  }
