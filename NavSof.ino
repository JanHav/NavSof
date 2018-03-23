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

// Variabelen die de parsed data bijhouden
float xhuidig_wp = 0.0;
float yhuidig_wp = 0.0;

boolean newData = false;

// Global variables definition
int waypoints;            // waypoint counter 
byte current_wp=0;          // Bevat de index waarmee we in de array van de waypoints in de header file geraken
float xstart_wp = 0.0;    // variabele die x-coördinaat bevat voor begin trackline
float ystart_wp = 0.0;    // variabele die y-coördinaat bevat voor begin trackline
float ErrorTrack = 0.0;   // Variabele die de afgeweken afstand tot de trackline bevat
float RicoHuidigeKoers = 0;
float RicoTrackKoers = 0;
float AfstandTotWaypoint = 0;


// Flag variables
byte jumplock_wp = 0;   //When switching waypoints this lock will allow only one transition..
byte beginlock_wp = 1;  //Dit lock moet ervoor zorgen dat in het begin een trackline kan 
                        //gevormd worden door de trackline op te stellen van het huidig punt tot het volgende waypoint

// Arduino Startup
void setup()
{  
  waypoints = sizeof(wps) / sizeof(XasYas); //Berekenen van het aantal waypoints in onze huidige array
  
  Serial.begin(9600);                     //De baudrate van de seriële monitor instellen

  Serial.println("Geef twee coördinaten in die de huidige locatie bepalen (x,y)");      //We vragen de gebruiker de coördinaten van de huidige locatie door te geven
  Serial.println("Enter data in this style <20.35,36.54>  ");
  Serial.println();
}


// Arduino main loop
void loop()
{
  recvWithStartEndMarkers();                                                            //Code voor het binnnenlezen van userinput
    if (newData == true) {                                                              //This temporary copy is necessary to protect the original data
        strcpy(tempChars, receivedChars);                                               //Because strtok() used in parseData() replaces the commas with \0
        parseData();                                                                    //Deze functie haalt de data gescheiden door komma's uit elkaar
        showParsedData();                                                               //Deze functie zorgt ervoor dat we via de serial monitor zien wat we ingegeven hebben
        newData = false;
    }
    
  if (beginlock_wp == 0x01)     //Ervoor zorgen dat de eerste trackline berekend kan worden door het startwaypoint te voorzien van de huidige locatie
  {
    xstart_wp = xhuidig_wp;     //Het allereerste punt kunnen we niet opslaan in een array omdat deze bepaald wordt door de plaats waar de AGV wordt neergezet
    ystart_wp = yhuidig_wp;
    beginlock_wp = 0x00;        //Nadat de xstart_wp en ystart_wp zijn upgedatet met de huidige locatie, mag dit vervolgens niet meer zo gebeuren,daarom dat we ons lock resetten
                                //nu is het de bedoeling dat xstart_wp en ystart_wp steeds verwijzen naar het laatste eindwaypoint, zodat de trackline correct kan berekend worden
  }

  ErrorTrack = AfstandPuntRechte(xstart_wp, ystart_wp,wps[current_wp].Xas, wps[current_wp].Yas, xhuidig_wp, yhuidig_wp);  //In deze variabele wordt de afwijking van de trackline opgeslaan
  RicoHuidigeKoers = Rico(xstart_wp, ystart_wp,wps[current_wp].Xas, wps[current_wp].Yas);                                 //Rico huidige koers
  RicoTrackKoers= Rico(xhuidig_wp, yhuidig_wp,wps[current_wp].Xas, wps[current_wp].Yas);                                  //Rico tracklijn 
  AfstandTotWaypoint = AfstandPuntPunt(xhuidig_wp, yhuidig_wp,wps[current_wp].Xas, wps[current_wp].Yas);                  //Afstand tot volgende waypoint

  if (AfstandTotWaypoint < 0.2)
  {
    current_wp++;
    xstart_wp = wps[current_wp-1].Xas;
    ystart_wp = wps [current_wp-1].Yas;
  }
  


//De software moet oorsprong_wp en doel_wp updaten, ze mogen elk één waypoint verspringen
  
} // end loop ()

