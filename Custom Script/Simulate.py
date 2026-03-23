import subprocess
import os

# Path to your compiled Unreal Engine Executable
EXE_PATH = r"C:\Users\Name\Project\Build\Windows\MyUnrealApp.exe"

# Core Parameters
RES_WIDTH = 640
RES_HEIGHT = 512
OUTPUT_FOLDER = r"C:\Users\Name\Project\OutputData"
CAMERA_HEIGHT = 500.0 	# Height of the camera above the ground in Unreal units (1 unit = 1 m)
TIME_OF_DAY = 1200 	# Military time
FOV = 90 		# Ranging from 30 to 100
WEATHER = 0     	# An integer representing weather conditions (0 = Clear, 1 = Rainy, 2 = Foggy, etc.)
CAMERA_BLUR = 0 	# Amount of camera blur
ATMOSPHERIC_HAZE = 0 	# Amount of atmospheric haze
CHARACTER_COUNT = 5  	# Number of characters in a vehicle
ARRIVAL_TIME= 0 	# Time in seconds for vehicle arrival (has to fall within the 10 second recording window)
ARMED_COUNT = 2 	# Number of armed characters in a vehicle
ROTATION_SPEED = 30.0 	# Degrees per second for camera rotation
VEHICLE_ID = 0 		# ID of the vehicle to spawn (if applicable)
BUILDING_ID = 0 	# ID of the building to record (if applicable)
MODE = 0 		# An integer representing the sensor type (0 = VIS, 1 = IR, 2 = LWIR)
ENABLE_LOGS = True

def launch_unreal_app():
    # Check if the application exists before trying to run it
    if not os.path.exists(EXE_PATH):
        print(f"Error: Executable not found at {EXE_PATH}")
        return

    # Construct the command line arguments (Unreal uses -Key=Value format for custom parameters)
    cmd = [
        EXE_PATH,
        f"-ResX={RES_WIDTH}",
        f"-ResY={RES_HEIGHT}",
        f"-OutputDir={OUTPUT_FOLDER}",
        f"-CamHeight={CAMERA_HEIGHT}",
        f"-TimeOfDay={TIME_OF_DAY}",
        f"-FOV={FOV}",
        f"-Weather={WEATHER}",
        f"-CharacterCount={CHARACTER_COUNT}",
        f"-CameraBlur={CAMERA_BLUR}",
        f"-AtmosphericHaze={ATMOSPHERIC_HAZE}",
        f"-ArrivalTime={ARRIVAL_TIME}",
        f"-ArmedCount={ARMED_COUNT}",
        f"-RotationSpeed={ROTATION_SPEED}",
        f"-VehicleID={VEHICLE_ID}",
        f"-BuildingID={BUILDING_ID}",
        f"-Mode={MODE}",
        "-windowed",           # Runs in windowed mode
        "-NoTextureStreaming"  # Useful for high-quality data capture
    ]

    if ENABLE_LOGS:
        cmd.append("-log")

    print(f"Launching: {' '.join(cmd)}")
    
    try:
        # Launch the application
        subprocess.Popen(cmd)
        print("Application launched successfully.")
    except Exception as e:
        print(f"An error occurred: {e}")

if __name__ == "__main__":
    launch_unreal_app()