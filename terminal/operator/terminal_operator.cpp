#include <stdio.h>
#include <iostream>
#include <windows.h>
#include <atlstr.h>
#include <string>
#define uchar unsigned char 
using namespace std;
int main(){
	int inputFlag=0,flowflag=0;			//flags
	uchar file_num;
	uchar Byte_R=0;
	uchar start_sym = '#';
	DWORD BytesSent, dwBytesTransferred;
	DCB dcb;
	HANDLE hPort;
	char modeset[] = { 0xE6, 0xB5, 0xBA, 0xB9, 0xB2, 0xB3, 0xA9, 'd' };				//character set for changing the board mode

	//create a handle for the COM port
	hPort = CreateFile(						
		(CString)"COM3",					//file name
		GENERIC_WRITE | GENERIC_READ,		//create for both reading and writing
		0,									//open with exclusive access
		NULL,								//no security attrbutes
		OPEN_EXISTING,						//open existing file
		NULL,								//synchronized I/O
		0);									//template 0 for comm devices

	//retrieve comm state
	if (!GetCommState(hPort, &dcb))
		printf("retreive setting state error...");

	//setting the dcb structure
	dcb.BaudRate = CBR_19200; //19200 Baud
	dcb.ByteSize = 8; //8 data bits
	dcb.Parity = NOPARITY; //no parity
	dcb.StopBits = ONESTOPBIT; //1 stop

	//config comm state
	if (!SetCommState(hPort, &dcb))
		printf("config communication state error...");

	for (int i = 0; i < 8; i++){
		WriteFile(hPort, &modeset[i], 1, &BytesSent, NULL);				//change board mode
	}

	printf("Now start......\n");
	while (1){
		flowflag = 0;
		//start get command from user
		Sleep(500);
		printf("\n");
		do{
			inputFlag = 0;											//print out the file list provided
			printf("The files available are:\n");
			printf("0. freshman.txt\n");
			printf("1. sophomore.txt\n");
			printf("2. junior.txt\n");
			printf("3. senior.txt\n");
			printf("Please input the file number:");
			cin >> file_num;
			if ((file_num != '0') && (file_num != '1') && (file_num != '2') && (file_num != '3')){			//deal with input exception
				printf("The input file number is not correct! Please input again! \n");
				printf("\n");
				inputFlag = 1;

			}
		} while (inputFlag==1);



		//first send the start symbol
		WriteFile(hPort, &start_sym, 1, &BytesSent, NULL);
		//send the file num asset by the user
		WriteFile(hPort, &file_num, 1, &BytesSent, NULL);
		printf("Please wait......\n");

		while(flowflag==0){
			ReadFile(hPort, &Byte_R, 1, &dwBytesTransferred, NULL);	
			if (Byte_R == '*'){											//judge if the data read in is the start symbol of the file content
				ReadFile(hPort, &Byte_R, 1, &dwBytesTransferred, NULL);			//start read the content of the file
				printf("The content is....\n");									
				while (Byte_R != '$'){									//end mark of the file content
					printf("%c", Byte_R);
					ReadFile(hPort, &Byte_R, 1, &dwBytesTransferred, NULL);	
				}
				printf("\n");
				flowflag = 1;								//file content reaches end, go out of the loop
			}
			else{
				flowflag = 0;
			}
		}
		
	}
		getchar();
		return 0;

}