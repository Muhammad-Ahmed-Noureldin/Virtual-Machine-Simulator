==============================================================================
                          VOLE MACHINE SIMULATOR
==============================================================================

Cairo University
Faculty of Computers and Artificial Intelligence (FCAI)
CS213 - Object Oriented Programming
Object-Oriented Design Evaluation Report - October 2026

Under the supervision of: Prof. Mohammad El-Ramly


------------------------------------------------------------------------------
1. ABOUT THE PROJECT
------------------------------------------------------------------------------

The Vole Machine Simulator is a C++ program that simulates the Vole machine,
a small teaching computer with a very simple architecture. It loads a program
written in Vole machine language (hexadecimal instructions), then executes it
one instruction at a time, or all at once, while showing the state of the CPU,
the registers, the main memory and the screen.

The project uses an object-oriented design: every hardware part of the machine
(ALU, memory, registers, program counter, instruction register, control unit,
CPU, screen) is its own class, and a Qt Widgets graphical interface sits on
top of them.


------------------------------------------------------------------------------
2. TEAM MEMBERS
------------------------------------------------------------------------------

Name                                   ID         Role
-------------------------------------  ---------  -------------------------
Muhammad Ahmed Noureldin Abdelghany    20251324   Team Leader
Nabil Mohamed Bahget Mohamed           20250699   Team Member
Abdelrahman Haithem Mostafa Said       20250378   Team Member
Eslam Ahmed Abdelaziz Ahmed            20250083   Team Member

TEAM LEADER: Muhammad Ahmed Noureldin Abdelghany is the only team leader of
this project. All other members are team members.

Team Leader phone number: 01114102049  (Muhammad Ahmed Noureldin Abdelghany)


------------------------------------------------------------------------------
3. WHO DID WHAT
------------------------------------------------------------------------------

MUHAMMAD AHMED NOURELDIN ABDELGHANY (20251324) - TEAM LEADER
  - Created the GitHub repository.
  - Created the project plan that the team followed.
  - ALU class (Muhammad.h / Muhammad.cpp): floating-point decode and encode,
    two's complement addition, floating-point addition, bitwise OR / AND /
    XOR, and rotate right.
  - Control Unit (CU) class (Muhammad.h / Muhammad.cpp): fetch,
    decode-and-execute for every opcode, single stepping and halt handling.
  - GUI (gui.cpp): the Qt Widgets front end.

ESLAM AHMED ABDELAZIZ AHMED (20250083)
  - Memory class (Eslam.h / Eslam.cpp): the 256-cell main memory with read,
    write and display_matrix.
  - Screen class (Eslam.h / Eslam.cpp): the output buffer with write,
    display and clear.

ABDELRAHMAN HAITHEM MOSTAFA SAID (20250378)
  - programCounter class (Abdelrahman.h / Abdelrahman.cpp): set, get and
    increment.
  - registers class (Abdelrahman.h / Abdelrahman.cpp): the 16 general-purpose
    registers with read, write and display.
  - voleMachine class (Abdelrahman.h / Abdelrahman.cpp): loading a program
    from a text file into memory (load_program_from_file) and the machine's
    display_state.

NABIL MOHAMED BAHGET MOHAMED (20250699)
  - InstructionRegister class (Nabil.h / Nabil.cpp): stores the current
    instruction and extracts the opcode and operand fields (get_opcode,
    get_r, get_s, get_t, get_xy, get_x).
  - CPU class (Nabil.h / Nabil.cpp): ties the registers, program counter,
    instruction register, ALU and control unit together (step,
    display_state).
  - Created BOTH project videos (two videos in total).

SHARED BY THE WHOLE TEAM
  - Team meetings, and the final code review before submission.
  - The README files (.txt and .md), planned for Thursday 5/11.


------------------------------------------------------------------------------
4. DEVELOPMENT PLAN
------------------------------------------------------------------------------

The plan below was created by Muhammad and followed by the team.

  Fri 30/10/2026   ALU, Memory, Program Counter, Instruction Register
  Sat 31/10/2026   Control Unit, Screen, Registers
  Mon  2/11/2026   CPU
  Tue  3/11/2026   GUI, VoleMachine
  Thu  5/11/2026   README (.txt) and (.md), video
  Fri  6/11/2026   Final code review and submission


------------------------------------------------------------------------------
5. THE VOLE MACHINE
------------------------------------------------------------------------------

  Main memory        256 cells, each 1 byte, addressed 00 to FF
  Registers          16 general-purpose registers, R0 to RF, each 1 byte
  Instruction size   2 bytes (4 hexadecimal digits)
  Program counter    Address of the next instruction; starts at 00 and
                     increases by 2 after each fetch
  Screen             Memory-mapped output: storing a register to address FF
                     prints it on the screen

