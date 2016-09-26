 
const float VOLT_LEVEL = 3.3; //applied voltage input of accel
const float X_VOLT_0G = 1.65; //voltage at 0g for x
const float Y_VOLT_0G = 1.65;
const float Z_VOLT_0G = 1.8;
const float SENS = 0.33; //Sensitivity 330mV/g
 
 ////////////////////////////////////////////////
 const unsigned int X_AXIS_PIN = A3;
  const unsigned int Y_AXIS_PIN = A2;
  const unsigned int Z_AXIS_PIN = A1;
  const unsigned int NUM_AXES = 3;
 const unsigned int PINS[NUM_AXES] = {
  X_AXIS_PIN, Y_AXIS_PIN, Z_AXIS_PIN
  };
  const unsigned int BUFFER_SIZE = 30;
  const unsigned int BAUD_RATE = 9600;
 int buffer[NUM_AXES][BUFFER_SIZE];
  int buffer_pos[NUM_AXES] = { 0 };
 
  void setup() {
  Serial.begin(BAUD_RATE);
 }
 
  int get_axis(const int axis) {
  delay(1);
  buffer[axis][buffer_pos[axis]] = analogRead(PINS[axis]);
buffer_pos[axis] = (buffer_pos[axis] + 1) % BUFFER_SIZE;
  long sum = 0;
  for (unsigned int i = 0; i < BUFFER_SIZE; i++)
  sum += buffer[axis][i];
  return round(sum / BUFFER_SIZE); }
 
  int get_x() { return get_axis(0); }
  int get_y() { return get_axis(1); }
  int get_z() { return get_axis(2); }
 void loop() {

  int x = get_x();
  int y = get_y();
  int z = get_z();
  
  float xG = convertToG_xy(x);
  float yG = convertToG_xy(y);
  float zG = convertToG_z(z);
  
  Serial.print("x:");
  Serial.print(x);
  Serial.print("/ ");
  Serial.print(xG);
  Serial.print("\ty:");
  Serial.print(y);
  Serial.print("/ ");
  Serial.print(yG);
  Serial.print("\tz:");
 Serial.print(z);
 Serial.print("/");
 Serial.println(zG);
  
  
  }

float convertToG_xy(int axis)
{

  float xy_volt = (axis * VOLT_LEVEL) / 1023; //voltage output relative to VOLT_LEVEL
  float xy_deltaVolt = xy_volt - X_VOLT_0G; //Diffrence from VOLT_0g
  float  xyG = xy_deltaVolt / SENS;
  
  return xyG;
}

int convertToG_z(int axis)
{

  float z_volt = (axis * VOLT_LEVEL) / 1023; //voltage output relative to VOLT_LEVEL
  float z_deltaVolt = z_volt - Z_VOLT_0G; //Diffrence from VOLT_0g
  float zG = z_deltaVolt / SENS;
 
  return zG;
}

