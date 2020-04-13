#include "debouncebutton.h"

struct DebounceButton toggleButton = {10, HIGH, false, HIGH, 0};
struct DebounceButton momentaryButton = {11, HIGH, false, HIGH, 0};
const int ledPin = 13;
const unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

// Variables will change:
int ledState = LOW;         // the current state of the output pin


void setupButton(struct DebounceButton *button) {
    pinMode(button->pin, INPUT_PULLUP);
}


boolean isPressed(int pinValue) {
    return pinValue == LOW;
}

void printButtonState(struct DebounceButton *button) {
    Serial.print("Pin: ");
    Serial.print(button->pin);
    Serial.print("; Stable state: ");
    Serial.print(button->stableState);
    Serial.print("; Has new stable state: ");
    Serial.print(button->hasNewStableState);
    Serial.print("; Last read state: ");
    Serial.print(button->lastReadState);
    Serial.print("; Last debounce time: ");
    Serial.println(button->lastDebounceTime);
}

void setup() {
    Serial.begin(115200);

    setupButton(&toggleButton);
    setupButton(&momentaryButton);
    pinMode(ledPin, OUTPUT);

    // set initial LED state
    digitalWrite(ledPin, ledState);
}

void loopButton(struct DebounceButton *button) {
    button->hasNewStableState = false;
    int newButtonState = digitalRead(button->pin);

    // check to see if you just pressed the button
    // (i.e. the input went from LOW to HIGH), and you've waited long enough
    // since the last press to ignore any noise:

    // If the switch changed, due to noise or pressing:
    if (newButtonState != button->lastReadState) {
        // reset the debouncing timer
        button->lastDebounceTime = millis();
    }

    if ((millis() - button->lastDebounceTime) > debounceDelay) {
        // whatever the reading is at, it's been there for longer than the debounce
        // delay, so take it as the actual current state:
        if (newButtonState != button->stableState) {
            button->stableState = newButtonState;
            button->hasNewStableState = true;
        }
    }

    // save the reading. Next time through the loop, it'll be the lastButtonState:
    button->lastReadState = newButtonState;
}


void loop() {
    loopButton(&toggleButton);
    //printButtonState(&toggleButton);
    if (toggleButton.hasNewStableState && isPressed(toggleButton.stableState)) {
        ledState = !ledState;
        digitalWrite(ledPin, ledState);
    }

    loopButton(&momentaryButton);
    printButtonState(&momentaryButton);
    if (momentaryButton.hasNewStableState) {
        if (isPressed(momentaryButton.stableState)) {
            ledState = HIGH;
        } else {
            ledState = LOW;
        }
        digitalWrite(ledPin, ledState);
    }
}
