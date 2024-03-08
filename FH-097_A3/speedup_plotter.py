import matplotlib.pyplot as plt

# Read CPU times from output.txt
cpu_times = []
with open("output.txt", "r") as file:
    for line in file:
        cpu_times.append(float(line.strip()))

# Extract sequential, parallel, and dynamic CPU times
sequential_time = cpu_times[0]
parallel_times = cpu_times[1:8]
dynamic_times = cpu_times[8:16]

# Calculate speedup
speedup_parallel = [sequential_time / time for time in parallel_times]
speedup_dynamic = [sequential_time / time for time in dynamic_times]

# Number of workers (cores)
num_workers = [2, 4, 6, 8, 16, 32, 64]

# Create a plot
plt.plot(num_workers, speedup_parallel, label="Parallel Speedup")
plt.plot(num_workers, speedup_dynamic, label="Dynamic Speedup")
plt.xlabel("Number of Workers (Cores)")
plt.ylabel("Speedup")
plt.legend()
plt.title("Speedup vs. Number of Workers")
plt.grid()
plt.savefig("speedup_plot.png")  # Save plot to a file
plt.show()
