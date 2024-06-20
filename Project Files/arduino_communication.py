import serial
import time
import bottle_detection

# Set up the serial port
ser = serial.Serial('COM3', 115200)  # Replace '/dev/ttyUSB0' with your ESP32's serial port
time.sleep(2)  # Give some time for the serial connection to initialize

# Keep the script running to receive the response from Arduino
def interact_with_arduino():
    while True:
        if ser.in_waiting > 0:
            # Read the message from Arduino
            message_from_arduino = ser.readline().decode('utf-8', errors='ignore').strip()
            if(message_from_arduino == "Weight is ok"):
                print(f"Read the message from Arduino: {message_from_arduino}")
                
                # Call the main function from bottle_detection to capture and classify the image
                bottle_detection.main()
                
                prediction = bottle_detection.myPrediction

                # Send a message back to Arduino
                if(prediction == "Plastic bottle detected!"):
                    ser.write(b"Plastic bottle detected!")
                    
                elif(prediction == "No plastic bottle detected."):
                    ser.write(b"No plastic bottle detected.")
                
                else:
                    ser.write(b"Failed to capture image")

                #read the response from Arduino
                # response = ser.readline().decode('utf-8').strip()
                # print(f"Send a message back to Arduino: {response}")

if __name__ == "__main__":
    try:
        interact_with_arduino()
    except KeyboardInterrupt:
        print("Serial connection closed.")