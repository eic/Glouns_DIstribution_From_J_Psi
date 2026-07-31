import pandas as pd
import numpy as np
import os

files = [
    "Uncertainty_files_April_2026/Combined_Cross_Section_HFS_12_28.csv",
    "Uncertainty_files_April_2026/Combined_Cross_Section_HFS_15_25.csv",
    "Uncertainty_files_April_2026/Combined_Cross_Section_HFS_18_22.csv",
]

outdir = "Uncertainty_files_April_2026"
os.makedirs(outdir, exist_ok=True)

# Read only the second column, skipping header automatically
data = []

for f in files:
    df = pd.read_csv(f)

    # second column = cross section
    values = df.iloc[:, 1].to_numpy(dtype=float)

    data.append(values)

data = np.array(data)  # shape: 3 files x N bins

# Mean and standard deviation across the 3 files, bin-by-bin
mean = np.mean(data, axis=0)
std_dev = np.std(data, axis=0, ddof=1)   # sample standard deviation

# Bin number
bins = np.arange(1, len(mean) + 1)

print(" ")
out = pd.DataFrame({
    "Bin": bins,
    "Mean_Cross_Section": mean,
    "Std_Dev": std_dev,
    "Rel_Std_Dev_percent": 100.0 * std_dev / mean
})

out.to_csv(f"{outdir}/HFS_Cut_Cross_Section_StdDev.csv", index=False)

print("Saved:")
print(f"{outdir}/HFS_Cut_Cross_Section_StdDev.csv")
print(" ")
