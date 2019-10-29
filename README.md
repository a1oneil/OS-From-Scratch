- // entire project is in progress, this only accounts for projects A & B out of A,B,C,D,E.
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
