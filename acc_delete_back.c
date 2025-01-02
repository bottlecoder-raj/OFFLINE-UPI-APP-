/*
 *DATE:01-01-2025
 *AUTHOR:RAJNARAYAN HAZRA
 *THIS CODE IS MADE FOR MINI PROJECT-OFFLINE UPI PAYMENT APP[TEXTMONEY]
 *THIS IS THE SERVER THAT WILL BE WORKING BACKEND
 *IT CAN ALSO BE USED AS A MANUAL REMOTE SERVER
 *TO SHOW APPRECIATION reply on my mail mentioned in profile
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NUM 10
#define MAX_LINE 256

int main()
{
    int pos = 0;
    const char *filename = "data.txt";
    const char *tempfile = "temp.txt";

    FILE *file = fopen("data.txt", "r");
    FILE *temp = fopen("temp.txt", "w");

    if (file == NULL||temp == NULL)
    {
        perror("Error opening file");
        exit(1);
    }
   char acc[10];
    char line[MAX_LINE];
    int found = 0;

    printf("Enter Account number [UP TO 9 DIGITS]: ");
    scanf("%9s", acc);

    while (fgets(line, sizeof(line), file)) {
        char *line_acc = strtok(line, ",");
        if (line_acc && strcmp(line_acc, acc) == 0) {
            printf("Account deleted: %s\n", acc);
            found = 1;
            continue; // Skip writing this line to the temp file
        }
        fputs(line, temp); // Write the non-matching line
    }

    if (!found) {
        printf("Account not registered!\n");
    } else {
        printf("DATA REMOVED!\n");
    }

    fclose(file);
    fclose(temp);

    if (remove(filename) != 0) {
        perror("Error deleting original file");
        exit(1);
    }
    if (rename(tempfile, filename) != 0) {
        perror("Error renaming temp file");
        exit(1);
    }

    
    return 0;
}
