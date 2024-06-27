import pandas as pd 
import matplotlib.pyplot as plt

# Load the CSV file into a pandas DataFrame
csv_file = "./matrix_multiplication_blocking/performance_blocking.csv"
df = pd.read_csv(csv_file)

# Extract data from the DataFrame
array_size = df["SIZE"]
implementation = df["IMPLEMENTATION"]
gflops = df["GFLOPS"]

blocking_size_k = array_size[::3]
blocking_gflops_k = gflops[::3]

blocking_size_m = array_size[1::3]
blocking_gflops_m = gflops[1::3]

non_blocking_size = array_size[2::3]
non_blocking_gflops = gflops[2::3]

# Create a scatter plot
plt.plot(blocking_size_k, blocking_gflops_k, marker='o', color='blue', label='blocking k')
plt.plot(blocking_size_m, blocking_gflops_m, marker='o', color='green', label='blocking m')
plt.plot(non_blocking_size, non_blocking_gflops, marker='o', color='orange', label='non-blocking')

# Customize the plot
plt.title("GFLOPS vs Array Size")
plt.xlabel("Array Size (bytes)")
plt.xscale("log", base=2)
plt.ylabel("GFLOPS")
plt.grid(True)
plt.legend()

# Save the plot as an image file
output_image = "bandwidth_vs_array_size_blocking.png"
plt.savefig(output_image)