#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ESP32Servo.h>

// Pin definitions
#define BUTTON_PIN 5       // Button pin
#define BOX_OPENER_PIN 13  // Servo for opening the entry door
#define BOTTLE_SERVO_PIN 12 // Servo for sorting bottles (left/right)
#define TOFFEE_SERVO_PIN 14 // Servo for dispensing toffee

// Create objects
LiquidCrystal_I2C lcd(0x27, 16, 2); // LCD address, columns, rows
Servo boxOpenerServo;  // Servo for opening the box
Servo bottleServo;     // Servo for sorting bottles
Servo toffeeServo;     // Servo for dispensing toffee

// Variables
int buttonState = 0;
bool processingBottle = false;

void setup() {
  // Initialize serial communication 
  Serial.begin(9600);
  
  // Initialize I2C LCD
  lcd.init();
  lcd.backlight();
  
  // Initialize servo motors
  boxOpenerServo.attach(BOX_OPENER_PIN);
  bottleServo.attach(BOTTLE_SERVO_PIN);
  toffeeServo.attach(TOFFEE_SERVO_PIN);
  
  // Set initial servo positions
  boxOpenerServo.write(0);  // Door closed
  bottleServo.write(90);    // Center position
  toffeeServo.write(0);     // Toffee dispenser closed
  
  // Set button pin as input with pull-up resistor
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  
  // Display welcome message
  displayWelcomeMessage();
}

void loop() {
  // Read button state (LOW when pressed because of pull-up)
  buttonState = digitalRead(BUTTON_PIN);
  
  // Check if button is pressed and not currently processing a bottle
  if (buttonState == LOW && !processingBottle) {
    processingBottle = true;
    
    // Process the bottle
    processBottle();
    
    // Reset processing flag
    processingBottle = false;
    
    // Display welcome message again
    displayWelcomeMessage();
  }
  
  // Short delay to prevent button bounce
  delay(100);
}

void displayWelcomeMessage() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Welcome! Press");
  lcd.setCursor(0, 1);
  lcd.print("button to start");
}

void processBottle() {
  // 1. Open the door
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Opening door...");
  boxOpenerServo.write(90);  // Open the door
  
  // 2. Wait for bottle insertion
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Insert bottle");
  lcd.setCursor(0, 1);
  lcd.print("please...");
  delay(5000);  // Wait 5 seconds for bottle insertion
  
  // 3. Close the door
  boxOpenerServo.write(0);  // Close the door
  
  // 4. Tell Python to run the model
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Scanning...");
  Serial.println("run model");
  
  // 5. Wait for Python response
  String response = waitForPythonResponse();
  
  // 6. Process the response
  lcd.clear();
if (response == "Plastic bottle detected!") {
    lcd.setCursor(0, 0);
    lcd.print("Plastic bottle");
    lcd.setCursor(0, 1);
    lcd.print("detected!");

    // Ensure the servo is attached
    bottleServo.attach(BOTTLE_SERVO_PIN);

    // Move bottle to the left side (accepted)
    moveServo(bottleServo, 90, 130); // Adjusted angle for better sorting
    delay(500);
    moveServo(bottleServo, 130, 90);

    dispenseToffee();
} 
else {
    lcd.setCursor(0, 0);
    lcd.print("Not a plastic");
    lcd.setCursor(0, 1);
    lcd.print("bottle!");

    // Ensure the servo is attached
    bottleServo.attach(BOTTLE_SERVO_PIN);

    // Move bottle to the right side (rejected)
    moveServo(bottleServo, 90, 45); // Adjusted angle for rejection
    delay(500);
    moveServo(bottleServo, 45, 90);
}

  
  // 7. Thank you message
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Thank you!");
  lcd.setCursor(0, 1);
  lcd.print("Have a nice day!");
  delay(3000);
}

void dispenseToffee() {
  // Display toffee message
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Get your toffee");
  
  // Move toffee servo to dispense
  toffeeServo.attach(TOFFEE_SERVO_PIN);
  moveServo(toffeeServo, 0, 5);
  
  // Detach the servo to stop movement
  toffeeServo.detach();
  
  // Wait a moment
  delay(1000);
}


void moveServo(Servo &servo, int startAngle, int endAngle) {
  if (startAngle < endAngle) {
    for (int angle = startAngle; angle <= endAngle; angle++) {
      servo.write(angle);
      delay(15);  // Slow movement
    }
  } else {
    for (int angle = startAngle; angle >= endAngle; angle--) {
      servo.write(angle);
      delay(15);  // Slow movement
    }
  }
}

String waitForPythonResponse() {
  String response = "";
  unsigned long startTime = millis();
  unsigned long timeout = 15000;  // 15 seconds timeout
  
  while (millis() - startTime < timeout) {
    if (Serial.available() > 0) {
      response = Serial.readStringUntil('\n');
      response.trim();
      return response;
    }
    delay(100);
  }
  
  // If timeout occurs
  return "Detection timeout";
}