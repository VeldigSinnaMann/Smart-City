#include <Wire.h>
#include <Zumo32U4.h>
Zumo32U4IMU imu;
const float conversion_factor = 0.061; // [mg/LSB] akselerometerlesningene konverteres til enheter av g ved å bruke sensitiviteten
const float g = 8.91;                  // [m/s^2] tyngdeaksellerasjon
float accelerations_m_s2[3];

// Variabler for beregning av gjenomsnittlig akselerasjon de siste x minuttene
const unsigned long UpdateAverageInterval = 1000; // Hvor ofte ny verdi skal legges til i gjenomsnittet som skal beregnes
unsigned long lastUpdateAverageTime= 0; // lagrer sist tid for gjennomsnittsoppdatering
const int bufferlength = 60; // Lagrer 60 sek verd av data
float accelerationBuffer[bufferlength]; // Array for å lagre verdiene det skal tas gj.snitt av
int bufferIndex = 0; // Starter lagring i buffer-arrayet på plass 0 

// Gj,snitt x-akse
float accelXBuffer[bufferlength]; 

//Gj.snitt y-akse
float accelYBuffer[bufferlength];

// Struct til å beregne gj.snittsakselerasjonen til alle aksene
struct AccelerationSums {
    float avgX;  //Gj.snitt x-akse
    float avgY;  //Gj.snitt x-akse
    float avgA;  //Gj.snitt total
};

union FloatUnion {
    float value;
    uint8_t bytes[sizeof(float)];
};

void setup()
{
    Wire.begin();
    Serial.begin(9600);
    if (!imu.init())
    {
        // Failed to detect the compass.
        ledRed(1);
        while (1)
        {
            Serial.println(F("Failed to initialize IMU sensors."));
            delay(100);
        }
    }
    imu.enableDefault();

    for ( int i = 0; i < bufferlength; i++) {
        accelerationBuffer[i] = 0;
    }

}
void loop()
{
    if (millis() - UpdateAverageInterval >= lastUpdateAverageTime) {
        float accelerations_m_s2[3];
        accelerationValues_m_s2(accelerations_m_s2);
        updateAccelerationBuffer(accelerations_m_s2[0],accelerations_m_s2[1],accelerations_m_s2[2]);
        lastUpdateAverageTime = millis();

        AccelerationSums averages = calculateAverageAccel();

        Serial.print(" Average A: "); Serial.println(averages.avgA);

        FloatUnion fu;
        fu.value = averages.avgA; // Lagrer verdien til averages.avgA (gjennomsnitt total aks) som en union(delt opp i bytes)

        Wire.beginTransmission(4);
        Wire.write(fu.bytes, sizeof(float));
        Wire.endTransmission();


    }

}

void accelerationValues_m_s2(float accelerations_m_s2[3])
{
    imu.read();

    float ax = imu.a.x * conversion_factor / 1000; // Accel x i g's
    float ay = imu.a.y * conversion_factor / 1000; // Accel y i g's
    float a = sqrtf((ax * ax) + (ay * ay));        // Acceleration in g's total

    accelerations_m_s2[0] = ax * g; // Accel x i m/s^2
    accelerations_m_s2[1] = ay * g; // Accel y i m/s^2
    accelerations_m_s2[2] = a * g;  // Absolute Accel total i m/s^2
}

void displayWarnings(const AccelerationSums &averages, const float *currentAccels)
{
    const int16_t accelerationTreshold = 2;
    const int16_t averageTreshold = 1;

    if (accelerations_m_s2[0] < -accelerationTreshold)
    {
        Serial.println("You are breaking to hard, try slowing down earlier :)");
    }
    else if (accelerations_m_s2[0] > accelerationTreshold)
    {
        Serial.println("You are speeding too fast, try accelerating slower :)");
    }

    if(averages.avgA > averageTreshold){
        Serial.println("Warning: Average total acceleration exceeds 1 m/s^2!");
        if (averages.avgX < - averageTreshold) {
            Serial.println(" You are generally breaking too hard, try slowing down before stops!");
        } else if (averages.avgX > averageTreshold) {
            Serial.println("You are generally accelerating too fast!");
        }
    }
}

void updateAccelerationBuffer(float newXAcceleration, float newYAcceleration, float newAcceleration){
    // Fyll på arrayene med akselerasjoner
    accelXBuffer[bufferIndex] = newXAcceleration;
    accelYBuffer[bufferIndex] = newYAcceleration;
    accelerationBuffer[bufferIndex] = newAcceleration; 
    // Hopp til neste plass i arrayene
    bufferIndex = (bufferIndex + 1) % bufferlength; // start på nytt dersom nådd 60
}


AccelerationSums calculateAverageAccel(){
    float sumX = 0, sumY = 0, sumA = 0;
    // Opdater plassene i arrayet med akselerasjonsverdier
    for (int i = 0; i < bufferlength ; i++) {
        sumX += accelXBuffer[i];
        sumY += accelYBuffer[i];
        sumA += accelerationBuffer[i];
    }
    AccelerationSums sums;
    sums.avgX = sumX / bufferlength;
    sums.avgY = sumY / bufferlength;
    sums.avgA = sumA / bufferlength;
    // Returner de beregnede gjenomsnittene
    return sums;
}
/*float calculateAverageAccel(){
    float sum = 0;
    for (int i = 0; i < bufferlength; i++){
        sum += accelerationBuffer[i];
    }
    return sum / bufferlength;
}*/
