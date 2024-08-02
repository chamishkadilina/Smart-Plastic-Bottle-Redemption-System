import serial
import time
import bottle_detection
import db_updater

# Set up the serial port
ser = serial.Serial('COM3', 9600) # serial port number
time.sleep(2)  # Allow time for the serial connection to initialize

# Keep the script running to receive the response from Arduino
def interact_with_arduino():
    while True:
        if ser.in_waiting > 0:
            # Read the message from Arduino
            message_from_arduino = ser.readline().decode('utf-8', errors='ignore').strip()

            # Check if the message indicates weight is OK
            if message_from_arduino.startswith("Weight is ok, User: "):
                print(f"Read the message from Arduino: {message_from_arduino}")

                # Extract the user name from the message
                user_name = message_from_arduino.split("User: ")[1]
                
                # Call the main function from bottle_detection to capture and classify the image
                bottle_detection.main()

                # to get myPrediction value into this code space
                prediction = bottle_detection.myPrediction

                # Send a message back to Arduino and update the database
                if(prediction == "Plastic bottle detected!"):
                    ser.write(b"Plastic bottle detected!\n")
                    # update db
                    db_updater.record_user_points(user_name)
                    
                elif(prediction == "No plastic bottle detected."):
                    ser.write(b"No plastic bottle detected.\n")
                
                else:
                    ser.write(b"Failed to capture image\n")

if __name__ == "__main__":
    try:
        interact_with_arduino()
    except KeyboardInterrupt:
        print("Serial connection closed.")