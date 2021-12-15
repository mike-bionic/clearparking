int entrance_sensor_1 = 8;
int entrance_sensor_2 = 9;
int entrance_sensor_3 = 10;

int entrance_relay = 6;

int val_entrance_sensor_1;
int val_entrance_sensor_2;
int val_entrance_sensor_3;
bool ledState = false;
int counter = 0;

bool gate_state = false;

unsigned long close_door = 0;
unsigned long previous_millis = 0;

String data;

void setup() {
  Serial.begin(9600);
  pinMode(entrance_sensor_1, INPUT);
  pinMode(entrance_sensor_2, INPUT);
  pinMode(entrance_sensor_3, INPUT);
  pinMode(entrance_relay, OUTPUT);
}

void loop() {
  measure();
  if (Serial.available() != 0) {
    String data = Serial.readStringUntil('\n');
    data.trim();
    if (data == "value") {
      check_car_presence();
    }
    if (data.length() > 15) {
      gate_management(data);
    }
  }
  close_gates();
}

void measure() {
  val_entrance_sensor_1 = digitalRead(entrance_sensor_1);
  val_entrance_sensor_2 = digitalRead(entrance_sensor_2);
  val_entrance_sensor_3 = digitalRead(entrance_sensor_3);
}
void close_gates() {
  if (counter == 6) {
    gate_state = false;
    digitalWrite(entrance_relay, LOW);
    counter = 0;
  }
  if (millis() - close_door >= 1000 and gate_state == true and val_entrance_sensor_2 == 1 and val_entrance_sensor_3 == 1) {
    close_door = millis();
    counter++;
  }
  if (val_entrance_sensor_3 == 0 or val_entrance_sensor_2 == 0) {
    counter = 0;
  }
}

void check_car_presence() {
  if (val_entrance_sensor_1 == 0 and val_entrance_sensor_2 == 0) {
    Serial.println(1);
  }
  else {
    Serial.println(0);
  }
}

void gate_management(String data) {
  //type:entrance:direction:up
  String gate_type = getValue(data, ':', 1);
  String gate_direction = getValue(data, ':', 3);
  if (gate_type == "entrance") {
    Serial.println("1stOK");
  }
  if (gate_direction == "up") {
    Serial.println("2ndOK");
  }
  if (gate_type == "entrance" && gate_direction == "up") {
    Serial.println("LedOn");
    gate_state = true;
    counter = 0;
    digitalWrite(entrance_relay, HIGH);
  }
}

String getValue(String data, char separator, int index) {
  int strIndex[] = {0, -1};
  int found = 0;
  int maxIndex = data.length() - 1;
  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
