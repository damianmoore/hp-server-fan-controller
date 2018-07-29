const int fan_pin = 2;
const int alert_pin = 3;
const int pwm_in_pin = 4;
const int pwm_out_pin = 5;
const int led_pin =  LED_BUILTIN;

int fan_state = 0;
unsigned long start = 0;
unsigned long went_high = 0;
unsigned long went_low = 0;
int pwm_in_value = 0;
int pwm_out_value = 100;


void setup() {
  pinMode(fan_pin, INPUT);
  pinMode(alert_pin, OUTPUT);
  pinMode(led_pin, OUTPUT);
  pinMode(pwm_in_pin, INPUT);
  pinMode(pwm_out_pin, OUTPUT);
  digitalWrite(led_pin, LOW);
  Serial.begin(115200);
  analogWrite(pwm_out_pin, pwm_out_value);
}

void loop() {
  // Set the fan speed high or low depending on whether server PWM signal is
  // above a threshold
  pwm_in_value = pulseIn(pwm_in_pin, HIGH);
  if (pwm_in_value > 40) {
    if (pwm_out_value != 1000) {
      pwm_out_value = 1000;
      analogWrite(pwm_out_pin, pwm_out_value);
    }
  }
  else {
    if (pwm_out_value != 100) {
      pwm_out_value = 100;
      analogWrite(pwm_out_pin, pwm_out_value);
    }
  }
  
  Serial.print("Server PWM width: ");
  Serial.println(pwm_in_value);

  Serial.print("Output PWM value: ");
  Serial.println(pwm_out_value);
  
  went_high = 0;
  went_low = 0;
  start = millis();

  // Read the fan pulses for a period of time to make sure it goes high and
  // low. If it doesn't change then then fan has stopped moving and we need
  // to alert the server by setting alert_pin high
  while (true) {
    fan_state = digitalRead(fan_pin);
    if (fan_state == HIGH) {
      went_high += 1;
    } else {
      went_low += 1;
    }
    
    if (millis() > start + 500) {
      if (went_high > 1000 && went_low > 1000) {
        digitalWrite(alert_pin, LOW);
        digitalWrite(led_pin, LOW);
        Serial.println("Status: OK\n");
      }
      else {
        digitalWrite(alert_pin, HIGH);
        digitalWrite(led_pin, HIGH);
        Serial.println("STATUS: FAILURE\n");
      }
      break;
    }
  }
}
