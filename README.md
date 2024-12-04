#Investigating the Impact of Ventilation on Nocturnal Air Quality in Urban Environments

##About The Project
This project explores how indoor air quality during nocturnal hours changes with and without ventilation. The project monitors various environmental parameters at different locations within a room, providing insights into the dynamics of indoor air quality during sleep. The collected data uses key indoor air quality metrics such as Temperature / ° C, Humidity / %, CO2 / ppm, VOC / ppm, Compensated Gas / ppm and IAQ which have been collected using the Nicla Sense ME BSEC sensor.

##Motivation
The project aims to explore the impacts of Indoor Air Quality (IAQ) in a bedroom at night when it is ventilated and unventilated. In doing so, it is hoped that we will be able to conclude the best way to maintain a healthy IAQ during sleeping periods during nocturnal hours. The project hopes to further explore the various parameters associated with IAQ and the resultant impact they have on air quality. 

##License
This project is licensed under the MIT License – for more details regarding this see the LICENSE file within the repository. 
Summary:
You are free to:
•	Use the code for personal, academic, or commercial purposes
•	Modify and distribute the code
•	Merge the code into your own projects
However, the following conditions apply:
•	You must include the original copyright notice and the license in any copies or substantial portions of the Software.
•	The software is provided "as is", without warranty of any kind.

##Data Collection Methodology
1.	The Nicla Sense Me’s were connected to an Arduino MKR WIFI 1010 Board and labelled 1 or 2, representative of their location within a room.
2.	The Arduino code file titled ‘Code_for_Nicla.ino’ is uploaded to the Nicla Sense ME.
3.	The Arduino code file titled ‘Arduino_x’ was uploaded to the Arduino MKR WIFI 1010 Board, where x = 1 or 2, so that BSEC data could be published to the MQTT broker as topics.
4.	The Arduino 2 was calibrated to Arduino 1. An example of how this can be done can be seen in the python code ‘’. The Calibration was uploaded to Arduino 2 using the Arduino code titled ‘Arduino_2_Calibrated_With_Comments’. 
5.	For each experiment the window condition was changed to open or closed.
6.	The BSEC data for the two sensors were captured every second for 6 hours and recorded in two csv file’s using the code titled ‘Add_Data_and_Metadata_Collection_in_Python.ipynb’
7.	The weather metadata within the code was captured using OpenWeatherAPI, a timestamp and unique overall ID for each measurement were also recorded in the csv. 
8.	The csv files were then stored in a SQL database titled ‘Ventilation_Experiments.db’
9.	Various plots were constructed to understand the effects of ventilation for the two experimental conditions and locations, an example of these plots can be found using the code ‘Accessible_Data_Visualisation.ipynb’

##Software Installation Instructions

###Installing Arduino IDE on Your Laptop (Windows & Mac)
1. Download the Arduino IDE
Windows:
•	Visit the Arduino download page for Windows.
•	Under the "Windows" section, click the "Windows 10/8/7" link to download the installer (e.g., arduino-1.8.x-windows.exe).
macOS:
•	Visit the Arduino download page for macOS.
•	Under the "macOS" section, click the "Mac OS X" link to download the .zip file (e.g., arduino-1.8.x-macos.zip).
2. Install the Arduino IDE
Windows:
•	Run the downloaded installer (arduino-1.8.x-windows.exe).
•	Follow the installation instructions in the setup wizard:
•	Select your preferred installation options.
•	Make sure to check the box to install drivers (important for connecting Arduino boards).
•	Once done, click "Install" and wait for the process to finish.
macOS:
•	After downloading the .zip file, extract it by double-clicking it.
•	Drag the extracted Arduino IDE folder into your Applications folder.
•	To start Arduino, open your Applications folder and double-click on the Arduino.app.
3. Install Arduino Drivers (Windows Only)
Windows:
•	If you are using an Arduino board (e.g., Arduino Uno, Nano, etc.) for the first time, your system may require drivers to communicate with the board.
•	The drivers should automatically install when you run the Arduino IDE and connect the board via USB. If not, you can manually install drivers by following these steps:
•	Open Device Manager (right-click on the Start menu and select Device Manager).
•	Under "Other devices" or "Ports (COM & LPT)," find the Arduino board.
•	Right-click and select Update Driver.
•	Choose Browse my computer for driver software, then point it to the drivers folder inside the Arduino IDE installation directory (e.g., C:\Program Files (x86)\Arduino\drivers).
•	Follow the on-screen prompts to complete the installation.
4. Open the Arduino IDE
•	Windows: After installation, you can launch the Arduino IDE from the Start menu or from the desktop shortcut.
•	macOS: Open the Arduino.app from the Applications folder.
5. Configure the Arduino IDE
•	Select Board Type:
•	Open the Arduino IDE.
•	Go to Tools → Board → select your board model (e.g., Arduino Uno, Arduino Mega, etc.).
•	Select Port:
•	Go to Tools → Port and select the port your Arduino is connected to.
•	On Windows, the port might show up as COM1, COM3, etc., depending on your device.
•	On macOS, it will appear as something like /dev/cu.usbmodemxxxx or /dev/cu.usbserialxxxx.

