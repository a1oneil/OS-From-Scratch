// kernel.c

void printString(char*);
void printChar(char);
void readString(char*);
void readSector(char*, int);
void handleInterrupt21(int, int, int, int);


void main() {

//char buffer[512];
char line[80];
printString("Enter a line: ");
makeInterrupt21();
interrupt(0x21, 1, line, 0,0);
interrupt(0x21, 0, line, 0, 0);
//printString("Hello World | ");

//printString("Enter a line: ");
//readString(line);
//printString(line);

//readSector(buffer, 30);
//printString(buffer);

//makeInterrupt21();
//interrupt(0x21, 0, 0, 0, 0);
while(1);
}

void printString(char* chars)
{
while (*chars != 0x0){
 interrupt(0x10, 0xe*256+*chars, 0, 0, 0);
 chars++;
}
}
// prints single character
void printChar(char c)
{
interrupt(0x10, 0xe*256+c, 0, 0, 0);
}

// step 2
void readString(char* userCommand)
{
int i = 0;

char enterKey = 0xd;
char lineFeed = 0xa;
char stringEnd = 0x0;
char backKey = 0x8;

char reader = interrupt(0x16, 0, 0, 0, 0);
interrupt(0x10, 0xe*256+reader, 0, 0, 0);

while(reader != enterKey)
{
 if(reader == backKey && i > 0)
 {
  i--;
 }
 else if(reader == backKey)
 {
  i = 0;
 }
 else
 {
 userCommand[i] = reader;
 i++;
 }

 reader = interrupt(0x16, 0, 0, 0, 0);
 interrupt(0x10, 0xe*256+reader, 0, 0, 0);

 if(reader == backKey)
 {
  interrupt(0x10, 0xe*256+stringEnd, 0, 0, 0);
  interrupt(0x10, 0xe*256+backKey, 0, 0, 0);
 }
}

userCommand[i] = lineFeed;

userCommand[i] = lineFeed;
i++;
userCommand[i] = stringEnd;

interrupt(0x10, 0xe*256+lineFeed, 0, 0, 0);
}

// step 3
void readSector(char* buffer, int sector){

int AH = 2;
int AL = 1;
int AX = AH*256+AL;
int BX = buffer;
int CH = 0;
int CL = sector + 1;
int CX = CH*256+CL;
int DH = 0;
int DL = 0x80;
int DX = DH*256+DL;

interrupt(0x13, AX, BX, CX, DX);

}

void handleInterrupt21(int AX, int BX, int CX, int DX){
if (AX==0)
printString(BX);
else if(AX==1)
readString(BX);
else if(AX==2)
readSector(BX, CX);
else
printString("Error!");
}
