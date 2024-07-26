#include <SPI.h>
#include <HX711_ADC.h> 
#include <Servo.h> 
#include <MFRC522.h> 
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

Servo boxOpener;
Servo bottleServo;
int angle = 0;

// Define pins for HX711, RFID, and LCD
HX711_ADC LoadCell(4, 5); // DT pin = 4, SCK pin = 5
#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);
LiquidCrystal_I2C lcd(0x27, 16, 2); // LCD address = 0x27, 16 columns, 2 rows


float minWeight = 17.0; // define min weight (g)
float maxWeight = 23.0; // define max weight (g)
String userName;

// setup code here, this run once
void setup() {
  while (!Serial) {
    ; // Wait for serial port to connect (usb cable)
  }

  boxOpener.attach(3); // Servo for opening box
  bottleServo.attach(6); // Servo for load cell
  bottleServo.write(90); // Initial position
  delay(1000);

  Serial.begin(9600); // Start serial communication
  SPI.begin(); // Initialize SPI bus
  mfrc522.PCD_Init(); // Initialize RFID module
  lcd.init(); // Initialize LCD
  lcd.backlight(); // Turn on LCD backlight
  
  LoadCell.begin(); // Start HX711
  LoadCell.start(1000); // Allow time for stabilization
  LoadCell.setCalFactor(1000); // Calibration factor (adjust as per setup)
}

// main code, this run repeatedly
void loop() {
  // Display message on LCD
  lcd.clear(); 
  lcd.setCursor(0, 0);
  lcd.print("Please scan your");
  lcd.setCursor(0,1);
  lcd.print("RFID card....");
  delay(2000);

  // Check for new RFID card and Read it
  if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  // Get UID of RFID card
  String content = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  content.toUpperCase(); // Convert to uppercase

  // Check if the scanned RFID card is registered or not
  if (content.substring(1) == "F3 C9 71 A9" || content.substring(1) == "A3 48 13 AA") {
    // Assign a user name for specific RFID Card using ternary operator
    userName = (content.substring(1) == "F3 C9 71 A9") ? "Chamishka" : "Harini";

    // Show 'Insert bottle' message in LCD
    lcd.setCursor(0, 0);
    lcd.print("Insert bottle ");
    lcd.setCursor(0, 1);
    lcd.print("to the machine");
    delay(2000);

    // Open servo motor to allow bottle insert to the machine
    boxOpener.write(0); // Rotate servo to 0 degrees
    delay(7000); // Wait for servo to open
    boxOpener.write(90); // Rotate servo to 90 degrees (adjust as needed)
    delay(4000); // Wait for bottle insertion

    // Measure weight after bottle insertion using load cell
    LoadCell.update(); // Update load cell data
    float weight = LoadCell.getData(); // Get weight in grams
    
    // Display weight on LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Weight: ");
    lcd.print(weight);
  
    // Check if weight is within our acceptable range
    if (weight >= minWeight && weight <= maxWeight) {
      // send a message to the Python script
      Serial.print("Weight is ok, User: ");
      Serial.println(userName);

      // Display 'Bottle weight ok' on LCD
      lcd.setCursor(0, 1);
      lcd.print("Bottle weight ok"); 
      delay(4000);

      if (Serial.available() > 0) {
        // Read incoming data from Python
        String message = Serial.readStringUntil('\n');

        // Accepted by Python OpenCV
        if (message == "Plastic bottle detected!") {
          // Rotate servo to accept box and then return to initial position
          moveServo(bottleServo, 90,180);  
          moveServo(bottleServo, 180, 90);  
        }
        // Rejected by Python OpenCV
        else if (message == "No plastic bottle detected.") {
          // Display 'Bottle weight ok' on LCD
          lcd.setCursor(0, 0); 
          lcd.print("rejected by");
          lcd.setCursor(0, 1); 
          lcd.print("Python OpenCV");
          delay(4000);
          // Rotate servo to reject box and then return to initial position
          moveServo(bottleServo, 90, 0);  
          moveServo(bottleServo, 0, 90);  
        }
        // Image capture failed
        else {
          // Display 'capture Failed' on LCD
          lcd.setCursor(0, 0); 
          lcd.print("capture Failed");
          delay(4000);
          // Rotate servo to reject box and then return to initial position
          moveServo(bottleServo, 90, 0);  
          moveServo(bottleServo, 0, 90); 
        }
      } 
    }

    // Weight is outside the acceptable range
    else {
      // Display 'Reject bottle' on LCD
      lcd.setCursor(0, 0);
      lcd.print("Weight out of");
      lcd.setCursor(0, 1);
      lcd.print("range");
      delay(4000);
      // Rotate servo to reject box and then return to initial position
      moveServo(bottleServo, 90, 0);
      moveServo(bottleServo, 0, 90);
    }

    // Reset load cell for next operation
    LoadCell.powerDown();
    LoadCell.powerUp();
    delay(100); // Delay for stabilization
    lcd.clear();
  }

  // if scanned RFID card is NOT registered
  else {
    // Display 'Not Registered' on LCD
    lcd.setCursor(0, 0);
    lcd.print("ID: ");
    lcd.print(content.substring(1));
    lcd.setCursor(0, 1);
    lcd.print("Not Registered");
    delay(4000);
    lcd.clear();
  }
  // Delay to prevent serial flooding
  delay(2000);
}

// servo reusable function
void moveServo(Servo& servo, int startAngle, int endAngle) {
  if (startAngle < endAngle) {
    for (angle = startAngle; angle <= endAngle; angle++) {
      servo.write(angle);
      delay(5);
    }
  } else {
    for (angle = startAngle; angle >= endAngle; angle--) {
      servo.write(angle);
      delay(5);
    }
  }
}