###Installing Visual Studio Code (VS Code) on Your Laptop (Windows & macOS)
1. Download VS Code
Windows:
1.	Visit the official Visual Studio Code download page: VS Code for Windows.
2.	Click the "Download for Windows" button. This will download the .exe installer.
macOS:
3.	Visit the official Visual Studio Code download page: VS Code for macOS.
4.	Click the "Download for Mac" button. This will download the .zip installer.

2. Install VS Code
•	Windows:
1.	Once the download is complete, run the VS Code installer (VSCodeUserSetup-x64-<version>.exe).
2.	Follow the installation steps in the setup wizard:
	Accept the license agreement.
	Choose the installation path (the default is fine for most users).
	Select additional tasks (e.g., add VS Code to the PATH, register context menu options).
3.	Click Install, and once finished, click Finish to complete the installation.
•	macOS:
1.	After downloading the .zip file, open it to extract the Visual Studio Code application.
2.	Drag Visual Studio Code into your Applications folder.
3.	Open Visual Studio Code from your Applications folder or use Spotlight search to launch it.

3. Launch VS Code
•	Windows: Open VS Code from the Start Menu or by searching for "Visual Studio Code" in the search bar.
•	macOS: Open VS Code from the Applications folder or search using Spotlight.

4. Configure VS Code for Your Development Environment
You can configure VS Code for various programming languages by installing their respective language support extensions (e.g., Python in this experiment) You may also need to install the runtime or compiler for the language you intend to use.
For Python:
1.	Install Python: If you don't already have Python installed, download it here.
2.	Install Python Extension: In VS Code, go to the Extensions view, search for Python, and install the Python extension by Microsoft.
3.	Set Python Interpreter: Open the Command Palette (Ctrl+Shift+P or Cmd+Shift+P on macOS), type Python: Select Interpreter, and select your installed Python interpreter.

###Required Python Libraries
1. Pandas
•	For handling data manipulation, especially with CSV and Excel files.
•	Install: pip install pandas
2. Matplotlib
•	For creating static, animated, and interactive visualizations, such as line plots, heatmaps, etc.
•	Install: pip install matplotlib
3. Seaborn
•	For advanced statistical visualizations, particularly for heatmaps.
•	Install: pip install seaborn
4. Bokeh
•	For creating interactive visualizations (e.g., line plots, hover tools).
•	Install: pip install bokeh
5. pyttsx3
•	For text-to-speech functionality.
•	Install: pip install pyttsx3
6. Openpyxl
•	For reading Excel files
•	Install: pip install openpyxl
7. Matplotlib.colors
•	For custom color maps used in heatmap generation.
•	This comes pre-installed with Matplotlib.

##Cloning the Repository
To clone this repository to your local machine, follow the steps below:
Step 1: Copy the Repository URL
•	Copy the URL: https://github.com/sgsbelle/Investigating-the-Impact-of-Ventilation-on-Nocturnal-Air-Quality-in-Urban-Environments.git
Step 2: Clone the Repository
•	Open your terminal (or Git Bash on Windows).
•	Navigate to the directory where you want to clone the repository.
•	Example (if you want to clone it to the Documents directory):
•	cd ~/Desktop
1.	Run the following command in your terminal to clone the repository:
•	git clone https://github.com/YourUsername/Desktop/ Investigating-the-Impact-of-Ventilation-on-Nocturnal-Air-Quality-in-Urban-Environments.git
2.	Once the command completes, you will have a local copy of the repository in a new folder with the repository's name.
Comments: Ensure all necessary software and python libraries are installed.
