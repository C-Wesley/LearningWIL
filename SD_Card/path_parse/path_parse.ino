#include <SPI.h> 
#include <SD.h>

File ourFile;
int   CS = 5; 

void setup() 
{
  Serial.begin(9600);
  while(!Serial) {} 

  pinMode(CS, OUTPUT); 
  pinMode(LED_BUILTIN, OUTPUT);

  
  Serial.println("Attempting to init SD card...");
  // Continue trying to open the SD card. 
  while(!SD.begin(CS))
  {
    Serial.println("Failed to init SD card. Retrying in 5s.");
    digitalWrite(LED_BUILTIN, HIGH);
    delay(5000);
  }
  
  ourFile = SD.open("path.csv");

  // While there is still stuff in the file, keep reading it.
  int ln = 1;                         // What line are we on? 
  while (ourFile.available())
  {
    
    char aline[50] = {};              // Step up a place to store the line 
    char data      = ' ';             // It doesn't matter what this is
    int  index     = 0;               // Counting variable 

    // While we aren't about to go to a newline: 
    while(data != '\n')
    {
      // Keep sotring the characters in the array 
      data = ourFile.read();
      aline[index] = data;
      index++; 
    }

    // Once we are here, we need to parse the string.
    if (ln == 1)
    {
      // Header info. We don't need to tokenize this...
      Serial.println(aline); 
    }
    else
    {
      // Line format: 'ID, Longitude, Latitude\n'
      
      char* tok = strtok(aline, ",");    // Read the first token 
      int id  = atoi(tok);              // Save it as an int
      
      tok = strtok(NULL, ",");         // Read the next token
      long longitude= atol(tok);        // Save it as a long
  
      tok = strtok(NULL, ",");          // Read the next token
      long latitude = atol(tok);        // Save it as a long
      
      tok = strtok(NULL, ",");          // Make sure we are done parsing 
      
      if (tok != NULL)
      {
        // Any good error message subtly blames the user.
        Serial.println("Expected NULL but did not see NULL. Ensure parseing matches the line format.");
      }
      Serial.print("id = "); Serial.println(id);
      Serial.print("longitude = "); Serial.println(longitude);
      Serial.print("Latitude = "); Serial.println(latitude);
      Serial.println("----");
    }
    ln++;
  }
}

void loop() 
{

}
