- // Dr. Black's COMP350 Operating System from Scratch Project
- // entire project is in progress, this only accounts for projects A, B & C out of A,B,C,D,E.

## Project B
- // Arianna O’Neil, Logan Teceno, Sam Silvia, Chris Roppolo
- // Dr. Black’s COMP 350 Project B

## Step 1: Printing to the Screen - Interrupt 0x10 (Logan)
Printing to the Screen w/ printString

- Using Interrupt 0x10, we were able to print a String to the screen using the printString function, which takes an array of characters as a parameter.
- If you type out printString(“Hello World”), when the OS boots up, “Hello World” will be displayed on the screen.
- Additionally, printChar, with a parameter of a single character.

## Step 2: Reading from the keyboard - Interrupt 0x16 (Logan)
Reading from the keyboard w/ readString

- Using Interrupt 0x16, along with the readString function, a character array is taken as a parameter.
- The Interrupt 0x16 returns the ASCII code of the key pressed on the keyboard.
- The Interrupt is run repeatedly and saved to the character array, until the ENTER key is pressed.
- To test this, when the OS boots up, if readString is used, it should prompt you to enter a line. When you press ENTER, it will repeat what you typed on the next line.

## Step 3: Read a sector from the disk - Interrupt 0x13 (Logan & Arianna)
Read a sector from the disk w/ readSector

- Using Interrupt 0x13 we were able to read/write sectors from the disk and took the following parameters int AH, AL, AX, BX, CH, CL, CX, DH, DL, DX.
- We created a text file and placed it at sector 30.
- Then, we called the Interrupt and read the sector into the buffer that was initialized in main.
- After compiling, the text file was printed onto the screen.

## Step 4: Create your own interrupt (Arianna & Sam)
Creating our own interrupts

- We wrote a function handleInterrupt21 that took parameters (int ax, int bx, int cx, int dx).
- We called to the function printString, and when the interrupt was called printString was the designated output
- To test this, we called makeInterrupt21 and 0x21in main and, if working properly, the message written in the handleInterrupt21 function was displayed

## Step 5: Make printString, readString, and readSector interrupt calls (Arianna & Sam)
Created interrupt calls for printString, readString, and readSector

- handleInterrupt21 waited for user input to read a line, after input it echoed back the user’s text.
- We created various if/else statements in the function that called to printString, readString, and readSector.
- AX is a number that determines which function to run
    - if (AX==0), then printString(BX)
    - if(AX==1), then readString(BX)
    - if(AX==2), then readSector(BX, CX)
    - if(AX > 3 || < 0), printString(“Error!”) 
    - [Since we only account for three functions]
- Depending on what value AX is, a certain function (as detailed above) will run.


## Project C

- // Arianna O’Neil, Logan Teceno, Sam Silvia, Chris Roppolo
- // Dr. Black’s COMP 350 Project C

## Step 1: Read a file and print it (Logan & Arianna)
readFile takes a character array containing a file name and reads the file into a buffer

- We created a new function, readFile(), which reads a file into a buffer using a series of for loops.
- readFile interrupt 21 call
    - AX == 3
    - BX == address of file name (in char array)
    - CX == address of a buffer to hold the file
    - DX == address of an int to hold the number of sectors read
- Using readSector, load the directory sector 512 byte char array
- Iterate through the directory to match the file name
    - If it is not found, set the sector number to 0 and return
- Using sector numbers in the dir, load file by sector into the buffer array
    - Add 512 to the buffer address every time readSector 
    - Increment the number of sectors read

## Step 2: Load a program and execute it (Logan)
Load a program into memory and execute it

- We created a new function, executeProgram, which loads a program into memory and executes it.
    - It takes a char* as a parameter.
- This happens by loading the file into a buffer using readFile, then transferring the file from the buffer into memory.
- From there, we used the launchProgram function from assembly to run the program.
- We then made a new interrupt 0x21 call, which handles this function.

## Step 3: Terminate program system call (Arianna)
When a user program finishes, it should make an interrupt 0x21 call to return to the operating system

- In this step, the function terminate call just hangs up the computer.
    - Later, terminate will make the operating system reload the shell.
- In void handleInterrupt21 if AX == 5, then the function terminate is called
- We verified this with tstpr2
    - After compiling and loading, tstpr2 does not hang up at the end but instead calls the terminate program interrupt

## Step 4: The Shell (Sam)
Making our own user program

- First you have to create the files pertaining to the shell
    - Create shell.c file
    - Assemble userlib.asm using as86 to create userlib.o
    - Compile the shell.c file  which creates a shell_c.o
    - Link userlib.o and shell_c.o using ld86 to create the shell file
    - Load shell onto disk
-Create an infinite loop that prints out “SHELL>” (this is where the user will type input), then have shell read in the user input and try to match it to a command that we have written
- Since there are no commands written it will print out an error
- Functions made:
    - promptPrint() - “SHELL>” characters are here
    - errorPrint() - “Bad command!” characters are here
    - is___Command() - checks if the user input matches and of the commands, we will write in the next steps and if there is a      match it will pin that to a number
    - recognizeCommand() - uses the number that is pinned to commands and executes the commands

## Step 5: Shell Command “type” (Logan & Chris)
Modified the shell to recognize the command “type filename”

- When the user types “type filename” in the shell, the shell will load the file into memory and print out the contents in the OS.
- If the file is not found, an error message is printed out.

## Step 6: Shell command “exec” (Logan)
Recognize the command “exec filename”

- When the user types “exec filename” in the shell, the shell will call the kernel to load and execute the project through the executeFile function.
- If the file is not found, an error message is printed out.

