#include <math.h>
#include <Separador.h>
#include <TinyGPS.h>

TinyGPS gps;

float LAT_D=0; //Latitude utilizada
float LON_D=0; //longitude utilizada
float LAT_D2=0; //Latitude recebida
float LON_D2=0;// Longitude recebida
String LAT_D_str;
String LON_D_str; 
bool newData = false;
char C;
  
//  Auxiliares
float X1=0, te1=0, te2=0;
float Y1=0;
float X2=0;
float Y2=0;
float X3=0;
float Y3=0; 
int a=9; 
int b=9; 
int c=0; 
int z1;
int z2;
float dist1;
float i1;
float i2;
float dist;
String decod;  // String recebida pelo apk
//int t=100;
int v=250;
float lat_gps;
float lon_gps;
unsigned long age;
float latitude, longitude;
unsigned long fix_age;


Separador s;

void setup() 
{
  pinMode(2,OUTPUT); //Entrada1 da ponte h - motor 1
  pinMode(3,OUTPUT);  //Entrada2 da ponte h - motor 1
  pinMode(4,OUTPUT); //Entrada1 da ponte h - motor 2
  pinMode(5,OUTPUT);  //Entrada2 da ponte h - motor 2
  digitalWrite(2,LOW); // O motor começa parado 1
  digitalWrite(3,LOW);
  digitalWrite(4,LOW); // O motor começa parado 2
  digitalWrite(5,LOW);
  Serial1.begin(9600); // Para o GPS
  Serial2.begin(9600); // Para o Bluetooth
  Serial.begin(9600);
  Serial.println("Começou");

    while (Serial1.available())
    { 
      char C = Serial1.read();
        if (gps.encode(C)) // Did a new valid sentence come in?
          newData = true;
    }
     gps.f_get_position(&latitude, &longitude);//, &fix_age);
  }  
   
void loop() 
{
  for (unsigned long start = millis(); millis() - start < 500;)
  {
    while (Serial1.available())
    { 
      char C = Serial1.read();
        if (gps.encode(C)) // Did a new valid sentence come in?
          newData = true;
    }
  }  
   if (newData)
   {
      decod=Serial2.readString();
      Serial.println(" ");
      Serial.println(decod);
      LAT_D_str=s.separa(decod,':',1);
      LAT_D2=LAT_D_str.toFloat();  //Converete a coordenada em float
      LON_D_str=s.separa(decod,':',2);
      LON_D2=LON_D_str.toFloat();    //Converte a coordenada em float 
      
      if(LAT_D2!=0 && LON_D2!=0)
      {  LAT_D=LAT_D2 ;
      LON_D=LON_D2;
     }
     if(LAT_D2==0 && LON_D2==0)
      {  LAT_D=LAT_D;
      LON_D=LON_D;
     }
            
    Serial.print("LAT_DESTINO=");
    Serial.print(LAT_D, 6);
    Serial.print(" ");
    Serial.print("LON_DESTINO=");
    Serial.print(LON_D, 6);
for (unsigned long start = millis(); millis() - start < 100;)
  {
    while (Serial1.available())
    { 
      char C = Serial1.read();
        if (gps.encode(C)) // Did a new valid sentence come in?
          newData = true;
    }
  }  
    gps.f_get_position(&latitude, &longitude);//, &fix_age);
    Y1=latitude;
    X1=longitude;
    Serial.print(" LAT=");
    Serial.print(Y1, 6);
    Serial.print(" LON=");
    Serial.print(X1, 6);
    Serial.print(" ");

dist1 = gps.distance_between(LAT_D, LON_D, latitude, longitude);
   
    Serial.print("dist_1=");
    Serial.print(dist1,6);
    Serial.println(" ");

    
  if(dist1>15)
    {
    andar();
    
for (unsigned long start = millis(); millis() - start < 100;)
  {
    while (Serial1.available())
    { 
      char C = Serial1.read();
        if (gps.encode(C)) // Did a new valid sentence come in?
          newData = true;
    }
  }  
    gps.f_get_position(&latitude, &longitude);
    Y2=latitude;
    X2=longitude;
    Serial.print(" valor_de_y2=");
    Serial.print(Y2, 6);
    Serial.print(" valor_de_x2=");
    Serial.print(X2, 6);
        
    a = P(Y1, X1, Y2, X2);
    Serial.println(" ");
    Serial.print("valor_de_a=");
    Serial.print(a);
    Serial.println(" ");
    
    b = D(LAT_D, LON_D, X1, Y1);
    Serial.print("valor_de_b=");
    Serial.print(b);
    Serial.println(" ");
    
    c = esq_dir(a,b);
    Serial.print("valor_de_c=");
    Serial.print(c);
    Serial.println(" ");
       
    te1 = teta(X2, Y2, LAT_D, LON_D, X1, Y1);
    Serial.print("teta_1=");
    Serial.print(te1,7);
    Serial.println(" ");
    
    girar(c, v);
    andar();

for (unsigned long start = millis(); millis() - start < 100;)
  {
    while (Serial1.available())
    { 
      char C = Serial1.read();
        if (gps.encode(C)) // Did a new valid sentence come in?
          newData = true;
    }
  }  
    gps.f_get_position(&latitude, &longitude);//, &fix_age);
    Y3=latitude;
    X3=longitude; 
    Serial.print("valor_de_y3=");
    Serial.print(Y3, 6);
    Serial.print("valor_de_x3=");
    Serial.print(X3, 6); 
     
    te2 = teta(X3, Y3, LAT_D, LON_D, X2, Y2);
    Serial.print("teta_2=");
    Serial.print(te2,7);   
    Serial.println(" ");
    
    v = verificacao(te1, te2, v);
    Serial.print("tempo_de_giro=");
    Serial.print(v);
    Serial.println(" ");
    
    }
  }
}

