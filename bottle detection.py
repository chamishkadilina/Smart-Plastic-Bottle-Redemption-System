import cv2
import numpy as np
import tensorflow as tf
from tensorflow.keras.applications.mobilenet_v2 import MobileNetV2, preprocess_input, decode_predictions

# Load pre-trained MobileNetV2 model + higher level layers
model = MobileNetV2(weights='imagenet')

def capture_image():
    # Initialize the camera
    cap = cv2.VideoCapture(0)
    
    # Read a frame from the camera
    ret, frame = cap.read()
    if not ret:
        print("Failed to capture image")
        cap.release()
        return None
    
    # Release the camera
    cap.release()
    
    # Save the captured image to a file
    cv2.imwrite('captured_image.jpg', frame)
    
    return frame

def classify_image(image):
    # Convert the image to a numpy array
    img_array = cv2.resize(image, (224, 224))  # Resize to 224x224 as required by MobileNetV2
    img_array = np.expand_dims(img_array, axis=0)  # Add batch dimension
    img_array = preprocess_input(img_array)  # Preprocess the image
    
    # Perform the prediction
    predictions = model.predict(img_array)
    
    # Decode the predictions
    decoded_predictions = decode_predictions(predictions, top=3)
    
    return decoded_predictions

def main():
    # Capture image from camera
    image = capture_image()
    if image is None:
        return
    
    # Classify the captured image
    predictions = classify_image(image)
    
    # Print the predictions
    for i, (imagenet_id, label, score) in enumerate(predictions[0]):
        print(f"{i+1}: {label} ({score:.2f})")
    
    # Check if one of the top predictions is a 'bottle'
    for _, label, _ in predictions[0]:
        if 'bottle' in label:
            print("Plastic bottle detected!")
            return
    
    print("No plastic bottle detected.")

if __name__ == "__main__":
    main()
