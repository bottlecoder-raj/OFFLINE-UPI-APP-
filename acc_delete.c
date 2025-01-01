/*
 *DATE:01-01-2025
 *AUTHOR:RAJNARAYAN HAZRA
 *THIS CODE IS MADE FOR MINI PROJECT-OFFLINE UPI PAYMENT APP[TEXTMONEY]
 *THIS IS THE ACCOUNT REMOVAL THAT WILL BE WORKING BACKEND
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

    if (file == NULL)
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
            printf("Record found!\n");
            found = 1; // Set the flag to 1 when found
                       // Exit the loop after finding the record
        }
    }

    // If the account was not found, print a message
    if (!found)
    {
        printf("\nAccount not registered!\n");
    }
    if (found)
    {
        rewind(file);
        char update[256];
        while (fgets(update, sizeof(update), file))
        {
            // Check if the line contains the targeupdate
            if (strstr(update, acc))
            {
                fgets(update, sizeof(update), file);
            }
            fputs(update, temp);
            fprintf(temp, ""); // Write the line to the temp file
        }
        printf("\nDATA REMOVED!");
        fclose(file);
        fclose(temp);
        remove(filename);
        rename(tempfile, filename);
    }
    return 0;
}
