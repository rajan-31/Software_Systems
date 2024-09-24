/*
================================================
Author : Rajan Khade

Description :
Write a program to measure how much time is taken to execute 100 getppid ( ) system call. Use time stamp counter
================================================
*/

#include <stdio.h>
#include <unistd.h>

// Inline function to read the Time Stamp Counter
static inline unsigned long long rdtsc() {
    unsigned long long tsc;
    asm volatile("rdtsc" : "=A"(tsc));
    return tsc;
}

int main() {
    unsigned long long start, end;
    pid_t ppid;

    // Get starting TSC value
    start = rdtsc();

    // Execute 100 getppid() system calls
    for (int i = 0; i < 100; i++) {
        ppid = getppid();
    }

    // Get ending TSC value
    end = rdtsc();

    // Calculate and print the number of CPU cycles taken
    printf("Time taken for 100 getppid() calls: %llu CPU cycles\n", end - start);

    return 0;
}


/*
"rdtsc" assembly instruction.
It stands for Read Time-Stamp Counter
The TSC is a CPU register that counts the number of cycles since the last reset

The rdtsc instruction counts CPU cycles, not time in seconds or nanoseconds. If you want the actual time, you would need to convert cycles to time using your CPU's clock speed

This program directly measures the CPU cycles and does not account for multi-core effects, CPU frequency scaling, or context switching.

 ============================

asm volatile("rdtsc" : "=A"(tsc));

asm keyword allows you to include assembly code in a C program. It lets you interact directly with the CPU, bypassing high-level C constructs.

volatile keyword tells the compiler not to optimize this assembly code. Without volatile, the compiler might optimize the code away or reorder it, which could affect timing and results

: "=A"(tsc):
This part is called the output operand section of the inline assembly, which tells the compiler where to store the result of the rdtsc instruction.
=A means that the result will be stored in a 64-bit register pair (eax and edx combined), and that it will be an output-only operand (=).
tsc is the C variable where the result (the number of CPU clock cycles) will be stored. The "=A" constraint combines the 32-bit values from eax and edx into a single 64-bit value.
*/
