//code wrtten by CHAT GPT
//Edited by RAJNARAYAN
#include <windows.h>
#include <stdio.h>

int main()
{

        FILE *file;
        char line[256]; // Buffer to store the line

        // Open the file in read mode
        file = fopen("mess.txt", "r");
        if (file == NULL)
        {
            printf("Error opening file.\n");
            return 1;
        }

        // Read a single line from the file
        if (fgets(line, sizeof(line), file) != NULL)
        {
            // Print the line
            printf("Line read: %s\n", line);
        }
        else
        {
            printf("Error or end of file reached.\n");
        }

        // Close the file
        fclose(file);


    // Open the COM port
    HANDLE hSerial = CreateFile("COM7", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);

    if (hSerial == INVALID_HANDLE_VALUE)
    {
        printf("Error opening COM port: %ld\n", GetLastError());
        return 1;
    }

    // Configure the serial port
    DCB dcbSerialParams = {0};
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

    if (!GetCommState(hSerial, &dcbSerialParams))
    {
        printf("Error getting COM state\n");
        CloseHandle(hSerial);
        return 1;
    }

    dcbSerialParams.BaudRate = CBR_9600;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;

    if (!SetCommState(hSerial, &dcbSerialParams))
    {
        printf("Error setting COM state\n");
        CloseHandle(hSerial);
        return 1;
    }

    // Clear buffers
    PurgeComm(hSerial, PURGE_RXCLEAR | PURGE_TXCLEAR);

    char command[256];
    strcpy(command,line);

        // Send command to Arduino
        DWORD bytes_written;
  if (!WriteFile(hSerial, command, strlen(command), &bytes_written, NULL))

    {
        printf("Error writing to COM port\n");
        CloseHandle(hSerial);
        return 1;
    }
    printf("Command sent: %s\n", command);

    // Receive response from Arduino
    char buffer[256];
    DWORD bytes_read;
    if (ReadFile(hSerial, buffer, sizeof(buffer) - 1, &bytes_read, NULL))
    {
        buffer[bytes_read] = '\0'; // Null-terminate the string
        printf("Arduino response: %s\n", buffer);
    }
    else
    {
        printf("Error reading from COM port\n");
    }

    // Close the COM port
    CloseHandle(hSerial);

    return 0;
}
