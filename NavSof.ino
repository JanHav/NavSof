/* Gebasseerd op software geschreven door Chris Anderson en Jordi Munoz, aangepast door Jan Haverbeke om te gebruiken voor AGV's */
/* Nov/27/2010
/* Version 1.1 */
/* Released under an Apache 2.0 open source license*/
/* Project home page is at carsexplained.com*/

#include "waypoints.h"
#include <math.h>

// Global variables definition
int waypoints;                      // Waypoint teller 
byte current_wp=0;                  // Bevat de index waarmee we in de array van de waypoints in de header file geraken, bepaald eindpunt trackline
byte previous_wp=0;                 // Bevat de index waarmee we in de array van de waypoints in de header file geraken, bepaald beginpunt trackline
float xhuidig_wp = 0.0;             // Bevat de huidige x-coördinaat van de AGV, in demonstratiesoftware verkrijgen we dit via userinput 
float yhuidig_wp = 0.0;             // Bevat de huidige y-coördinaat van de AGV, in demonstratiesoftware verkrijgen we dit via userinput
float xstart_wp = 0.0;              // Bevat de x-coördinaat begin trackline
float ystart_wp = 0.0;              // Bevat de y-coördinaat begin trackline
float ErrorTrack = 0.0;             // Bevat de afgeweken afstand tot de trackline 
float RicoHuidigeKoers = 0;         // De rico van de lijn waarop de AGV nu zit
float RicoTrackKoers = 0;           // De rico van de lijn die de AGV het best volgt
float AfstandTotWaypoint = 0;       // De afstand tot het volgende waypoint
double TrackHoek = 0;               // De hoek tussen de de lijn waarop de AGV nu zit en de lijn die de AGV moet volgen

// Flag variables
byte jumplock_wp = 0;   //Dit lock moet ervoor zorgen dat in het begin een trackline kan 
                        //gevormd worden door de trackline op te stellen van het huidig punt tot het volgende waypoint

// Arduino Startup
void setup()
{  
  waypoints = sizeof(wps) / sizeof(XasYas); //Berekenen van het aantal waypoints in onze huidige array
  
  Serial.begin(115200);                     //De baudrate van de seriële monitor instellen
}


// Arduino main loop
void loop()
{
  Serial.println("Huidige x-coordinaat ");  //De gebruiker naar informatie vragen
  while(Serial.available()<2)               //Wachten tot de gebruiker iets typt
  {};
  xhuidig_wp=Serial.parseFloat();           //De data die de gebruiker heeft ingegeven binnenlezen

  Serial.println("Huidige y-coordinaat ");  //De gebruiker naar informatie vragen
  while(Serial.available()<2)               //Wachten tot de gebruiker iets typt
  {};
  yhuidig_wp=Serial.parseFloat();           //De data die de gebruiker heeft ingegeven binnenlezen
    
  if (jumplock_wp == 0x00)                  //Ervoor zorgen dat de eerste trackline berekend kan worden door het startwaypoint te voorzien van de huidige locatie
  {
    xstart_wp = xhuidig_wp;                 //Het allereerste punt kunnen we niet opslaan in een array omdat deze bepaald wordt door de plaats waar de AGV wordt neergezet
    ystart_wp = yhuidig_wp;                 //daarom bevat xstart en ystart initieel de waarde van xhuidig en yhuidig
    jumplock_wp = 0x01;                     //Nadat de xstart_wp en ystart_wp zijn upgedatet met de huidige locatie, 
                                            //mag dit vervolgens niet meer zo gebeuren,daarom dat we ons lock resetten
                                            //nu is het de bedoeling dat xstart_wp en ystart_wp steeds verwijzen naar het laatste eindwaypoint, 
                                            //zodat de trackline correct kan berekend worden
  }

  ErrorTrack = AfstandPuntRechte(xstart_wp, ystart_wp,wps[current_wp].Xas, wps[current_wp].Yas, xhuidig_wp, yhuidig_wp);  //In deze variabele wordt de afwijking van de trackline opgeslaan
  RicoHuidigeKoers = Rico(xstart_wp, ystart_wp,wps[current_wp].Xas, wps[current_wp].Yas);                                 //Rico huidige koers
  RicoTrackKoers= Rico(xhuidig_wp, yhuidig_wp,wps[current_wp].Xas, wps[current_wp].Yas);                                  //Rico tracklijn 
  AfstandTotWaypoint = AfstandPuntPunt(xhuidig_wp, yhuidig_wp,wps[current_wp].Xas, wps[current_wp].Yas);                  //Afstand tot volgende waypoint
  TrackHoek = HoekTweeRechten(ErrorTrack, AfstandTotWaypoint);                                                            //Hoek tussen huidige trackline en oorspronkelijke trackline berekenen

  if (AfstandTotWaypoint < 0.4)             //Op het ogenblik dat de AGV dicht genoeg bij het eindwaypoint is, moet de trackline 
  {                                         //gevormd worden door het huidige eindwaypoint en een volgend waypoint
    previous_wp = current_wp;
    current_wp++;
    xstart_wp = wps[previous_wp].Xas;
    ystart_wp = wps [previous_wp].Yas;
  }

  //De onderstaande printstatements zijn voor debugging/controle en visualisatie
    Serial.print("Huidige x-coördinaat ");
    Serial.println(xhuidig_wp);
    Serial.print("Huidige y-coördinaat ");
    Serial.println(yhuidig_wp);
    Serial.println(wps[1].Xas);             //Array aanspreken uit header file die coördinaten waypoints bevat
    Serial.print("De error op de track is ");
    Serial.println(ErrorTrack);
    Serial.print("Rico huidige koers ");
    Serial.println(RicoHuidigeKoers);
    Serial.print("Rico trackline ");
    Serial.println(RicoTrackKoers);
    Serial.print("Afstand tot waypoint ");
    Serial.println(AfstandTotWaypoint);
    Serial.print("Trackhoek ");
    Serial.println(TrackHoek);
    Serial.print("Huidig waypoint ");
    Serial.println(current_wp);
    Serial.print("Begin ");
    Serial.println(xstart_wp);  
} // end loop ()

