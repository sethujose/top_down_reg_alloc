# Top Down Register Allocator

This is the implementation of a top down register allocator. It reads the instructions from the input file 'input.txt' and writes the register allocated code to output.txt file. 

It generates the statics and detailed information about the register allocation in top_dwn_reg_alloc_report.txt file. 

The allocator allocates registers to the most frequently used variables. The frequency of variables is available in the top_dwn_reg_alloc_report.txt file. If the number of variable is less than or equal to available registers, then all registers are used for general purpose computation. If the number of variables is greater than the register count, then 3 registers are used for handling register spilling.

The input instruction file to be converted and the register count should be specified as command line argument to the exectuable.

# Files
- list.c: implements the linked list to store the variables which are encountered in the input instructions
- merge.c: sorts the linked list based on the use frequency of the variables.
reg_alloc.c : performs the entire top down allocation

# Preqrequisites
To build the module, following tools are needed:
- gcc

This allocator was developed using gcc (7.3.0)

# Build
To clean build the top down allocator, the following commands can be used:

make clean
make 

# Run
Allocator can be run by executing the output file top_down as follows:

./top_down input.txt 10

where 10 is the number of registers to be used.

