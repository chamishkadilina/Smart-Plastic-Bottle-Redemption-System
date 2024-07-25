#include <SPI.h>
#include <HX711_ADC.h> // HX711 library
#include <Servo.h> // Servo motor library
#include <MFRC522.h> // RFID library
#include <Wire.h>
#include <LiquidCrystal_I2C.h> // I2C LCD library

Servo myServo;  // create servo object name as my servo
Servo myServo2;  // create servo object name as my servo
int angle = 0;  // Initialize the angle variable

// Define pins for HX711 (DT, SCK), Servo motor, RFID (SS, RST), and LCD (address, columns, rows)
HX711_ADC LoadCell(4, 5); // DT pin = 4, SCK pin = 5

#define SS_PIN 10 // RFID SS pin
#define RST_PIN 9  // RFID RST pin
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance
LiquidCrystal_I2C lcd(0x27, 16, 2); // LCD address = 0x27, 16 columns, 2 rows

// Define acceptable weight range
float minWeight = 17.0; // Minimum acceptable weight in grams
float maxWeight = 23.0; // Maximum acceptable weight in grams

void setup() {
  //-------------------chamishka added-------------------
    while (!Serial) {
    ; // wait for the serial port to connect. Needed for native USB port only
  }
  //-------------------chamishka added-------------------
  myServo.attach(3); // Attach servo to pin 3 open box sevo motor
  myServo2.attach(6);// Attach servo to pin 6 motor connect with load cell
  myServo2.write(90);  // Set initial position to 90 degrees
  delay(1000);        // Delay for 1 second

  Serial.begin(9600); // Start serial communication
  SPI.begin(); // Initialize SPI bus
  mfrc522.PCD_Init(); // Initialize RFID module
  Serial.println("scan your RFID card to the reader");
  Serial.println();
  lcd.init(); // Initialize LCD
  lcd.backlight(); // Turn on LCD backlight
  
  LoadCell.begin(); // Start HX711
  LoadCell.start(1000); // Allow time for stabilization
  LoadCell.setCalFactor(1000); // Calibration factor (adjust as per setup) we can get using 4 numbers value

}

