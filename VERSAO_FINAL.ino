#include <dht.h>
#include <LiquidCrystal.h>
#include <Ultrasonic.h>
#define pino_trigger A2
#define pino_echo A3
#define dht_dpin A1 
dht DHT;
int n = 1;
int sensorValue = 0;        
int timer = 500; 
int time = 50;
int distancia = 0;
int pino_D0 = 6;
 int rpm;
volatile byte pulsos;
unsigned long timeold;
unsigned int pulsos_por_volta = 20;
float cmMsec;
int L1 = 7, L2 = 8, L3 = 9, L4 = 10;
int control=0;
int contador1 =0;
int onoff =0;

  
Ultrasonic ultrasonic(pino_trigger, pino_echo);
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

int Verificar() {
   
     if (cmMsec > 10) {   
     frente(100);   
   }else{   
   para(50);
   re(100);
   direita(300);        
}
}

void contador()
{
  pulsos++;
}

void MOVER(int l1, int l2, int r1, int r2)
{
  digitalWrite(L1, l1);
  digitalWrite(L2, l2);
  digitalWrite(L3, r1);
  digitalWrite(L4, r2);  
}
void re(int wait)
{
  Serial.println("RE");
  MOVER(LOW, HIGH, LOW, HIGH);
  delay(wait);
}
void frente(int wait)
{
  Serial.println("FRENTE");
  MOVER(HIGH, LOW, HIGH, LOW);
  delay(wait);
}

void direita(int wait)
{
  Serial.println("ESQUERDA");
  MOVER(HIGH, LOW, LOW, HIGH);
  delay(wait);
}

void esquerda(int wait)
{
  Serial.println("DIREITA");
  MOVER(LOW, HIGH, HIGH, LOW);
  delay(wait);
}

void para(int wait)
{
  Serial.println("PARA");
  MOVER(LOW, LOW, LOW, LOW);
  delay(wait);
}

void setup(){
  Serial.begin(9600);
  pinMode(6, INPUT);
  pinMode(pino_trigger,OUTPUT);
  pinMode(pino_echo,INPUT);
  pinMode(L1, OUTPUT);     
  pinMode(L2, OUTPUT);     
  pinMode(L3, OUTPUT);     
  pinMode(L4, OUTPUT);
  Serial.println("Lendo dados do sensor...");
  attachInterrupt(0, contador, FALLING);
   pulsos = 0;
  rpm = 0;
  timeold = 0;
  lcd.begin(16, 2);
  lcd.print("CARRINHO AUTONOMO");
  delay (1000);
  
}
 
 void loop()
{
  int chk = DHT.read11(dht_dpin);
  float cmMsec;
  long microsec = ultrasonic.timing();
  cmMsec = ultrasonic.convert(microsec, Ultrasonic::CM);
  Serial.begin(9600);
  
  
timer = map(sensorValue, 0, 1023, 50, 500);
if (millis() - timeold >= 100)
  {
    detachInterrupt(0);
    rpm = ((60 * 100 / pulsos_por_volta )/(millis() - timeold) * pulsos);
    timeold = millis();
    pulsos = 0;
   Serial.print("RPM = ");
   Serial.println(rpm, DEC);
    
    attachInterrupt(0, contador, FALLING);
  }
  
if(onoff==0)
       {
      if (cmMsec > 10) {   
     frente(100);   
   }else{   
   para(50);
   re(100);
   direita(300);    

 //Serial.print("RPM = ");
 //   Serial.println(rpm, DEC);
  }
 }
       
        if (Serial.available() > 0) {
        control = Serial.read();
        if(control == 'M') // Mudança de direção
        {
         onoff = 1;
         Serial.print("onoff==1");
        }
        if(control == 'm') // Mudança de direção
        {
         onoff = 0;
         Serial.print("onoff==1");
        }
        if(control == 'F') // andar para frente
        {
          frente(100);
        }  
        if(control == 'B')//Andar para trás 
        {
          re(50); 
        }
         if(control == 'L')  //Virar Direita  
        {
          direita(100);
        }
          if(control == 'R')  //Virar Esquerda  
        {
         esquerda(100);
          
        }
        if(control == 'S')  // Parar
           {
            para(100);
           }
 } 
         
        contador1 = contador1 + 1;
        if(contador >= 40){

      
  lcd.setCursor(0,0); 
lcd.print("Temp: ");
 Serial.print("Distancia em cm: ");
  Serial.print(cmMsec);
Serial.print("Temp: ");
Serial.print(DHT.temperature);
Serial.print("C");
Serial.print("\t");
Serial.print("\n");
Serial.print("umidade: ");
Serial.print(DHT.humidity);
Serial.print("%");
Serial.print("\t");
Serial.print("\n"); 
lcd.print(DHT.temperature);
lcd.print((char)223);
lcd.print("C");
lcd.setCursor(0,1);
lcd.print("umidade: ");
lcd.print(DHT.humidity);
lcd.print("%");
lcd.setCursor(0,0); 
delay(100);
lcd.print("rpm:");
lcd.print(rpm, DEC);
lcd.print("        ");
lcd.setCursor(0,1);
lcd.print("distancia:");
lcd.print(cmMsec);
lcd.print("cm");
delay(100);
 contador1 = 0;
        }
}  
