// shell.c
// Arianna O'Neil

void promptPrint(char *buffer);
int is___Command(char *command);
void errorPrint(char *buffer);
void typeFile(char *line);
void executeFile(char *line);
void dir();
void del(char *line);
int divide (int i, int j);
int modulo (int i, int j);
void copyFile(char *line);
void createTxt(char *line);

int main() {
	char line[80];
	char promptMessage[8];
	char errorMessage[14];
	int command;

	errorPrint(errorMessage);
	promptPrint(promptMessage);

	while (1) {
		syscall(0, promptMessage);
		syscall(1, line);
		command = is___Command(line);

		if (command == 1) {
			typeFile(line);
		}
		else if (command == 2) {
			executeFile(line);
		}
		else if (command == 3) {
			dir();
		}
		else if (command == 4) {
			del(line);
		}
		else if (command == 5) {
			copyFile(line);
		}
		else if (command == 6) {
			createTxt(line + 7);
		}
		else if (line[0] != 0xa || line[1] != 0x0) {
			syscall(0, errorMessage);
		}
	}
	return 0;
}

// Prompt Message
void promptPrint(char *buffer) {
	buffer[0] = 'S';
	buffer[1] = 'H';
	buffer[2] = 'E';
	buffer[3] = 'L';
	buffer[4] = 'L';
	buffer[5] = '>';
	buffer[6] = ' ';
	buffer[7] = '\0';
}

int is___Command(char *command) {
	if (command[0] == 't' && command[1] == 'y' && command[2] == 'p' && command[3] == 'e' && command[4] == ' ') {
		return 1;
	}
	else if (command[0] == 'e' && command[1] == 'x' && command[2] == 'e' && command[3] == 'c' && command[4] == ' ') {
		return 2;
	}
	else if (command[0] == 'd' && command[1] == 'i' && command[2] == 'r' && command[3] == 0xa) {
		return 3;
	}
	else if (command[0] == 'd' && command[1] == 'e' && command[2] == 'l' && command[3] == ' ') {
		return 4;
	}
	else if (command[0] == 'c' && command[1] == 'o' && command[2] == 'p' && command[3] == 'y' && command[4] == ' ') {
		return 5;
	}
	else if (command[0] == 'c' && command[1] == 'r' && command[2] == 'e' && command[3] == 'a' && command[4] == 't' && command[5] == 'e' && command[6] == ' ') {
		return 6;
	}
	return 0;
}

// Error Message
void errorPrint(char *buffer) {
	buffer[0] = 'B';
	buffer[1] = 'a';
	buffer[2] = 'd';
	buffer[3] = ' ';
	buffer[4] = 'C';
	buffer[5] = 'o';
	buffer[6] = 'm';
	buffer[7] = 'm';
	buffer[8] = 'a';
	buffer[9] = 'n';
	buffer[10] = 'd';
	buffer[11] = '!';
	buffer[12] = '\n';
	buffer[13] = '\0';
}

void typeFile(char *line) {
	char fileName[7];
	char fileBuffer[13312];
	int i;
	int sectorsRead;

	for (i = 0; i < 6 && line[i + 5] != 0xa; i++) {
		fileName[i] = line[i + 5];
	}

	for (; i < 6; i++) {
		fileName[i] = 0x0;
	}

	syscall(3, fileName, fileBuffer, &sectorsRead);
	if (sectorsRead == 0) {
		syscall(0, "File Not Found! Try again.\n\r");
	}
	else {
		syscall(0, fileBuffer);
	}
}

void executeFile(char *line) {
	int i;
	char fileName[7];

	for (i = 0; i < 6; i++) {
		fileName[i] = line[i + 5];
	}
	fileName[6] = '\0';

	syscall(4, fileName);
}

void dir() {
	char sectorDir[512];
	int sora;
	int sectorNumb;
	int riku;
	char file[11];
	int kairi;

	syscall(2, sectorDir, 2);

	for (sora = 0; sora < 512; sora = sora + 32) {
		// REMEMBER: 0x0 means NULL
		if (sectorDir[sora] != '\0') {
			sectorNumb = 0;
			for (riku = sora; riku < sora + 6 && sectorDir[riku] != '\0'; riku++) {
				file[riku - sora] = sectorDir[riku];
			}
			file[riku - sora] = ' ';
			for (kairi = sora + 6; kairi < sora + 32 && sectorDir[kairi] != '\0'; kairi++) {
				sectorNumb++;
			}
			if (sectorNumb > 9) {
				file[1 + riku - sora] = '0' + divide(sectorNumb, 10);
				file[2 + riku - sora] = '0' + modulo(sectorNumb, 10);
				file[3 + riku - sora] = '\n';
//				file[4 + riku - sora] = '\0';
				file[4 + riku - sora] = '\r';
			}
			else if (sectorNumb <= 9) {
				file[1 + riku - sora] = '0' + sectorNumb;
				file[2 + riku - sora] = '\n';
//				file[3 + riku - sora] = '\0';
				file[3 + riku - sora] = '\r';
			}
		syscall(0, file);
		}
	}
}

void del(char *line) {
	char fileName[7];
	int i;

	for (i = 0; i < 7; i++) {
		fileName[i] = 0x0;
	}
	for (i = 0; i < 6 && line[4 + i] != 0xd; i++) {
		fileName[i] = line[4 + i];
	}
	syscall(0, fileName);
	syscall(7, fileName);
}

int divide (int i, int j) {
	int k;
	for (k = 0; j * (1 + k) < (i + 1); k++) {
		return k;
	}
}

int modulo (int i, int j) {
	for ( ; i > (j - 1); i = i - j) {
		return i;
	}
}

void copyFile(char *line) {
	int i;
	char firFile[6];
	char secFile[6];
	int j;
	int k;
	char buf[13312];
	char dir[512];
	int sect;

	// copy over name of file into char array
	for (i = 5; i < 11 && line[i] != ' '; i = i + 1) {
		firFile[i - 5] = line[i];
	}

	for (j = i; j < 6 + 5; j++) {
		firFile[j - 5] = 0x00;
	}

	for (; line[i] != ' '; i++) {

	}

	i++;
	k = i;

	for (; i < 6 + k && line[i] != 0xa; i++) {
		secFile[i - k] = line[i];
	}

	for (; i < 6 + k; i++) {
		secFile[i - k] = 0x0;
	}

	syscall(3, firFile, buf, &sect);
	syscall(2, dir, 2);

	syscall(8, buf, secFile, 3);
}

void createTxt(char *line) {
	int i;
	char file[6];
	int howBig;
	int max = 13312;
	char buf[13312];

	for (i = 0; line[i] != 0xa && i < 6; i++) {
		file[i] = line[i];
	}

	for (; i < 6; i++) {
		file[i] = 0x0;
	}

	howBig = 0;

	while (howBig < 13312) {
		syscall(1, buf + howBig);
		if (buf[howBig] == 0xa) {
			buf[howBig] = '\0';
			syscall(8, buf, file, 3);
			return;
		}

		for (i = 0; buf[howBig + i] != 0xa; i = i + 1) {

		}

		howBig += i + 2;
		buf[howBig - 2] = '\n';
		buf[howBig - 1] = '\r';
	}
}
// big
