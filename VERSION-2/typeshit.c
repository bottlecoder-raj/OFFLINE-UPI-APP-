#include <windows.h>
#include <stdio.h>

int main()
{
    // Open the COM port
    HANDLE hSerial = CreateFile("COM3", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);

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
    dcbSerialParams.fDtrControl = DTR_CONTROL_ENABLE; // Enable DTR

    if (!SetCommState(hSerial, &dcbSerialParams))
    {
        printf("Error setting COM state\n");
        CloseHandle(hSerial);
        return 1;
    }

    // Set serial timeouts
    COMMTIMEOUTS timeouts = {0};
    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;

    if (!SetCommTimeouts(hSerial, &timeouts))
    {
        printf("Error setting timeouts\n");
        CloseHandle(hSerial);
        return 1;
    }

    // Clear buffers
    PurgeComm(hSerial, PURGE_RXCLEAR | PURGE_TXCLEAR);
    Sleep(4000);

    // Open the file in write mode
    FILE *file = fopen("gsm.txt", "w");
    if (file == NULL)
    {
        printf("Error opening file.\n");
        CloseHandle(hSerial);
        return 1;
    }

    // Receive response from Arduino
    char buffer[256];
    DWORD bytes_read;
    if (ReadFile(hSerial, buffer, sizeof(buffer) - 1, &bytes_read, NULL) && bytes_read > 0)
    {
        if (bytes_read < sizeof(buffer)) // Ensure no overflow
            buffer[bytes_read] = '\0'; // Null-terminate the string

        printf("Arduino response: %s\n", buffer);
        fprintf(file, "%s", buffer);
    }
    else
    {
        printf("Error reading from COM port\n");
    }

    // Close the COM port and file
    CloseHandle(hSerial);
    fclose(file);
    system("money_transfer.exe");

    return 0;
}