void loop()
 {
  lcd.clear(); // Clear LCD screen
  lcd.setCursor(0, 0); // Set cursor to top left corner
  lcd.print("Please scan your"); // Display message on LCD
  lcd.setCursor(0,1);
  lcd.print("RFID card....");
  delay(1000); // Delay for readability
  lcd.clear(); // Clear LCD screen

  // Check for new RFID card
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  
  // Read RFID card
  if (!mfrc522.PICC_ReadCardSerial()) {
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
  if (content.substring(1) == "F3 C9 71 A9" || content.substring(1) == "A3 48 13 AA")
   {
    lcd.setCursor(0, 0); // Set cursor to top left corner
    lcd.print("ID: "); // student number of the students we given
    lcd.print(content.substring(1)); // Display ID on LCD
    lcd.setCursor(0, 1); // Set cursor to bottom left corner
    lcd.print("Registered"); // Display message on LCD
    Serial.println("RFID card registered. Proceeding..."); // Serial monitor output
    delay(2000); // Delay for readability
    lcd.clear(); // Clear LCD screen

    lcd.setCursor(0, 0); // Set cursor to top left corner
    lcd.print("Insert bottle "); // Display message on LCD
    lcd.setCursor(0, 1);
    lcd.print("to the machine");
    delay(2000); // Delay for readability to the user

    // Open servo motor to allow bottle insert to the machine
    myServo.write(0); // Rotate servo to 0 degrees
    delay(7000); // Wait for servo to open
    myServo.write(80); // Rotate servo to 120 degrees (adjust as needed)
    delay(3000); // Wait for bottle insertion

    // Measure weight after bottle insertion using load cell
    LoadCell.update(); // Update load cell data
    float weight = LoadCell.getData(); // Get weight in grams
    

    // Display weight on LCD
    lcd.clear();
    lcd.setCursor(0, 0); // Set cursor to top left corner
    lcd.print("Weight: ");
    lcd.print(weight); // Display weight on LCD
  
    // Check if weight is within our acceptable range
    if (weight >= minWeight && weight <= maxWeight) {
      //-------------------chamishka added-------------------
      //  Weight is ok. so,send a message to the Python script
      Serial.println("Weight is ok");
      //-------------------chamishka added-------------------

      lcd.setCursor(0, 1); // Set cursor to bottom left corner
      lcd.print("Bottle weight ok"); // Display message on LCD
      delay(4000);
      Serial.println("Bottle weight match we included weigh range"); // Serial monitor print output

// if bottle accept again check using open cv whether it is bottle or not if conditon shoul be included this place
// This is servo motor rotate accept bottle side 
// methana if condition ekak dla open cv ekekn ena output eka 1 kada 0 kiyala dla if(0) unoth e kiynne bottle eka plastic newe nm rjection patte
// dora arila pahalata watenawa if(1) unoth e kiynne accept patte dora arila eka pahalata watenna ona 

      //-------------------chamishka added-------------------
      if (Serial.available() > 0) {
          // Read the incoming data from Python
          String message = Serial.readStringUntil('\n');

          // Accepted by Python OpenCV
          if (message == "Plastic bottle detected!") {

            // servo motor rotate to accept box
            for (angle = 90; angle <= 180; angle += 1) {
              myServo2.write(angle);  
              delay(5);             
            }
            delay(1000);  

            // servo motor rotate back to it's initial possition
            for (angle = 180; angle >= 90; angle -= 1) {
              myServo2.write(angle);  
              delay(5);            
            }
            delay(1000);  
          }

          // Rejected by Python OpenCV
          else if (message == "No plastic bottle detected.") {

            // LCD display message "Rejected by Python OpenCV"
            lcd.setCursor(0, 1); 
            lcd.print("rejected by Python OpenCV");
            delay(4000);

            // servo motor rotate to reject box
            for (angle = 90; angle >= 0; angle -= 1) {
                myServo2.write(angle);
                delay(5);
            }
            delay(1000);

            // servo motor rotate back to it's initial possition
            for (angle = 0; angle <= 90; angle += 1) {
                myServo2.write(angle);
                delay(5);
            }
            delay(1000);
          }

          // Failed to capture image by Python OpenCV. hens, reject the object.
          else {

            // LCD display message "Failed to capture image"
            lcd.setCursor(0, 1); 
            lcd.print("Failed to capture image");
            delay(4000);

            // servo motor rotate to reject box
            for (angle = 90; angle >= 0; angle -= 1) {
              myServo2.write(angle);
              delay(5);
            }
            delay(1000);

            // servo motor rotate back to it's initial possition
            for (angle = 0; angle <= 90; angle += 1) {
              myServo2.write(angle);
              delay(5);
            }
            delay(1000);
          }
      } 
      //-------------------chamishka added-------------------

    } 
    // if rejected direcly from the load cell this code part should be worked
    else {
      lcd.setCursor(0, 1); // Set cursor to bottom left corner
      lcd.print("Reject bottle"); // Display message on LCD
      delay(4000);
      Serial.println("Your bottle was rejected.Please make sure to include empty bottle without filling anything"); // Serial monitor print output

    for (angle = 90; angle >= 0; angle -= 1) {
        myServo2.write(angle);  // Set servo angle
        delay(5);             // Delay for smooth motion
    }
      delay(1000);  // Delay for 1 second

    for (angle = 0; angle <= 90; angle += 1) {
        myServo2.write(angle);  // Set servo angle
        delay(5);             // Delay for smooth motion
    }
    delay(1000);  // Delay for 1 second

    }

    // Reset load cell for next operation
    LoadCell.powerDown(); // Power down HX711
    LoadCell.powerUp(); // Power up HX711
    delay(100); // Delay for stabilization
    lcd.clear(); // Clear LCD screen

  } else {
    // if someone put not registered card in to RFID reader
    // Display not registered message on LCD

    lcd.setCursor(0, 0); // Set cursor to top left corner
    lcd.print("ID: ");
    lcd.print(content.substring(1)); // Display ID on LCD
    lcd.setCursor(0, 1); // Set cursor to bottom left corner
    lcd.print("Not Registered"); // Display message on LCD
    Serial.println("RFID card not registered.Firstly you should register for this system"); // Serial output
    delay(5000); // Delay for readability
    lcd.clear(); // Clear LCD screen
    lcd.setCursor(0, 0); 
    lcd.print(" Firstly you  ");
    lcd.setCursor(0, 1);
    lcd.print(" should register ");
    delay(5000);
    lcd.clear();
    lcd.setCursor(0, 0); // Set cursor to top left corner
    lcd.print(" Thank you "); // Display message on LCD
    delay(2000); // Delay for readability
    lcd.clear(); // Clear LCD screen
  }
  //-------------------chamishka added-------------------
  // Add a small delay to avoid flooding the serial port
  delay(2000);
  //-------------------chamishka added-------------------

}
