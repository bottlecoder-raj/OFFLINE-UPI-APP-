/*
 *DATE:01-01-2025
 *AUTHOR:RAJNARAYAN HAZRA
 *THIS CODE IS MADE FOR MINI PROJECT-OFFLINE UPI PAYMENT APP[TEXTMONEY]
 *THIS IS THE SERVER THAT WILL BE WORKING BACKEND
 *ACOOUNT DELETION PROCESS
 *IT CAN ALSO BE USED AS A MANUAL REMOTE SERVER
 *TO SHOW APPRECIATION reply on my mail mentioned in profile
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NUM 10

int main()
{
    int pos = 0;
    const char *filename = "data.txt";
    const char *tempfile = "temp.txt";

    FILE *file = fopen("data.txt", "r");
    FILE *temp = fopen("temp.txt", "w");

    if (file == NULL || temp == NULL)
    {
        perror("Error opening file");
        exit(1);
    }
    char acc[10];
    char line[256];
    int found = 0;
    char *token;
    rewind(file);
    printf("Enter Account number[UPTO 9 DIGITS]:");
    scanf("%9s", acc);
    while (fgets(line, sizeof(line), file))
    {
        pos++;
        // Check if the account number exists in the current line
        if (strstr(line, acc))
        {

            printf("Account deleted!");
            found = 1; // Set the flag to 1 when found
            continue;
            // fprintf(temp, ""); // Write the line to the temp file
        }
        fputs(line, temp);
    }
    if (!found)
    {
        printf("\nAccount not registered!\n");
        exit(1);
    }

    printf("\nDATA REMOVED!");
    fclose(file);
    fclose(temp);
    if (remove(filename) != 0)
    {
        perror("Error deleting original file");
        exit(1);
    }
    if (rename(tempfile, filename) != 0)
    {
        perror("Error renaming temp file");
        exit(1);
    }

    return 0;
}
