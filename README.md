# **Recurrence Quantification Analysis (RQA) from File**

## 📌 Overview
This program performs **Recurrence Quantification Analysis (RQA)** on time series data stored in a `.dat` file. It calculates **recurrence plots (RPs)** and various **recurrence quantifiers**, such as:

- **Recurrence Rate (RR)**
- **Determinism (DET)**
- **Laminarity (LAM)**
- **Trapping Time (TT)**
- **Entropy of diagonal and vertical structures (ENTR_L, ENTR_V)**
- **Average and Maximum diagonal/vertical line lengths (L_MED, L_MAX, V_MED, V_MAX)**
- **Divergence (DIV)**

The program dynamically determines the **size of the recurrence matrix** based on the number of lines in the input file.

---

## 🛠 Compilation & Execution

### 🔹 Prerequisites
Ensure you have the following dependencies installed:

- **GCC** (GNU Compiler Collection)
- **GSL** (GNU Scientific Library) → Required for numerical computations
- **OpenMP** → Enables parallel computation (if applicable)
- **GNUPlot** (optional, for visualization)

### 🔹 Build Instructions
To **compile** and **run the program**, simply type:
```sh
make
```

This will execute the compiled program and process the default **`data.dat`** file.

### 🔹 Cleaning Up
To remove compiled files and reset the directory, run:
```sh
make clean
```

---

## 📂 Project Structure
```
project_root/
 ├── Makefile                    # Build system
 ├── README.md                    # Documentation (this file)
 ├── general_RQA_from_file        # Compiled executable (after `make`)
 ├── general_RQA_from_file.c      # Main program file
 ├── lib/                          # Library folder
 │   ├── recurrence_functions.c    # Recurrence matrix & quantifiers
 │   ├── auxiliary_functions.c     # Utility functions
 │   ├── gnuplot_functions.c       # Visualization functions
 │   ├── common.h                  # Common headers
 │   ├── recurrence_functions.h    # Header file for RQA functions
 │   ├── auxiliary_functions.h     # Header file for utilities
 │   ├── gnuplot_functions.h       # Header file for plotting
 ├── data.dat                      # Sample input file (time series)
 ├── rm.dat                        # Recurrence matrix output
 ├── rp.dat                        # Recurrence plot output
 ├── RQA_results/                  # Folder for recurrence quantifier results
```

---

## 📜 Input File Format (`data.dat`)
The input `.dat` file should contain **two columns** with numerical data:
```
X1   Y1
X2   Y2
X3   Y3
...
XN   YN
```
Each row represents a point in the **phase space**.

---

## 📊 Output Files
The program generates:
- **`rm.dat`** → The full **recurrence matrix** (binary format: `0` or `1`)
- **`rp.dat`** → The **recurrence plot** (only `1` points)
- **`RQA_results/`** → Contains `.dat` files for each recurrence quantifier.

---

## 📈 Visualization
If **GNUPlot** is installed, recurrence plots can be generated automatically using:
```sh
gnuplot -e "plot 'rp.dat' using 1:2 with dots"
```
Alternatively, use **Python or MATLAB** for custom visualization.

---

## 💡 Customization

### **Changing the Recurrence Threshold (`eps`)**
Modify the global variable in `general_RQA_from_file.c`:
```c
double eps = 0.1;  // Set your desired threshold
```
Alternatively, **adjust dynamically** using a recurrence rate percentage:
```c
bool define_threshold_via_percentage = true;
double percentage = 20.0;  // Target recurrence rate
```

### **Choosing a Distance Metric**
Modify `calculate_distance_matrix()` in `recurrence_functions.c`:
```c
calculate_distance_matrix(orbit, N, 2, DM, "euclidean");  // Default
```
Supported norms:
- `"euclidean"` (default)
- `"manhattan"`
- `"supremum"`
- `"modulated"`

---

## 📬 Contact & Issues
For **bug reports** or **feature requests**, feel free to contact me (matheuspalmero@gmail.com) or open an issue in the repository.

🚀 **Happy Analyzing!**

