# 1 "C:\\Users\\Aaseb\\OneDrive\\Skrivebord\\Repository\\Zumo-Smart-City\\Zumo-car\\Kjoremonster\\Kjoremonster.ino"
# 2 "C:\\Users\\Aaseb\\OneDrive\\Skrivebord\\Repository\\Zumo-Smart-City\\Zumo-car\\Kjoremonster\\Kjoremonster.ino" 2
# 3 "C:\\Users\\Aaseb\\OneDrive\\Skrivebord\\Repository\\Zumo-Smart-City\\Zumo-car\\Kjoremonster\\Kjoremonster.ino" 2
Zumo32U4IMU imu;
char report[120];
const float conversion_factor = 0.061; // [mg/LSB] akselerometerlesningene konverteres til enheter av g ved å bruke sensitiviteten
const float g = 8.91; // [m/s^2] tyngdeaksellerasjon
const int16_t accelerationTreshold = 2;
float accelerations_m_s2[3];

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
      Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 20 "C:\\Users\\Aaseb\\OneDrive\\Skrivebord\\Repository\\Zumo-Smart-City\\Zumo-car\\Kjoremonster\\Kjoremonster.ino" 3
                    (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 20 "C:\\Users\\Aaseb\\OneDrive\\Skrivebord\\Repository\\Zumo-Smart-City\\Zumo-car\\Kjoremonster\\Kjoremonster.ino"
                    "Failed to initialize IMU sensors."
# 20 "C:\\Users\\Aaseb\\OneDrive\\Skrivebord\\Repository\\Zumo-Smart-City\\Zumo-car\\Kjoremonster\\Kjoremonster.ino" 3
                    ); &__c[0];}))
# 20 "C:\\Users\\Aaseb\\OneDrive\\Skrivebord\\Repository\\Zumo-Smart-City\\Zumo-car\\Kjoremonster\\Kjoremonster.ino"
                    )));
      delay(100);
    }
  }
  imu.enableDefault();
}
void loop()
{

  accelerationValues_m_s2(accelerations_m_s2);
  Serial.print("ax in m/s^2: ");
  Serial.print(accelerations_m_s2[0], 4); // ax component in m/s^2
  Serial.print(", ay in m/s^2: ");
  Serial.print(accelerations_m_s2[1], 4); // ay component in m/s^2
  Serial.print(", Total in m/s^2: ");
  Serial.println(accelerations_m_s2[2], 4); // total vector magnitude in m/s^2

  displayWarnings();

  delay(500);
}

void accelerationValues_m_s2(float accelerations_m_s2[3])
{
  imu.read();

  float ax = imu.a.x * conversion_factor / 1000; // Accel x i g's
  float ay = imu.a.y * conversion_factor / 1000; // Accel y i g's
  float a = sqrtf((ax * ax) + (ay * ay)); // Acceleration in g's total

  accelerations_m_s2[0] = ax * g; // Accel x i m/s^2
  accelerations_m_s2[1] = ay * g; // Accel y i m/s^2
  accelerations_m_s2[2] = a * g; // Accel total i m/s^2
}

void displayWarnings (){
  if (accelerations_m_s2[0] < -accelerationTreshold){
    Serial.println("You are braking to hard, try slowing down earlier");
  } else if (accelerations_m_s2[0] > accelerationTreshold)
  {
    Serial.println("You are speeding too fast, try accelerating slower :)");
  }
}
