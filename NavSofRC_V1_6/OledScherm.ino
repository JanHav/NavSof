/*****
Doel: Weergave van de navigatieberekening tijdens testen
Parameters:
void
Return value:
geen
*****/

void Scherm()
{
 u8g.setFont(u8g_font_04b_03r);
 u8g.drawStr(5,10, "TE: ");
 u8g.setPrintPos(30,10);
 u8g.print(ErrorTrack);
 
 
 u8g.drawStr(5,20,"RT: ");
 u8g.setPrintPos(30,20);
 u8g.print(HuidigeKoers);
 
 u8g.drawStr(5,30, "RH: ");
 u8g.setPrintPos(30,30);
 u8g.print(TrackKoers);

 u8g.drawStr(5,40, "dP: ");
 u8g.setPrintPos(30,40);
 u8g.print(AfstandTotWaypoint);
 
 u8g.drawStr(5,50, "HT: ");
 u8g.setPrintPos(30,50);
 u8g.print(TestHoek);
 
 u8g.drawStr(60,10, "Afs: ");
 u8g.setPrintPos(90,10);
 u8g.print(afstand);

 u8g.drawStr(60,20, "dutyS: ");
 u8g.setPrintPos(100,20);
 u8g.print(GewensteDutyCycleServo);

 u8g.drawStr(60,30, "dutyA: ");
 u8g.setPrintPos(100,30);
 u8g.print(GewensteDutyCycle);

 u8g.drawStr(80,40, "X: ");
 u8g.setPrintPos(90,40);
 u8g.print(xhuidig_wp);

 u8g.drawStr(80,50, "Y: ");
 u8g.setPrintPos(90,50);
 u8g.print(yhuidig_wp);
  
}

