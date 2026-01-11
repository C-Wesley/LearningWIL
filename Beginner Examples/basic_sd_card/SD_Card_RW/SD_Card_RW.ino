/* Wesley Cooke 01/23/2025
 *   Referencing this guide by arudino: https://www.arduino.cc/en/Tutorial/LibraryExamples/ReadWrite
 *   
 */

// We are using the SPI protocol 
// MOSI (COPI) - Communications from the microcontroller to the SPI device
// Main Out Sub In 
// MISO (CISO) - Communications from the SPI device to the microcontroller
// Main In Sub Out
// SCK - Serial Clock. Tells MOSI and MISO when to do something.
// CS  - Chip Select. What device am I talking to
#include <SPI.h> 
#include <SD.h>

// Declare a file object
File ourFile;
int   CS = 5; 
float pi = 3.14; 
long  MilliesTrainBudget = 2147483647;  

void setup() 
{
  Serial.begin(9600);
  // Wait for the Serial Port to open -> Wait for you to open the Serial Monitor.
  // Careful! You only want this if you are plugging into your laptop.
  // If you leave this in your robot code, it will sit there and do nothing...
  // because it's waiting for a serial port to open!
  while(!Serial) {} 

  Serial.println("Breaking News! After the recent snow storms, there are mass breakouts of #CPE!");


  // CS is an output pin. 
  pinMode(CS, OUTPUT); 
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.println("Attempting to init SD card...");
  // Continue trying to open the SD card. 
  while( !SD.begin(CS))
  {
    Serial.println("Failed to init SD card. Retrying in 5s.");
    digitalWrite(LED_BUILTIN, HIGH);
    delay(5000);

    // What happens if you use an "If" statement instead of a while? 
    // What line would you need to add here to stop the program?
    // answer: while(1); // An infinite while loop will stop the program.
  }
  // If we make it to here, the SD card works!
  Serial.println("The party is about to get started!");

  // Check if a file exists and delete it if it does.
  if (SD.exists("data.txt"))
  {
    Serial.println("Data.txt already exsits...");
    Serial.println("Removing it!");
    SD.remove("data.txt");
  }

  // Open the file in Write Mode
  ourFile = SD.open("data.txt", FILE_WRITE); // Careful with your file names. Sometimes, names that are too long don't work. I don't know why. 

  // Check to make sure the file actually opened
  if (ourFile)
  {
    Serial.println("Writing Haugerisms...");

    ourFile.println("Haugerisims");
    // Notice the deliberate choice of println at the end of the line. 
    ourFile.print("#StandBy"); ourFile.print(", "); ourFile.println("#GetSome");
    ourFile.print("#CPE");     ourFile.print(", "); ourFile.println("Chronic Physics Enthusiasm");
    ourFile.println("#Agincourt");

    ourFile.println(pi);
    ourFile.print("Millie's Train Budget: "); ourFile.println(MilliesTrainBudget);
    ourFile.close();
  }
  else
  {
    Serial.println("There was a problem writing...");
  }

  // Open our file to read it. 
  ourFile = SD.open("data.txt", FILE_READ);
  if (ourFile)
  {
    Serial.println("------------");
    while(ourFile.available())
    {
      Serial.write(ourFile.read());
    }
    ourFile.close(); 
  }
  else
  {
    Serial.println("There was a problem reading...");
  }
}

void loop() 
{

}
