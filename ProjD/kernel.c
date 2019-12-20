// Arianna O'Neil

void printString(char*);
void printChar(char);
void readString(char*);
void readSector(char*,int);
void handleInterrupt21(int,int,int,int);
void readFile(char*,char*,int*);
void executeProgram(char*);
void terminate();
void writeSector(char*,int);
void deleteFile(char*);
void writeFile(char*, char*, int);
int compareCheck(char*, char*);

void main() {
	makeInterrupt21();

	// step 4 goodness
	interrupt(0x21, 8, "I've been having these weird thoughts lately... Like, is any of this for real... or not?\n\r", "testmg", 3);

	interrupt(0x21, 4, "shell", 0, 0);
}

void printString(char* chars)
{
	while (*chars != 0x0) {
		interrupt(0x10, 0xe*256+*chars, 0, 0, 0);
		*chars++;
	}
}

void printChar(char c)
{
	interrupt(0x10, 0xe*256+c, 0, 0, 0);
}

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
	i++;
	userCommand[i] = stringEnd;

	interrupt(0x10, 0xe*256+lineFeed, 0, 0, 0);
}

void readSector(char* buffer, int sector) {
	int AH = 2;
	int AL = 1;
	int AX = AH * 256 + AL;
	int BX = buffer;
	int CH = 0;
	int CL = sector + 1;
	int CX = CH * 256 + CL;
	int DH = 0;
	int DL = 0x80;
	int DX = DH * 256 + DL;

	interrupt(0x13, AX, BX, CX, DX);
}

void handleInterrupt21(int AX, int BX, int CX, int DX) {
	if (AX == 0) {
		printString(BX);
	}
	else if (AX == 1) {
		readString(BX);
	}
	else if (AX == 2) {
		readSector(BX, CX);
	}
	else if (AX == 3) {
		readFile(BX, CX, DX);
	}
	else if (AX == 4) {
		executeProgram(BX);
	}
	else if (AX == 5) {
		terminate();
	}
	else if (AX == 6) {
		writeSector(BX, CX);
	}
	else if (AX == 7) {
		deleteFile(BX);
	}
	else if (AX == 8) {
		writeFile(BX, CX, DX);
	}
	else {
		printString("Uh-oh Error!");
	}
}

void readFile(char* filename, char* buffer, int* sectorsread) {
	int c = 0; // claude
	int d = 0; // dimitri
	int e = 0; // edelgarde
	char fileEntry[32];

	// Step 1-1
	char dir[512];
	*sectorsread = 0;

	readSector(dir, 2);

	// Step 1-2
	for (c = 0; c < 512; c = c + 32) {
		if ((filename[0] == dir[c + 0]) && (filename[1] == dir[c + 1]) && (filename[2] == dir[c + 2]) && (filename[3] == dir[c + 3]) && (filename[4] == dir[c + 4]) && (filename[5] == dir[c + 5])) {
			for (e = 0; dir[c + e + 6] != 0; e++) {
				readSector(buffer, dir[c + e + 6]);
				buffer = buffer + 512;
				*sectorsread = *sectorsread + 1;
			}
		return;
		}
	}
}

void executeProgram(char* name) {
	char buffer[13312];
	int sectorsRead;
	int sora = 0;

	// Step 2-1
	readFile(name, buffer, &sectorsRead);

	// Step 2-2
	for (sora = 0; sora < 13312; sora++) {
		putInMemory(0x2000, sora, buffer[sora]);
	}

	// Step 2-3
	launchProgram(0x2000);
}

void terminate() {
	char shellname[6];

	shellname[0] = 's';
	shellname[1] = 'h';
	shellname[2] = 'e';
	shellname[3] = 'l';
	shellname[4] = 'l';
	shellname[5] = '\0';

	executeProgram(shellname);
}

void writeSector(char* buffer, int sector) {
	int AH = 3;
	int AL = 1;
	int AX = AH * 256 + AL;
	int BX = buffer;
	int CH = 0;
	int CL = sector + 1;
	int CX = CH * 256 + CL;
	int DH = 0;
	int DL = 0x80;
	int DX = DH * 256 + DL;

	interrupt (0x13, AX, BX, CX, DX);
}

void deleteFile(char* fileName) {
	char dir[512];
	char map[512];
	int sephi = 0;
	int roth = 0;
	int sector;

	readSector(map, 1);
	readSector(dir, 2);

	sephi = compareCheck(dir, fileName);
	if (sephi == -1) {
		return;
	}

	for (roth = 0; roth < 26; roth++) {
			sector = (int) dir[sephi * 32 + 6 + roth];
			if (sector == 0) {
				break;
			}
			map[sector] = 0x00;
		}
		dir[sephi * 32] = 0x00;
// one winged angel starts now
	writeSector(map, 1);
	writeSector(dir, 2);
}

void writeFile(char* buffer, char* filename, int numberOfSectors) {
	char map[512];
	char dir[512];
	int i;
	int dirLoc;
	int j;
	int compSect;

	readSector(map, 1);
	readSector(dir, 2);

	for (i = 0; i < 512; i = i + 32) {
		if (dir[i] == 0x00) {
			dirLoc = i;
			for (j = 0; j < 6; j++) {
				dir[i + j] = filename[j];
			}
		break;
		}
	}

	compSect = 0;

	for (i = 3; compSect < numberOfSectors && i < 512; i++) {
		if (map[i] == 0x00) {
			map[i] = 0xFF;
			dir[dirLoc + compSect + 6] = i;
			writeSector(compSect * 512 + buffer, i);

			compSect = compSect + 1;
		}
	}

	for (i = 6 + compSect; i < 32; i++) {
		dir[dirLoc + i] = 0x00;
	}

	writeSector(map, 1);
	writeSector(dir, 2);
}

int compareCheck(char* buffer, char*fileName) {
	int i, j;
	int checkFound = 0;

	for (i = 0; i < 16; i++) {
		if (buffer[32 * i] != 0x0) {
			printChar(buffer[32 * i]);
			printChar('w');
			printChar('/');
			printChar(fileName[0]);

			for (j = 0; j < 6; j++) {
				if (buffer[j + 32 * i] == 0x0 || fileName[j] == 0x0 || fileName[j] == '\r' || fileName[j] == '\n') {
					break;
				}
				else if (buffer[j + 32 * i] == fileName[j]) {
					checkFound = 1;
				}
				else {
					checkFound = 0;
					break;
				}
			}
		}

		if (checkFound == 1) {
			return i;
		}
		else {
			printChar('&');
		}
	}

	return -1;
}