void set_gps()
{   
  gps.f_get_position(&lat_gps, &lon_gps);//, &age);
  lat_gps == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : lat_gps, 6;
  lon_gps == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : lon_gps, 6;
}


void andar()
{
    delay(10);
    digitalWrite(3,HIGH);
    digitalWrite(2,LOW);
    digitalWrite(5,HIGH);
    digitalWrite(4,LOW);  
    delay(1500);
    digitalWrite(2,LOW);
    digitalWrite(3,LOW);
    digitalWrite(5,LOW);
    digitalWrite(4,LOW);
    delay(50);
}

int P(float LAT_dest,float LON_dest,float y2,float x2)
{    int p;

    if(x2==LON_dest && y2>LAT_dest)
    {
     p=1;      
    }
    else if(x2==LON_dest && y2<LAT_dest)
    {
     p=4;      
    }
    else if(x2>LON_dest && y2>LAT_dest)
    {
     p=2;      
    }
     else if(x2<LON_dest && y2<LAT_dest)
    {
     p=6;      
    }
     else if(x2>LON_dest && y2<LAT_dest)
    {
     p=5;      
    }
     else if(x2<LON_dest && y2>LAT_dest)
    {
     p=3;      
    }
      else if(x2>LON_dest && y2==LAT_dest)
    {
     p=7;      
    }
     else if(x2<LON_dest && y2==LAT_dest)
    {
     p=8;      
    }
    else
    {
      p=9;
    }
    return p;
}

int D(float LAT_D,float LON_D,float x,float y)
{   float LON_dest=LON_D; 
    float LAT_dest=LAT_D;
    int d;

    if((x==LON_dest) && (y<LAT_dest))
    {
     d=1;      
    }
    else if((x==LON_dest) && (y>LAT_dest))
    {
     d=4;      
    }
    else if((x>LON_dest) && (y<LAT_dest))
    {
     d=3;      
    }
     else if((x>LON_dest) && (y>LAT_dest))
    {
     d=6;      
    }
     else if((x<LON_dest) && (y>LAT_dest))
    {
     d=5;      
    }
     else if((x<LON_dest) && (y<LAT_dest))
    {
     d=2;      
    }
      else if((x<LON_dest) && (y==LAT_dest))
    {
     d=7;      
    }
     else if((x>LON_dest) && (y==LAT_dest))
    {
     d=8;      
    }
    else
    {
     d=9;
    }
    return d;
}

