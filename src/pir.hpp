#ifndef PIR_H
#define PIR_H

#ifndef PIR_PIN
#define PIR_PIN 27
#endif

typedef struct PirData {
    bool initialized = false;
    long lastDetectionTime = 0L;
} PirData_t;

void (*callback)() = nullptr;

void IRAM_ATTR detectsMovement() {
#ifdef DEBUG
        USE_SERIAL.println(F("[PIR] Motion detected"));
#endif
    if(callback != nullptr)
    {
        (*callback)();
    }
}

void setupPir(void (*callbackFunction)() ) {
    callback = callbackFunction;
    // PIR Motion Sensor mode INPUT_PULLUP
    pinMode(PIR_PIN, INPUT_PULLUP);
    // Set motionSensor pin as interrupt, assign interrupt function and set RISING mode
    attachInterrupt(digitalPinToInterrupt(PIR_PIN), detectsMovement, RISING);
}

#endif