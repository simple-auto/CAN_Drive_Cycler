/*
Codigo base para Proyecto Plataforma CAN

-----------------------------------------------------------------------------
Author : Matus Icaza, matusicaza@gmail.com
File   : CANDriveCycler_v0.ino
Create : 2017-06-21
Editor : Arduino IDE and Arduino Create (Online Editor)
 -----------------------------------------------------------------------------
*/

/*
 * References
 * 
 * Examples>Button
 * Sparkfun CAN Bus Shield Examples (SD, CAN Read)
 * SIM808 documentation (Hard to find, acknowledgements to Nicolas Hasbun (nhasbun@gmail.com)
 */


#include <SoftwareSerial.h>
#include <SPI.h>
#include <SD.h>



SoftwareSerial mySerial(3,4);
int baudRate = 9600;

// Chip Select pin is tied to pin 9 on the SparkFun CAN-Bus Shield
const int chipSelect = 9;  

const int buttonPin = 5;     // the number of the pushbutton pin
int buttonState = 0;         // variable for reading the pushbutton status



char* nombre_archivo = "data.txt"; //cambiar a nombre con fecha



int analogPin = A5;
int sensorVal=0;

void setup(){
  pinMode(buttonPin, INPUT);
  mySerial.begin(baudRate);              
  Serial.begin(baudRate);     
  delay(2000);
  
  Serial.print("Montar SD...");
  // make sure that the default chip select pin is set to
  // output, even if you don't use it:
  pinMode(chipSelect, OUTPUT);
  
  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Error");
    // don't do anything more:
    return;
  }
  Serial.println("OK");
  File dataFile = SD.open(nombre_archivo, FILE_WRITE); 
  if (dataFile)   {  
    dataFile.print("Sesion 30 Junio\n//datos de inicio como: hora, ubicacion inicial, ...\n\ntime\t\tveloc\trevos\tgear\n[ms]\t[km/h]\t[rpm]\t[]\n");
    //output also on Serial monitor for debugging
    Serial.print("Sesion 30 Junio\n\n\ntime\t\tveloc\trevos\tgear\n[ms]\t[km/h]\t[rpm]\t[]\n");
  }
} //Fin del Setup

void loop()
{
  buttonState = digitalRead(buttonPin); //Estado del contacto de motor
  if(buttonState == HIGH){
    //Lectura del Bus CAN
  
    //Filtrado
  
    //Registro Sesion

 
    // make a string for assembling the data to log:
    String dataString = "";
    // open the file. note that only one file can be open at a time,
    // so you have to close this one before opening another.
    // this opens the file and appends to the end of file
    // if the file does not exist, this will create a new file.
    long int timeStamp = 0;
    File dataFile = SD.open(nombre_archivo, FILE_WRITE);  
    if (dataFile)   {  
      timeStamp = millis();
      //write to uSD card
      dataFile.print(timeStamp);
      //output also on Serial monitor for debugging
      Serial.print(timeStamp);
    
      //Insertar Datos de Bus CAN

      Serial.print("\t\t20.00\t1200\t2");
      dataFile.print("\t\t20.00\t1200\t2"); //create a new row to read data more clearly
      
      //Serial.print(",");
      dataFile.println(); //create a new row to read data more clearly
      dataFile.close();   //close file
      Serial.println(); //print to the serial port too:
      delay(600);
    }
  } //Fin if(motor encendido)


  else{
    //Reporte
    Serial.println("Cerrando Ciclo");
    //Envio a la Nube
    envio_nube();
  
    Serial.println("(Fin del Ciclo)\n\n");
    delay(1500000); //Cambiar por sleep
  }
} //Fin del Loop


void envio_nube(){
  /*
  enviar_AT("ATD56981360659;");
  delay(10000);
  enviar_AT("ATH");
  */
  enviar_AT("AT");
  enviar_AT("AT+CPIN?");
  enviar_AT("AT+CSTT?");
  enviar_AT("AT+CSTT=\"bam.entelpcs.cl\",\"entelpcs\",\"entelpcs\"");
  enviar_AT("AT+CSTT?");
  enviar_AT("AT+CIICR");
  enviar_AT("AT+CIFSR");

}//Fin funcion envio_nube

//enviar_AT("");
void enviar_AT(String comando){
  mySerial.println(comando);
  Serial.println("Comando AT:\n"+comando);
  delay(1000);
  String respuesta = ""; //(String)(unsigned char)mySerial.read()
  
  while(mySerial.available()){       
    respuesta += (String)(char)(mySerial.read());
  } 
   
  Serial.println("Respuesta:\n"+respuesta);
  delay(100);
}//Fin funcion enviar_AT
