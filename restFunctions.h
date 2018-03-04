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

// Input string will be [bool isRainbow], [int backgroundR], [int backgroundG], [int backgroundB], [int foregroundR], [int foregroundG], [int foregroundB]
// If isRainbow is 1, foreground will be set but ignored and if background is not defined it will be black
// If isRainbow is 0, and nothing is defined, it will be blue text on black background
// This does not check every individual value, it does assume that an RGB value will be set completely or not at all
int switchTextStyle (String args) { 
    int isRainbow = -1;
    int bg_red = -1;
    int bg_grn = -1;
    int bg_blu = -1;
    int fg_red = -1;
    int fg_grn = -1;
    int fg_blu = -1;
    
    int isRainbow_End = args.indexOf(',');
    
    if (isRainbow_End == -1) { //if one number, no commas, use as style
        isRainbow = args.toInt();
        
        bg_red = 0;
        bg_grn= 0;
        bg_blu = 0;
        fg_red= 0;
        fg_grn = 0;
        fg_blu = 255;
    } else { // if there was a comma after the first number, background is defined
        isRainbow = args.substring(0, isRainbow_End).toInt();
        
        Particle.publish("bkgd", "set", PRIVATE);
        int bg_R_End = args.indexOf(',', isRainbow_End+1);
        int bg_G_End = args.indexOf(',', bg_R_End+1);
        int bg_B_End = args.indexOf(',', bg_G_End+1);
        
        bg_red = args.substring(isRainbow_End+1, bg_R_End).toInt();
        bg_grn = args.substring(bg_R_End+1, bg_G_End).toInt();
        bg_blu = args.substring(bg_G_End+1, bg_B_End).toInt();

        if (bg_B_End != -1) { // if there was a comma after Bkgd G value, foreground numbers defined
            int fg_R_End = args.indexOf(',', bg_B_End+1);
            int fg_G_End = args.indexOf(',', fg_R_End+1);
            int fg_B_End = args.indexOf(',', fg_G_End+1);
    
            fg_red = args.substring(bg_B_End+1, fg_R_End).toInt();
            fg_grn = args.substring(fg_R_End+1, fg_G_End).toInt();
            fg_blu = args.substring(fg_G_End+1, fg_B_End).toInt();
        }   
    }

    // This could be accomplished by a direct assignment from isRainbow to textStyle right now, but leave this here in case there are more than two modes
    // isRainbow will always be set to something
    if (isRainbow == TXT_RAINBOW) {
        textStyle =  TXT_RAINBOW;
    }  else {
        textStyle = TXT_NORMAL;
    }
    
    // only set colors that are at or above 0 for all 3 values
    if (fg_red >= 0 && fg_grn >= 0 && fg_blu >= 0) {
        textFgColor.red = fg_red;
        textFgColor.green = fg_grn;
        textFgColor.blue = fg_blu;
    }
    
    if (bg_red >= 0 && bg_grn >= 0 && bg_blu >= 0) {
        textBgColor.red = bg_red;
        textBgColor.green = bg_grn;
        textBgColor.blue = bg_blu;
    }
    return 1;
}