import serial
import time
import cv2
import numpy as np
import tensorflow as tf
from tensorflow.keras.applications.mobilenet_v2 import MobileNetV2, preprocess_input, decode_predictions # type: ignore

# Set up the serial port - adjust the port as needed for your system
# On Windows it might be 'COM3', on Linux '/dev/ttyUSB0', on Mac '/dev/cu.SLAB_USBtoUART'
ser = serial.Serial('COM6', 9600)  # Change this to match your ESP32 port
time.sleep(2)  # Allow time for the serial connection to initialize

# Load pre-trained MobileNetV2 model
print("Loading model...")
model = MobileNetV2(weights='imagenet')
print("Model loaded!")

def capture_image():
    print("Capturing image...")
    # Initialize the camera
    cap = cv2.VideoCapture(0)
    
    # Read a frame from the camera
    ret, frame = cap.read()
    if not ret:
        print("Failed to capture image")
        cap.release()
        return None
    
    # Release the camera``
    cap.release()
    
    # Save the captured image to a file
    cv2.imwrite('captured_image.jpg', frame)
    print("Image captured successfully")
    
    return frame

def classify_image(image):
    print("Classifying image...")
    # Convert the image to a numpy array
    img_array = cv2.resize(image, (224, 224))  # Resize to 224x224 as required by MobileNetV2
    img_array = np.expand_dims(img_array, axis=0)  # Add batch dimension
    img_array = preprocess_input(img_array)  # Preprocess the image
    
    # Perform the prediction
    predictions = model.predict(img_array)
    
    # Decode the predictions
    decoded_predictions = decode_predictions(predictions, top=5)
    
    return decoded_predictions

def detect_bottle():
    # Capture image from camera
    image = capture_image()
    if image is None:
        return "Failed to capture image"
    
    # Classify the captured image
    predictions = classify_image(image)
    
    # Print the predictions
    print("Top predictions:")
    for i, (imagenet_id, label, score) in enumerate(predictions[0]):
        print(f"{i+1}: {label} ({score:.2f})")
    
    # Check if one of the top predictions is a 'bottle'
    for _, label, score in predictions[0]:
        if 'bottle' in label and score > 0.1:  # Only accept bottles with confidence > 10%
            print("Plastic bottle detected!")
            return "Plastic bottle detected!"
    
    print("No plastic bottle detected.")
    return "No plastic bottle detected."

def main():
    print("Starting bottle detection system...")
    print("Waiting for commands from ESP32...")
    
    while True:
        try:
            if ser.in_waiting > 0:
                # Read the message from ESP32
                message = ser.readline().decode('utf-8', errors='ignore').strip()
                print(f"Received from ESP32: {message}")
                
                # Check if the message is for bottle detection
                if message == "run model":
                    print("Starting bottle detection...")
                    
                    # Detect bottle
                    result = detect_bottle()
                    print(f"Detection result: {result}")
                    
                    # Send the result back to ESP32
                    ser.write(f"{result}\n".encode())
        except Exception as e:
            print(f"Error: {e}")
            # Continue running even if there's an error

if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        print("\nProgram terminated by user")
        ser.close()
    except Exception as e:
        print(f"An unexpected error occurred: {e}")
        ser.close()