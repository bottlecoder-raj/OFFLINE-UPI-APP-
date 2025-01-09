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

int main()
{
    char acc[10];
    char mob[11];
    char name[20];
    float balance;
    char type;
    int i = 0, j = 0;

    while (1)
    {
        i++;
        printf("\nAttempt #%d", i);
        printf("\nREGISTRATION[r]\t  ACC INFO[e]\tTRANSACTION[t]\tEXIT[x]: ");
        scanf(" %1c", &type); // Space before %c skips any leftover newline characters

        if (type == 'r')
        {
            // Registration Mode
            j++;
            if (j >= NUM)
            {
                printf("User limit reached! Cannot register more users.\n");
                continue;
            }
            printf("\nREGISTRATION MODE\n");
            FILE *file = fopen("data.txt", "a");
            if (file == NULL)
            {
                perror("Error opening file");
                return 1;
            }

            printf("Entering details for User%d:\n", j);
            printf("Enter account number[only 9 characters]: ");
            scanf(" %10s", &acc);
            printf("Enter mobile number: ");
            scanf(" %11s", &mob);
            printf("Enter name: ");
            scanf(" %[^\n]%*c", name);
            for (int k = 0; name[k]; k++)
            {
                name[k] = toupper(name[k]);
            }
            printf("Enter balance: ");
            scanf("%f", &balance);

            printf("\n\nAcc:%s  Reg.mob:+91%s  Name:%s  Bal:%.2f", acc,mob, name, balance);
            int flag = 0;
            flag = fprintf(file, "%s,+91%s,%s,%.2f\n", acc, mob, name, balance);
            if (flag)
            {
                printf("\nContents written successfully\n");
            }
            else
            {
                printf("Error Writing to File!\n");
            }
            fclose(file);
        }

        else if (type == 'e')
        {
            // Extraction Mode
            printf("\nACCOUNT INFO\n");
            // Add extraction logic here

            FILE *file = fopen("data.txt", "r");

            if (file == NULL)
            {
                perror("Error opening file");
                continue;
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
                // Check if the account number exists in the current line
                if (strstr(line, acc))
                {
                    printf("Record found!\n");
                    // Split the line into tokens based on commas
                    token = strtok(line, ",");
                    if (token != NULL)
                    {
                        printf("Account Number: %s\n", token); // First token (account number)
                    }
                    token = strtok(NULL, ","); // Get next token (mobile)
                    if (token != NULL)
                    {
                        printf("Reg.mobile: %s\n", token);
                    }
                    token = strtok(NULL, ","); // Get next token (name)
                    if (token != NULL)
                    {
                        printf("Name: %s\n", token);
                    }

                    token = strtok(NULL, ","); // Get next token (balance)
                    if (token != NULL)
                    {
                        printf("Balance: %s\n", token); // Print the balance
                    }
                    found = 1; // Set the flag to 1 when found
                    break;     // Exit the loop after finding the record
                }
            }

            // If the account was not found, print a message
            if (!found)
            {
                printf("Account not registered!\n");
            }

            fclose(file);
        }
        else if (type == 't')
        {
            // TRANSACTION Program

            const char *filename = "data.txt";
            const char *tempfile = "temp.txt";

            FILE *file = fopen("data.txt", "r");
            FILE *temp = fopen("temp.txt", "w");

            if (file == NULL || temp == NULL)
            {
                perror("Error opening file");
                continue;
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
                // Check if the account number exists in the current line
                if (strstr(line, acc))
                {
                    printf("Record found!\n");
                    // Split the line into tokens based on commas
                    token = strtok(line, ",");
                    token = strtok(NULL, ",");
                    token = strtok(NULL, ","); // Get next token (name)
                    if (token != NULL)
                    {
                        printf("Hello, %s\n", token);
                    }

                    token = strtok(NULL, ","); // Get next token (balance)
                    if (token != NULL)
                    {
                        printf("Your Balance: %s\n", token); // Print the balance
                    }
                    found = 1; // Set the flag to 1 when found
                    break;     // Exit the loop after finding the record
                }
            }
            // Convert string to float using strtof
            float num = strtof(token, NULL);
            // If the account was not found, print a message
            if (!found)
            {
                printf("Account not registered!\n");
                fclose(file);
                fclose(temp);
                remove(tempfile);

                continue;
            }

            char trans;
            float amt;
            printf("Transaction proceeded\n");
            printf("DEPOSIT[d]\tWITHDRAWAL[w]:");
            scanf(" %1c", &trans);
            if (trans == 'd')
            {
                printf("\nDEPOSIT\n");
                printf("Enter amount to deposit:");
                scanf("%f", &amt);
                num = num + amt;
                printf("Balance:%.2f", num);
            }
            else if (trans == 'w')
            {
                printf("\nWITHDRAWAL\n");
                printf("Enter amount to withdraw:");
                scanf("%f", &amt);
                if (amt > num)
                {
                    printf("Insufficient Balance");
                    fclose(file);
                    fclose(temp);
                    remove(tempfile);
                    continue;
                }
                num = num - amt;
                printf("Transaction complete!");
                printf("Balance:%.2f", num);
            }

            char str[20]; // Buffer to hold the string representation

            // Convert float to string
            sprintf(str, "%.2f", num);
            //  printf("\n\nstring:%s", str);
            rewind(file);
            char update[256];
            while (fgets(update, sizeof(update), file))
            {
                // Check if the line contains the targeupdate
                if (strstr(update, acc))
                {
                    // Replace "Engineer" with "Manager"
                    char *pos = strstr(update, token);
                    if (pos)
                    {
                        strncpy(pos, str, 10); // Overwrite with "Manager"
                        pos[strlen(str)] = '\n';
                    }
                }
                fputs(update, temp);
                // Write the line to the temp file
            }

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
        }

        else if (type == 'x')
        {
            // Exit Program
            printf("Exiting the program.\n");
            break; // Exit the loop
        }
        else
        {
            printf("\nWRONG CODE! Please try again.\n");
        }
    }

    return 0;
}
