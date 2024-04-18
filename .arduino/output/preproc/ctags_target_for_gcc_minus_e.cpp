# 1 "C:\\Users\\Aaseb\\OneDrive\\Skrivebord\\Repository\\Zumo-Smart-City\\Zumo-car\\Kjoremonster\\Kjoremonster.ino"
# 2 "C:\\Users\\Aaseb\\OneDrive\\Skrivebord\\Repository\\Zumo-Smart-City\\Zumo-car\\Kjoremonster\\Kjoremonster.ino" 2
# 3 "C:\\Users\\Aaseb\\OneDrive\\Skrivebord\\Repository\\Zumo-Smart-City\\Zumo-car\\Kjoremonster\\Kjoremonster.ino" 2

Zumo32U4IMU imu;

char report[120];
const float conversion_factor = 0.061; // [mg/LSB] akselerometerlesningene konverteres til enheter av g ved å bruke sensitiviteten
const float g = 8.91; // [m/s^2] tyngdeaksellerasjon
float accelerationValues[2]; // Array to hold acceleration values

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
# 22 "C:\\Users\\Aaseb\\OneDrive\\Skrivebord\\Repository\\Zumo-Smart-City\\Zumo-car\\Kjoremonster\\Kjoremonster.ino" 3
                    (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 22 "C:\\Users\\Aaseb\\OneDrive\\Skrivebord\\Repository\\Zumo-Smart-City\\Zumo-car\\Kjoremonster\\Kjoremonster.ino"
                    "Failed to initialize IMU sensors."
# 22 "C:\\Users\\Aaseb\\OneDrive\\Skrivebord\\Repository\\Zumo-Smart-City\\Zumo-car\\Kjoremonster\\Kjoremonster.ino" 3
                    ); &__c[0];}))
# 22 "C:\\Users\\Aaseb\\OneDrive\\Skrivebord\\Repository\\Zumo-Smart-City\\Zumo-car\\Kjoremonster\\Kjoremonster.ino"
                    )));
      delay(100);
    }
  }

  imu.enableDefault();
}

void loop()
{

  calculate_Acceleration_g(accelerationValues); // Calculate acceleration in g and store in array

  float acceleration_m_s2 = calculate_Acceleration();

  // Now print out the acceleration values
  Serial.print("ax: ");
  Serial.print(accelerationValues[0], 4); // Print ax with 4 decimal places
  Serial.print(" g, ay: ");
  Serial.print(accelerationValues[1], 4); // Print ay with 4 decimal places
  Serial.println("g ");

  Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 44 "C:\\Users\\Aaseb\\OneDrive\\Skrivebord\\Repository\\Zumo-Smart-City\\Zumo-car\\Kjoremonster\\Kjoremonster.ino" 3
              (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 44 "C:\\Users\\Aaseb\\OneDrive\\Skrivebord\\Repository\\Zumo-Smart-City\\Zumo-car\\Kjoremonster\\Kjoremonster.ino"
              "Total acceleration (m/s^2): "
# 44 "C:\\Users\\Aaseb\\OneDrive\\Skrivebord\\Repository\\Zumo-Smart-City\\Zumo-car\\Kjoremonster\\Kjoremonster.ino" 3
              ); &__c[0];}))
# 44 "C:\\Users\\Aaseb\\OneDrive\\Skrivebord\\Repository\\Zumo-Smart-City\\Zumo-car\\Kjoremonster\\Kjoremonster.ino"
              )));
  Serial.println(acceleration_m_s2);

  delay(1000);
}

// Calculate acceleration from accelerometer raw data
// Driter i z-aksen, anntar byen er flat!
float calculate_Acceleration()
{

  // Accelerations in g's per axis
  float ax = imu.a.x * conversion_factor / 1000;
  float ay = imu.a.y * conversion_factor / 1000;

  float a = sqrtf((ax * ax) + (ay * ay)); // Acceleration in g's total
  float acceleration = a * g; // Acceleration in m/s^2
  return acceleration;
}

// Function to calculate and return acceleration values in an array
void calculate_Acceleration_g(float accelerationArray[3])
{
  imu.read();

  // Convert accelerometer readings from milli-g to g
  accelerationArray[0] = imu.a.x * conversion_factor / 1000.0; // ax
  accelerationArray[1] = imu.a.y * conversion_factor / 1000.0; // ay
}
