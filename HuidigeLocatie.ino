
//============

void recvWithStartEndMarkers() {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;

    while (Serial.available() > 0 && newData == false) {
        rc = Serial.read();

        if (recvInProgress == true) {
            if (rc != endMarker) {
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= numChars) {
                    ndx = numChars - 1;
                }
            }
            else {
                receivedChars[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                ndx = 0;
                newData = true;
            }
        }

        else if (rc == startMarker) {
            recvInProgress = true;
        }
    }
}

//============

void parseData() {      // split the data into its parts

    char * strtokIndx; // this is used by strtok() as an index

    strtokIndx = strtok(tempChars,",");      // get the first part - the string
    xhuidig_wp = atof(strtokIndx); // copy it to messageFromPC
 
    strtokIndx = strtok(NULL, ",");
    yhuidig_wp = atof(strtokIndx);     // convert this part to a float

}

//============

void showParsedData() {
    Serial.print("Huidige x-coördinaat ");
    Serial.println(xhuidig_wp);
    Serial.print("Huidige y-coördinaat ");
    Serial.println(yhuidig_wp);
    Serial.println(wps[2].Xas);             //Array aanspreken uit header file die coördinaten waypoints bevat
    Serial.print("De error op de track is ");
    Serial.println(ErrorTrack);
    Serial.print("Rico huidige koers ");
    Serial.print(RicoHuidigeKoers);
    Serial.print("Rico trackline ");
    Serial.print(RicoTrackKoers);
    Serial.print("Afstand tot waypoint ");
    Serial.println(AfstandTotWaypoint);
}