int esq_dir(int p,int d)
{ int h;
  if(d==p){h=0;}
  if((d==1 ) && (p==2)){h=1;}
  if((d==1 ) && (p==3)){h=2;}
  if((d==1 ) && (p==4)){h=2;}
  if((d==1 ) && (p==5)){h=1;}
  if((d==1) && (p==6)){h=2;}
  if((d==1 ) && (p==7)){h=1;}
  if((d== 1) && (p==8)){h=2;}
  if((d== 1) && (p==9)){h=0;}
  if((d== 2) && (p==1)){h=2;}
  if((d== 2) && (p==3)){h=2;}
  if((d== 2) && (p==4)){h=1;}
  if((d== 2) && (p==5)){h=1;}
  if((d== 2) && (p==6)){h=2;}
  if((d== 2) && (p==7)){h=1;}
  if((d== 2) && (p==8)){h=2;}
  if((d== 2) && (p==9)){h=0;}
  if((d== 3) && (p==1)){h=1;}
  if((d== 3) && (p==2)){h=1;}
  if((d== 3) && (p==4)){h=2;}
  if((d== 3) && (p==5)){h=2;}
  if((d== 3) && (p==6)){h=2;}
  if((d== 3) && (p==7)){h=1;}
  if((d== 3) && (p==8)){h=2;}
  if((d== 3) && (p==9)){h=0;}
  if((d== 4) && (p==1)){h=2;}
  if((d== 4) && (p==2)){h=2;}
  if((d== 4) && (p==3)){h=1;}
  if((d== 4) && (p==5)){h=2;}
  if((d== 4) && (p==6)){h=1;}
  if((d==4) && (p==7)){h=2;}
  if((d==4) && (p==8)){h=1;}
  if((d== 4) && (p==9)){h=0;}
  if((d==5) && (p==1)){h=2;}
  if((d==5) && (p==2)){h=2;}
  if((d==5) && (p==3)){h=1;}
  if((d==5) && (p==4)){h=1;}
  if((d==5) && (p==6)){h=1;}
  if((d==5) && (p==7)){h=2;}
  if((d==5) && (p==8)){h=1;}
  if((d== 5) && (p==9)){h=0;}
  if((d==6) && (p==1)){h=1;}
  if((d==6) && (p==2)){h=2;}
  if((d==6) && (p==3)){h=1;}
  if((d==6) && (p==4)){h=2;}
  if((d==6) && (p==5)){h=2;}
  if((d==6) && (p==7)){h=2;}
  if((d==6) && (p==8)){h=1;}
  if((d==6) && (p==9)){h=0;}
  if((d==7) && (p==1)){h=2;}
  if((d==7) && (p==2)){h=2;}
  if((d==7) && (p==3)){h=2;}
  if((d==7) && (p==4)){h=1;}
  if((d==7) && (p==5)){h=1;}
  if((d==7) && (p==6)){h=1;}
  if((d==7) && (p==8)){h=2;}
  if((d==7) && (p==9)){h=0;}
  if((d==8) && (p==1)){h=1;}
  if((d==8) && (p==2)){h=1;}
  if((d==8) && (p==3)){h=1;}
  if((d==8) && (p==4)){h=2;}
  if((d==8) && (p==5)){h=2;}
  if((d==8) && (p==6)){h=2;}
  if((d==8) && (p==7)){h=2;}
  if((d==8) && (p==9)){h=0;}
  if((d==9) && (p==9)){h=0;}
  return h;
}

float teta(float x2, float y2,float LAT_D,float LON_D,float x,float y)
{
 float k1=(LON_D-x);
 float k2=(x2-x);
 float w1=(LAT_D-y);
 float w2=(y2-y);
 float teta1 = (180/3.14)*acos(((k1*k2)+(w1*w2))/((sqrt(pow(k1,2) + pow(w1,2)))*(sqrt(pow(k2,2) + pow(w2,2)))));  
return teta1;
}

void girar(int h,int t)
{
  if(h==1)
  {
  delay(t);
  digitalWrite(2,HIGH); //2 e 3 motor da direita
  digitalWrite(3,LOW);
  digitalWrite(5,HIGH);  // 3 e 4 motor da esquerda
  digitalWrite(4,LOW);
  delay(t);
  digitalWrite(2,LOW);
  digitalWrite(3,LOW);
  digitalWrite(5,LOW);
  digitalWrite(4,LOW);
  delay(t);
  }
  else if(h==2)
  {
    delay(t);
    digitalWrite(3,HIGH);
    digitalWrite(2,LOW);
    digitalWrite(4,HIGH);
    digitalWrite(5,LOW);  
    delay(t);
    digitalWrite(2,LOW);
    digitalWrite(3,LOW);
    digitalWrite(5,LOW);
    digitalWrite(4,LOW);
    delay(t);
  }
}

int verificacao(float teta1,float teta2,int t )
  {
    if(abs(teta1-teta2)<90)
    {
      if(teta2<teta1)
    { 
      t=t-10;
    }
    else if(teta2>teta1)
    {
     t=t+20;
    }
  }
    return t;
  } 
