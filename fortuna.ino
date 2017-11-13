/**
 * @file     fortuna.ino
 * @author   Phi Luu
 * @date     March 13, 2016
 *
 * @brief    Robot Collection - Project 01: Fortuna
 *
 * @section  DESCRIPTION
 *
 * This robot bounces forward by two motors on its left and right wings. It
 * automatically beeps and flashes LEDs while bouncing. The robot also
 * changes the behaviors based on the amount of light received by the two
 * photocells.
 */

// Required hardware I/O connections
const int LEFT_PHOTO = A0;     // left photocell connected to A0
const int MIC_INPUT = A3;      // microphone connected to A3
const int RIGHT_PHOTO = A5;    // right photocell connected to A5
const int BUZZER = 2;          // buzzer connected to 2
const int LEFT_MOTOR = 5;      // left motor connected to ~5
const int RIGHT_MOTOR = 10;    // right motor connected to ~10

// Global constants
const int MOTOR_POWER = 255;   // 50% motor power
const int MIC_THRESHOLD = 550; // mic threshold, varies 0-1023

void setup() {
    pinMode(LEFT_MOTOR, OUTPUT);
    pinMode(RIGHT_MOTOR, OUTPUT);
    pinMode(BUZZER, OUTPUT);
}

void loop() {
    // turn off both motors
    analogWrite(LEFT_MOTOR, 0);
    analogWrite(RIGHT_MOTOR, 0);

    // beep a bunch of times
    for (int i = 1; i < 5; i++) {
        Beep(BUZZER, 1000, 100 * i);
        delay(100 * i);
    }

    // listen to the microphone for ~100 ms and get the loudest value
    int val;
    int maxVal = 0;

    for (int i = 1; i < 100; i++) {
        val = analogRead(MIC_INPUT);

        if (val > maxVal) {
            maxVal = val;
        }
        delay(1);
    }

    // if the largest voltage detected is above 1.77 V (3.3*550/1024),
    // commence the "beep dance" response
    if (maxVal > MIC_THRESHOLD) {
        // make the "siren" noise by alternating 1200 Hz and 800 Hz tones
        for (int i = 0; i < 5; i++) {
            Beep(BUZZER, 1200, 100);
            Beep(BUZZER, 800, 100);
        }

        // shake motors back and forth rapidly
        for (int i = 0; i < 3; i++) {
            analogWrite(RIGHT_MOTOR, MOTOR_POWER);
            delay(200);
            analogWrite(RIGHT_MOTOR, 0);
            analogWrite(LEFT_MOTOR, MOTOR_POWER);
            delay(200);
            analogWrite(LEFT_MOTOR, 0);
        }

        // make a series of tones with increasing frequency from 300-100 Hz
        for (int i = 30; i < 100; i += 1) {
            Beep(BUZZER, 10 * i, 10);
        }

        // then come back down
        for (int i = 100; i > 30; i -= 1) {
            Beep(BUZZER, 10 * i, 10);
        }
    }

    // read the status of photocells and adjust motor output
    // delay briefly to allow comparator outputs to settle
    delayMicroseconds(20);

    // check each photocell/circuit output and determine
    if (digitalRead(LEFT_PHOTO)) {
        analogWrite(LEFT_MOTOR, MOTOR_POWER); // whether to run on the left motor...
    }

    if (digitalRead(RIGHT_PHOTO)) {           // ...or right motor
        analogWrite(RIGHT_MOTOR, MOTOR_POWER);
    }
    delay(500);                               // wait 500ms
}

/**
 * Generates a square wave at a given frequency for ms milliseconds
 *
 * @param  pin      the pin number corresponding to the buzzer
 * @param  freq     the frequency of the sound wave
 * @param  ms       duration
 */
void Beep(int pin, int freq, long ms) {
    long semi_period = (long)(1000000 / (freq * 2));
    long num_loops = (long)((ms * 1000) / (semi_period * 2));

    for (int k = 0; k < num_loops; k++) {
        digitalWrite(pin, HIGH);        // set buzzer pin high
        delayMicroseconds(semi_period); // for half of the period
        digitalWrite(pin, LOW);         // set buzzer pin low
        delayMicroseconds(semi_period); // for the other half of the period
    }
}