Execution cycle (one step):
  1. FETCH   - read the two bytes at the program counter into the instruction
               register, then add 2 to the program counter.
  2. DECODE  - split the instruction into opcode and operands.
  3. EXECUTE - perform the operation.

The machine halts on a C000 instruction. Any opcode that is not recognised
also halts the machine.


------------------------------------------------------------------------------
6. INSTRUCTION SET
------------------------------------------------------------------------------

R, S, T are register numbers (one hex digit each).
XY is an 8-bit value or memory address (two hex digits).

  1RXY   LOAD register R with the contents of memory cell XY
  2RXY   LOAD register R with the value XY
  3RXY   STORE the contents of register R into memory cell XY
         (if XY = FF, the value is written to the screen)
  40RS   MOVE the contents of register R into register S
  5RST   ADD registers S and T as two's complement integers, result in R
  6RST   ADD registers S and T as floating-point numbers, result in R
  7RST   R = S OR T
  8RST   R = S AND T
  9RST   R = S XOR T
  AR0X   ROTATE register R to the right by X steps
  BRXY   JUMP to address XY if register R equals register R0
  C000   HALT


------------------------------------------------------------------------------
7. FLOATING-POINT FORMAT
------------------------------------------------------------------------------

Floating-point values use 8 bits:

  Bit 7      Bits 6-4              Bits 3-0
  Sign       Exponent (excess-4)   Mantissa (4 bits)
  (1 = neg)

  - Decoding:  value = (mantissa / 16) * 2^(exponent - 4), negated if the
    sign bit is set.
  - Encoding normalises the value so the fraction lies in [0.5, 1), adds 4 to
    the exponent, and keeps the top 4 bits of the mantissa.
  - An exponent that overflows is clamped to the largest representable value;
    an exponent that underflows becomes 00.


------------------------------------------------------------------------------
8. PROJECT STRUCTURE AND CLASS OVERVIEW
------------------------------------------------------------------------------

  File                                 Contents                       Author
  -----------------------------------  -----------------------------  --------
  Muhammad.h / Muhammad.cpp            ALU, CU                        Muhammad
  Eslam.h / Eslam.cpp                  Memory, Screen                 Eslam
  Abdelrahman.h / Abdelrahman.cpp      programCounter, registers,     Abdelrahman
                                       voleMachine
  Nabil.h / Nabil.cpp                  InstructionRegister, CPU       Nabil
  gui.cpp                              Qt Widgets front end           Muhammad

ALU (Muhammad)
  Performs all arithmetic and logic on 8-bit values.
  - decode(int) / encode(float): convert between the 8-bit floating-point
    format and a C++ float.
  - add_twos_complement(a, b): integer addition, masked to 8 bits.
  - add_floating_point(a, b): decodes both operands, adds them, encodes the
    result.
  - bitwise_or, bitwise_and, bitwise_xor: bitwise operations, masked to 8 bits.
  - rotate_right(val, steps): 8-bit circular right rotation.

CU - CONTROL UNIT (Muhammad)
  Drives the fetch-decode-execute cycle. It holds pointers to the program
  counter, instruction register, memory, registers, ALU and screen.
  - fetch(): reads two bytes from memory at the program counter, advances the
    counter by 2, and builds a 4-digit hex string stored in the instruction
    register.
  - decode_and_execute(): reads the fields from the instruction register and
    carries out the operation for the opcode.
  - step(): fetches and executes one instruction unless the machine is halted.
  - is_halted(): reports whether the machine has halted.

Memory (Eslam)
  A fixed array of 256 integer cells, all initialised to 0.
  - read(address) / write(address, value): access one cell. An out-of-range
    address terminates the program.
  - display_matrix(): prints all cells with their addresses in hexadecimal.

Screen (Eslam)
  Collects everything the program writes to address FF.
  - write(value): appends a value to the output buffer.
  - display(): prints the buffered values separated by spaces.
  - clear(): empties the buffer.

programCounter (Abdelrahman)
  - set(address), get(), and increment() (adds 2, because every instruction
    is 2 bytes).

registers (Abdelrahman)
  Sixteen registers stored in a vector.
  - read(index) / write(index, value): access one register.
  - display(): prints a table of all registers in hexadecimal.

voleMachine (Abdelrahman)
  Connects the machine to the outside world.
  - load_program_from_file(path, address): reads a text file, skips blank
    lines and lines starting with ';', takes the first four hex digits of
    each remaining line as an instruction, and writes the two bytes to memory
    starting at the given address.
  - display_state(): reserved for printing the machine state.

InstructionRegister (Nabil)
  Stores the current instruction as a 4-character hex string and extracts its
  fields:
  - get_opcode(): the first hex digit.
  - get_r(), get_s(), get_t(): register operands, taken from the correct digit
    for each instruction format.
  - get_xy(): the 8-bit value or address used by LOAD, STORE and JUMP.
  - get_x(): the rotation count used by ROTATE.

