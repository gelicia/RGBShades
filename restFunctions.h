int increaseBrightness (String args) {
    currentBrightness += 51; // increase the brightness (wraps to lowest)
    FastLED.setBrightness(scale8(currentBrightness, MAXBRIGHTNESS));
    return 1;   
}

int resetBrightness (String args) {
    currentBrightness = STARTBRIGHTNESS; // reset brightness to startup value
    FastLED.setBrightness(scale8(currentBrightness, MAXBRIGHTNESS));
    return 1;   
}

int nextMode (String args) {
    if (++currentEffect >= numEffects) currentEffect = 0; // loop to start of effect list
    effectInit = false; // trigger effect initialization when new effect is selected
    return 1;   
}

int autoCycleMode (String args) {
    autoCycle = !autoCycle; // toggle auto cycle mode
    confirmBlink(); // one blue blink: auto mode. two red blinks: manual mode.
    return 1;
}

// immediately replaces the string being displayed
int buildShortString (String args) {
    args.replace("%26", "&");
    displayString = args;
    return 1;
}

// Particle function has a max size for the parameter, this lets you append messages together
// and will politely change them after the previous string is done with a loop
// Although it will be hard to read short novels on the glasses ;)  
int buildString (String args) {
    int adminMsg = args.substring(0, 1).toInt();
    String msg = args.substring(2);
    
    if (adminMsg == 1){
        if (msg == "BEGIN"){
            displayPlaceholder = " ";
        }
        else if (msg == "END"){
            okayToSwitch = 1;
        }
    }
    else {
        //this is handled weird by the API, so it is the ONE exception of getting handled here
        msg.replace("%26", "&");
        displayPlaceholder.concat(" ");
        displayPlaceholder.concat(msg);
    }
    
    return 1;
}

int switchToTextMode (String args) {
    currentEffect = numEffects - 1;
    effectInit = false; // trigger effect initialization when new effect is selected
    return 1;
}