import random
import subprocess
import os
import time

# Configuration
EXE_PATH = r"C:\Users\Name\Project\Build\Windows\MyUnrealApp.exe"
OUTPUT_ROOT = r"C:\Users\Name\Project\OutputData\StressTest"

def run_randomized_sim(index):
    output_dir = os.path.join(OUTPUT_ROOT, f"Sim_{index}")
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)

    # Randomize parameters
    params = {
        "-ResX": 640,
        "-ResY": 512,
        "-OutputDir": output_dir,
        "-CamHeight": random.uniform(300, 1000),
        "-RotationSpeed": random.uniform(10, 60),
        "-Weather": random.randint(0, 4),
        "-CharacterCount": random.randint(1, 10),
        "-ArmedCount": random.randint(0, 5),
        "-ArrivalTime": random.uniform(0, 5),
        "-unattended": "",
        "-windowed": ""
    }

    cmd = [EXE_PATH]
    for key, value in params.items():
        if value != "":
            cmd.append(f"{key}={value}")
        else:
            cmd.append(key)

    print(f"[{index}] Launching with: {cmd}")
    process = subprocess.Popen(cmd)
    
    # Wait for simulation to finish (estimated 15 seconds)
    time.sleep(15)
    process.terminate()
    print(f"[{index}] Finished.")

if __name__ == "__main__":
    if not os.path.exists(OUTPUT_ROOT):
        os.makedirs(OUTPUT_ROOT)

    for i in range(5): # Run 5 randomized iterations
        run_randomized_sim(i)
