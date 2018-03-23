/* Gebasseerd op software geschreven door Chris Anderson en Jordi Munoz, aangepast door Jan Haverbeke om te gebruiken voor AGV's */
/* Nov/27/2010
/* Version 1.1 */
/* Released under an Apache 2.0 open source license*/
/* Project home page is at DIYdrones.com (and ArduPilot.com)
/* We hope you improve the code and share it with us at DIY Drones!*/


#include "waypoints.h"

//User input variables
const byte numChars = 32;
char receivedChars[numChars];
char tempChars[numChars];        // temporary array for use when parsing

// variables to hold the parsed data
float xhuidig_wp = 0.0;
float yhuidig_wp = 0.0;

boolean newData = false;


// Global variables definition
int waypoints;  // waypoint counter 


// Flag variables
byte jumplock_wp = 0; // When switching waypoints this lock will allow only one transition..


// Arduino Startup, entry point after power-on
void setup()
{  
  waypoints = sizeof(wps) / sizeof(XasYas); //Berekenen van het aantal waypoints in onze huidige array
  Serial.begin(9600);                     //De baudrate van de seriële monitor instellen

  //Setup code voor user input
  Serial.println("Geef twee coördinaten in die de huidige locatie bepalen (x,y)");
  Serial.println("Enter data in this style <20.35,36.54>  ");
  Serial.println();
}

// Program main loop starts here

// Arduino main loop
void loop()
{
  //Serial.println(waypoints);      //Berekent hoeveel waypoints onze huidige route bevat

  //Code voor het binnnenlezen van userinput
 recvWithStartEndMarkers();
    if (newData == true) {
        strcpy(tempChars, receivedChars);
            // this temporary copy is necessary to protect the original data
            //   because strtok() used in parseData() replaces the commas with \0
        parseData();
        showParsedData();
        newData = false;
    }
  //De software moet oorsprong_wp en doel_wp updaten, ze mogen elk één waypoint verspringen
  
} // end loop ()

