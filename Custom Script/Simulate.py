import subprocess
import os

# Path to your compiled Unreal Engine Executable
EXE_PATH = r"C:\Users\Name\Project\Build\Windows\MyUnrealApp.exe"

# Core Parameters
RES_WIDTH = 610
RES_HEIGHT = 520
OUTPUT_FOLDER = r"C:\Users\Name\Project\OutputData"
CAMERA_HEIGHT = 500.0

# Placeholder Parameters (Random names for future use)
TIME_OF_DAY = 1200          # e.g., military time
WEATHER_INTENSITY = 0.5     # e.g., 0.0 to 1.0
OBJECT_DENSITY = 10         # e.g., number of spawned items
SCAN_MODE = "Full"          # e.g., "Full", "Fast", "DepthOnly"
ENABLE_LOGS = True

def launch_unreal_app():
    # Check if the application exists before trying to run it
    if not os.path.exists(EXE_PATH):
        print(f"Error: Executable not found at {EXE_PATH}")
        return

    # Construct the command line arguments
    # Unreal uses -Key=Value format for custom parameters
    cmd = [
        EXE_PATH,
        f"-ResX={RES_WIDTH}",
        f"-ResY={RES_HEIGHT}",
        f"-OutputDir={OUTPUT_FOLDER}",
        f"-CamHeight={CAMERA_HEIGHT}",
        f"-TimeOfDay={TIME_OF_DAY}",
        f"-Weather={WEATHER_INTENSITY}",
        f"-Density={OBJECT_DENSITY}",
        f"-Mode={SCAN_MODE}",
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