#include <stdio.h>
#include <iostream>
#include <windows.h>
#include <atlstr.h>
#include <string>
//for reading file
#include <fstream>  

#define uchar unsigned char 
using namespace std;

char tx_buf[32];						//buffer for data to send back 
int main(){
	int datalength = 0;					//length of data sent back
	int file_num;						
	uchar Byte_R;						//buffer to store bytes received
	DWORD BytesSent, dwBytesTransferred;
	DCB dcb;							//object of DCB structure
	HANDLE hPort;						// serial port handle
	char modeset[] = { 0xE6, 0xB5, 0xBA, 0xB9, 0xB2, 0xB3, 0xA9, 'd' };		//character set for setting the data mode of the board
	string file_con[4] = { "freshman.txt", "sophomore.txt", "junior.txt", "senior.txt" };		//file list
	char start_sym = '*';				
	char end_sym = '$';					
	char* filename;
	int fileData(char* filename);		//declaration of fileData()
	char* chooseFile(char num);			//declaration of chooseFile()

	hPort = CreateFile(					//create file handle
		(CString)"COM3",				//port name
		GENERIC_WRITE | GENERIC_READ,	//read and write
		0,								
		NULL,
		OPEN_EXISTING,					//open existing file
		NULL,							//synchronized I/O
		0);								//template 0

	//retrieve comm state
	if (!GetCommState(hPort, &dcb))
		printf("retreive setting state error...\n");

	//setting the dcb structure
	dcb.BaudRate = CBR_19200; //19200 Baud
	dcb.ByteSize = 8; //8 data bits
	dcb.Parity = NOPARITY; //no parity
	dcb.StopBits = ONESTOPBIT; //1 stop

	//config comm state
	if (!SetCommState(hPort, &dcb))
		printf("config communication state error...\n");

	for (int i = 0; i < 8; i++){
		WriteFile(hPort, &modeset[i], 1, &BytesSent, NULL);		//setting the board mode
	}

	printf("Library side ready now.......\n");
	while (1){

		Byte_R = 0;
		//start to read from COM for start symbol and file number
		ReadFile(
			hPort,						//handle to the file or device
			&Byte_R,					//buffer to store bytes received
			1,							//miximum number of bytes to read
			&dwBytesTransferred,		//pointer to number of bytes received
			NULL);						//synchronous I/O

		if (Byte_R == '#'){				//command start with '#'
			ReadFile(hPort, &Byte_R, 1, &dwBytesTransferred, NULL);		//read in command(file number)
			printf("start to transfer file %c.....\n", Byte_R);
			filename = chooseFile(Byte_R);							//return file name
			datalength = fileData(filename);						//get data length

			Sleep(1000);//a long delay is necessary to sure to wait for the operator side to be a receiver!!!!!!!!

			WriteFile(
				hPort,					//handle to the file or device
				&start_sym,				//pointer to the bytes to send
				1,						//number of bytes to sent 
				&BytesSent,				//pointer to the number of bytes written
				NULL);					//syncronous I/O

			Sleep(100);

			for (int i = 0; i<datalength - 1; i++){			//here make it length-1 because the file contains an end mark, so the length is 1 byte longer than the content
				WriteFile(hPort, &tx_buf[i], 1, &BytesSent, NULL);
				Sleep(50);
			}

			WriteFile(hPort, &end_sym, 1, &BytesSent, NULL);		//write end symbol
			Sleep(100);
		}
	}
	rewind(stdin);
	getchar();
	return 0;

}
//function for reading file
int fileData(char* filename){
	fstream readByChar;							//file stream object 
	int n = 0;
	char c;
	readByChar.open(filename, ios::in);			//open file with read stream
	if (!readByChar){
		printf("File opening problem!");		
	}
	while (!readByChar.eof())					//judge if read stream reaches the end of the file
	{
		readByChar.read(&c, 1);					//read out one byte
		tx_buf[n] = c;							//save the new d byte into tx-buffer
		n++;									
	}
	readByChar.close();							//close file stream 
	return n;									//return the number of bytes read out
}
//choose file based on number, return file name
char* chooseFile(char num){
	char* filename = nullptr;
	if (num == '0'){
		filename = "freshman.txt";
	}
	if (num == '1'){
		filename = "sophomore.txt";
	}
	if (num == '2'){
		filename = "junior.txt";
	}
	if (num == '3'){
		filename = "senior.txt";
	}
	return filename;
}