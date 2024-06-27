import pandas as pd
import matplotlib.pyplot as plt

# Load the CSV file into a pandas DataFrame
csv_file = "mem_bandwidth.csv"
df = pd.read_csv(csv_file)

# Extract data from the DataFrame
array_size = df["Array Size (bytes)"]
bandwidth = df["Bandwidth (GB/s)"]

# Create a scatter plot
plt.plot(array_size, bandwidth, marker='o', color='blue', label='Bandwidth (GB/s)')

# Customize the plot
plt.title("Bandwidth vs Array Size")
plt.xlabel("Array Size (bytes)")
plt.xscale("log", base=2)
plt.ylabel("Bandwidth (GB/s)")
plt.grid(True)
plt.legend()

# Save the plot as an image file
output_image = "bandwidth_vs_array_size.png"
plt.savefig(output_image)
