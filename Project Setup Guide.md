# Project Setup Guide 🍀
> [!NOTE]
> This guide is for setting up the project on a ``Windows machine``.</br>
> If you're using a other OS, please contact us for specific instructions.

### 1. Clone the Repository
- Clone the project repository to your local machine.
```
https://github.com/chamishkadilina/Smart-Plastic-Bottle-Redemption-System.git
```

### 2. Install Python
- Download Python :
  - Visit the official Python website: [Python Downloads](https://www.python.org/downloads/).
  - Download the latest version of Python for Windows.
- Install Python :
  - Run the downloaded installer.
  - During installation, ensure the following options are selected :
    - Use admin privileges when installing py.exe
    - Add python.exe to PATH
- This setup ensures that Python and pip commands are recognized in the command prompt.
- Verify Installation :
		Open a new command prompt window and type :
    ```
    python --version
    ```
    ```
    pip --version
    ```
- These commands should display the versions of Python and pip installed on your system.

### 3. Install Required Python Packages
- To run this project, you'll need to install several Python packages.</br>
(The approximate download size is 500MB)
- Packages :
  - ```PySerial``` For communication with the Arduino.
  - ```OpenCV-python``` For capturing and processing images.
  - ```NumPy``` For handling arrays and numerical operations.
  - ```TensorFlow``` For loading and using the MobileNetV2 model.
  
- To install all required packages open the command prompt and run this :
  ```
  pip install pyserial opencv-python numpy tensorflow
  ```

### 4. Set Up a Code Editor
- We recommend using Visual Studio Code. Download and install it or any other preferred code editor.
- install these VS Code Extensions :
  - ```Python```
  - ```Pylance```
  - ```Python Debugger```

### 5. Install Arduino IDE
- Download and Install :
  - Visit the official Arduino ide website : [Arduino IDE download](https://www.arduino.cc/en/software).
  - Follow a YouTube tutorial if needed to set up the IDE correctly with your Arduino board.

### 6. Install XAMPP
- Download and Install :
  - Visit the official XAMPP website : [Download XAMPP](https://www.apachefriends.org/download.html).
  - This XAMPP provides a local web server environment for testing and development.

### 7. Start XAMPP Server
- Launch XAMPP Control Panel:
- Navigate to ```C:\xampp\xampp-control```.
- Start ```Apache``` and ```MySQL``` services.
- Minimize the control panel. ( Because it needs to remain running in the background. )

### 8. Create a Database and Table in MySQL
- In your web browser, access phpMyAdmin by navigating to ```http://localhost/phpmyadmin```.
- In phpMyAdmin, create a database named ```bottle_db``` and a table named ```user_data``` with the following columns:
  - id (INT, PRIMARY KEY, AUTO_INCREMENT)
  - name (VARCHAR)
  - value (INT)
### 9. View the Data on Webpage
- Insert example data manually in the ```user_data``` table to ensure it is functioning correctly.
- In your browser, navigate to ```http://localhost/display_data.php``` to view the inserted data. ( After verifying, delete the manually added data. )

### 10. Set Up Project Files
- Prepare Web Resources :
  - Go to the ```Smart-Plastic-Bottle-Redemption-System``` folder, then navigate to ```Project Files```.
  - Copy ```display_data.php``` and ```website_bg.jpg``` paste to ```C:\xampp\htdocs```.

### 11. Upload Arduino Sketch
- In the ```Smart-Plastic-Bottle-Redemption-System``` folder, go to ```Project Files\SmartRecycler```.
- Open ```SmartRecycler.ino``` in the Arduino IDE and upload it to your Arduino board. ```Keep the USB cable connected``` as it is required to run the Python script.

### 12. Run the Project 😍
- Run Python Script :
  - In VS Code, open the project and navigate to ```Project Files```.
  - Run the ```arduino_communication.py``` file to start the project.