CPU (Nabil)
  Groups the registers, program counter, instruction register, ALU and
  control unit.
  - step(): runs one instruction through the control unit.
  - display_state(): prints the program counter and the register table.


------------------------------------------------------------------------------
9. HOW THE COMPONENTS WORK TOGETHER
------------------------------------------------------------------------------

                 +---------------------------+
                 |        voleMachine        |  loads program text into memory
                 +-------------+-------------+
                               |
   +---------+      +----------v-----------+      +----------+
   | Memory  |<---->|          CU          |----->|  Screen  |
   +---------+      |  (fetch / decode /   |      +----------+
                    |       execute)       |
   +---------+      |                      |      +----------+
   |registers|<---->|                      |----->|   ALU    |
   +---------+      +----+-------------+---+      +----------+
                         |             |
              +----------v---+      +--v--------------------+
              |programCounter|      |  InstructionRegister  |
              +--------------+      +-----------------------+

  The CPU class groups: registers, programCounter, InstructionRegister,
  ALU and CU.


------------------------------------------------------------------------------
10. THE GRAPHICAL USER INTERFACE
------------------------------------------------------------------------------

gui.cpp is a Qt Widgets application titled "Vole Machine Simulator (FCAI
Cairo University)". A Machine structure owns every simulator component and
wires them together.

Panels
  - CPU State: the program counter, the instruction register, and all 16
    registers.
  - Source & Debugger: the loaded program, one instruction per line, with a
    short comment describing each one and an arrow marking the instruction
    that will execute next.
  - Console & Screen Output: the value most recently written to the screen.
  - Output Log: a step-by-step description of every executed instruction, and
    any errors.
  - Main Memory Viewer: a 16 x 16 grid of all 256 memory cells. Program bytes
    are highlighted, and the screen cell (FF) is shown separately.

Controls
  - Load File: choose a program text file to load.
  - Clear Memory: reset memory, registers, screen and the program counter.
  - Run All: run until the program halts (stops after 100,000 steps to guard
    against infinite loops).
  - Single Step: execute one instruction.
  - Help: shows the instruction set.
  - A status indicator shows READY, RUNNING, STEPPING or HALTED.

All actions are also available from the menu bar.


------------------------------------------------------------------------------
11. PROGRAM FILE FORMAT
------------------------------------------------------------------------------

A program is a plain text file with one instruction per line, written as four
hexadecimal digits. Blank lines and lines beginning with ';' are ignored. Text
after the four digits on a line is ignored, so it can be used for comments.
Instructions are loaded into memory from address 00.


------------------------------------------------------------------------------
12. BUILDING AND RUNNING
------------------------------------------------------------------------------

Platform: Windows.

Requirements: Qt 5 or Qt 6 for Windows with the Widgets module, together with
its MinGW (or MSVC) compiler. Both are installed by the Qt Online Installer,
which also installs Qt Creator.

All source files must be compiled together:

  Muhammad.cpp  Eslam.cpp  Nabil.cpp  Abdelrahman.cpp  gui.cpp

Option 1 - Qt Creator (recommended):
  1. Open Qt Creator and choose File > New Project > Application (Qt) >
     Qt Widgets Application, or any empty Qt project.
  2. Add all the project's .cpp and .h files (Muhammad, Eslam, Nabil,
     Abdelrahman and gui.cpp) to the project.
  3. Make sure the project file contains: QT += widgets
  4. Select a Qt kit for Windows (MinGW or MSVC) and press Ctrl+R to build
     and run.

Option 2 - Command line (Qt for Windows, MinGW):

  Create a file named VoleSimulator.pro in the project folder:

    QT += widgets
    CONFIG += c++17
    SOURCES += Muhammad.cpp Eslam.cpp Nabil.cpp Abdelrahman.cpp gui.cpp
    HEADERS += Muhammad.h Eslam.h Nabil.h Abdelrahman.h

  Then open the "Qt MinGW" command prompt (from the Start menu), go to the
  project folder, and run:

    qmake VoleSimulator.pro
    mingw32-make
    release\VoleSimulator.exe

  If the program does not start because of missing DLLs, run
  "windeployqt release\VoleSimulator.exe" once to copy the Qt libraries next
  to it.

Using the simulator:
  1. Click Load File and choose a program text file.
  2. Click Single Step to execute one instruction at a time, or Run All to
     run to completion.
  3. Watch the registers, memory, screen output and log update.
  4. Click Clear Memory to start again.


------------------------------------------------------------------------------
13. EXAMPLE PROGRAM
------------------------------------------------------------------------------

  2105 ; R1 = 05
  2203 ; R2 = 03
  5312 ; R3 = R1 + R2
  33FF ; write R3 to the screen
  C000 ; halt

Expected result: the screen shows 8, register R3 holds 08, and the machine
halts.


==============================================================================
 Cairo University - FCAI - CS213 Object Oriented Programming - October 2026
==============================================================================
