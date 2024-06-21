bool oneTimeLoop = true;

void setup() {
  // Start the serial communication
  Serial.begin(115200);
  pinMode(2, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(18, OUTPUT);
  while (!Serial) {
    ; // wait for the serial port to connect. Needed for native USB port only
  }
}

void loop() {
  if (oneTimeLoop) {
     //  send a message to the Python script
    Serial.println("Weight is ok");
    oneTimeLoop = false;
  }

  // Check if data is available to read from Python
  if (Serial.available() > 0) {
    // Read the incoming data
    String message = Serial.readStringUntil('\n');
    
    // Print the message received from Python
    Serial.print("Received: ");
    Serial.println(message);

    // Blink LED based on the message received
    if (message == "Plastic bottle detected!") {
      digitalWrite(2, HIGH);
      delay(1000);
      digitalWrite(2, LOW);
    }
    else if (message == "No plastic bottle detected.") {
      digitalWrite(4, HIGH);
      delay(1000);
      digitalWrite(4, LOW);
    }
    else if (message == "Failed to capture image") {
      digitalWrite(18, HIGH);
      delay(1000);
      digitalWrite(18, LOW);
    }
  }
}
