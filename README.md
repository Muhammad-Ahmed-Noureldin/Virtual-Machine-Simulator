# CS213 Object-Oriented Programming: Assignment 2
## Vole Machine Simulator

**Faculty of Computers and Artificial Intelligence, Cairo University**  
**Course:** CS213 - Object Oriented Programming (First Semester, 2026/2027)  
**Instructor:** Dr. Mohammad El-Ramly  

---

## 👥 Team Members
* **Muhammad Ahmed Noureldin Abdelghany** – `20251324`
* **Nabil Mohamed Bahget Mohamed** – `20250699`
* **Abdelrahman Haithem Mostafa Said** – `20250378`
* **Eslam Ahmed Abdelaziz Ahmed** – `20250083`

---

## 📌 Project Overview
This project implements an Object-Oriented Programming (OOP) simulator for the **Vole Machine** and its machine language, built in standard C++. The simulator models a complete computer architecture including:
* **CPU Components:** 16 General-Purpose Registers ($R_0$ to $R_F$), a Program Counter (PC), and an Instruction Register (IR).
* **Main Memory:** A 256-byte memory organized as a $16 \times 16$ grid (addresses ranging from `00` to `FF`).
* **Screen Output:** Memory mapping at location `FF` for direct text/screen output display.
* **Control Unit:** Supports the complete Fetch-Decode-Execute cycle with options for step-by-step or continuous execution, validation of assembly instructions, and robust error handling.

---

## ⚙️ Supported Vole Machine Instructions

| Op-code | Format | Description |
| :---: | :---: | :--- |
| **1** | `RXY` | **LOAD** register $R$ with the bit pattern from memory cell address $XY$. |
| **2** | `RXY` | **LOAD** register $R$ with the immediate bit pattern $XY$. |
| **3** | `RXY` | **STORE** the bit pattern from register $R$ into memory cell address $XY$. |
| **3** | `RFF` | **STORE** to location `FF` (screen output mapping). |
| **4** | `R0S` | **MOVE** the bit pattern from register $S$ to register $R$. |
| **5** | `RST` | **ADD** registers $S$ and $T$ as two's complement integers, storing result in $R$. |
| **6** | `RST` | **ADD** registers $S$ and $T$ in floating-point notation, storing result in $R$. |
| **7** | `RST` | **OR** bit patterns in registers $S$ and $T$, storing result in $R$. |
| **8** | `RST` | **AND** bit patterns in registers $S$ and $T$, storing result in $R$. |
| **9** | `RST` | **EXCLUSIVE OR** bit patterns in registers $S$ and $T$, storing result in $R$. |
| **A** | `R0X` | **ROTATE** the bit pattern in register $R$ one bit to the right $X$ times circularly. |
| **B** | `RXY` | **JUMP** to memory address $XY$ if the value in register $R$ equals register $0$. |
| **C** | `000` | **HALT** execution. |

---

## 🛠️ Project Structure & Architecture
The project follows strict OOP principles and separate compilation (header `.h` files and implementation `.cpp` files):
* `Machine` / `CPU`: Manages registers, Program Counter, and Instruction Register.
* `Memory`: Handles the 256-byte storage array, data loading, and screen mapping.
* `ALU`: Implements arithmetic, logical, and floating-point operations.
* `ControlUnit` / `Instruction`: Decodes and executes machine instructions while validating syntax.
* `App`: Manages the user interface, menus, file loading, and execution loops (step-by-step or run-at-once).

---

## 🚀 How to Compile and Run

### Prerequisites
* A C++ compiler supporting **C++17** or higher (e.g., `g++`, `clang`, or MSVC).
* Make or CMake (optional, or compile manually via terminal).

### Compilation (Manual via G++)
Open your terminal in the source directory and run:
```bash
g++ -std=c++17 src/*.cpp -o VoleSimulator
```

### Execution
Run the compiled executable:
```bash
./VoleSimulator
```
*(On Windows Command Prompt)*:
```cmd
VoleSimulator.exe
```

---

## 📝 How to Use the Simulator
1. Launch the application from your terminal or GUI interface.
2. Choose from the interactive menu:
   * **Load Program:** Load a `.txt` Vole assembly file into memory (starting at an even memory address).
   * **Run Program:** Execute the loaded program continuously until a `HALT` instruction (`C000`) is reached.
   * **Single Step:** Execute instructions one by one, inspecting register states, memory matrices, and PC updates after each step by pressing `Enter`.
   * **Modify State:** Overwrite specific register or memory cell values manually.
3. Observe real-time output updates (including memory state matrices and screen displays).


