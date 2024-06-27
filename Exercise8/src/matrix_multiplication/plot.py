import pandas as pd 
import matplotlib.pyplot as plt

# Load the CSV file into a pandas DataFrame
csv_file = "./performance.csv"
df = pd.read_csv(csv_file)

# Extract data from the DataFrame
array_size = df["M=N=K"]
implementation = df["IMPLEMENTATION"]
gflops = df["GFLOPS"]

mnk_size = array_size[::6]
mnk_gflops = gflops[::6]

mkn_size = array_size[1::6]
mkn_gflops = gflops[1::6]

nmk_size = array_size[2::6]
nmk_gflops = gflops[2::6]

nkm_size = array_size[3::6]
nkm_gflops = gflops[3::6]

kmn_size = array_size[4::6]
kmn_gflops = gflops[4::6]

knm_size = array_size[5::6]
knm_gflops = gflops[5::6]

# Create a scatter plot
plt.plot(mnk_size, mnk_gflops, marker='o', color='blue', label='mnk')
plt.plot(mkn_size, mkn_gflops, marker='o', color='orange', label='mkn')
plt.plot(nkm_size, nkm_gflops, marker='o', color='green', label='nkm')
plt.plot(nmk_size, nmk_gflops, marker='o', color='red', label='nmk')
plt.plot(kmn_size, kmn_gflops, marker='o', color='yellow', label='kmn')
plt.plot(knm_size, knm_gflops, marker='o', color='brown', label='knm')

# Customize the plot
plt.title("GFLOPS vs Array Size")
plt.xlabel("Array Size (bytes)")
plt.xscale("log", base=2)
plt.ylabel("GFLOPS")
plt.grid(True)
plt.legend()

# Save the plot as an image file
output_image = "bandwidth_vs_array_size.png"
plt.savefig(output_image)