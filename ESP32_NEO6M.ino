#include <TinyGPS++.h>
#include <HardwareSerial.h>

#define RXD2 16
#define TXD2 17
#define LED_PIN 2  // Usaremos el LED integrado en la mayoría de placas ESP32

HardwareSerial neogps(1);
TinyGPSPlus gps;

void setup() 
{
  Serial.begin(115200);
  neogps.begin(9600, SERIAL_8N1, RXD2, TXD2); 
  pinMode(LED_PIN, OUTPUT);
  Serial.println("Iniciando receptor GPS...");
}

void loop() 
{
  boolean newData = false;
  
  // Leer datos durante 1 segundo
  for (unsigned long start = millis(); millis() - start < 1000;)
  {
    while (neogps.available())
    {
      if (gps.encode(neogps.read()))
      {
        newData = true;         
      }
    }
  }  

  if(newData)
  {
    Visualizacion_Serial();
    // Si hay datos válidos, LED encendido fijo
    digitalWrite(LED_PIN, HIGH);
  }
  else
  {
    Serial.println("Esperando señal GPS...");
    // Si no hay señal, LED parpadeando
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    delay(500);
  }
}

void Visualizacion_Serial(void)
{ 
  if (gps.location.isValid())
  {  
    Serial.print("Lat: ");
    Serial.print(gps.location.lat(), 6);
    Serial.print(", Lon: ");
    Serial.println(gps.location.lng(), 6);  
    
    Serial.print("Velocidad: ");
    Serial.print(gps.speed.kmph());
    Serial.println(" km/h");    
    
    Serial.print("Satélites: ");
    Serial.println(gps.satellites.value());
    
    Serial.print("Altitud: ");   
    Serial.print(gps.altitude.meters(), 0);
    Serial.println(" m");

    Serial.print("Fecha: ");
    Serial.print(gps.date.day());
    Serial.print("/");
    Serial.print(gps.date.month());
    Serial.print("/");
    Serial.println(gps.date.year());

    Serial.print("Hora (UTC): ");
    if(gps.time.hour() < 10) Serial.print("0");
    Serial.print(gps.time.hour());
    Serial.print(":");
    if(gps.time.minute() < 10) Serial.print("0");
    Serial.print(gps.time.minute());
    Serial.print(":");
    if(gps.time.second() < 10) Serial.print("0");
    Serial.println(gps.time.second());
    
    Serial.println("---------------------------");
  }
  else
  {
    Serial.println("Señal GPS recibida pero sin fix de posición");
    Serial.print("Satélites visibles: ");
    Serial.println(gps.satellites.value());
    // LED parpadeando rápidamente cuando hay señal pero no fix
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    delay(250);
  }  
}