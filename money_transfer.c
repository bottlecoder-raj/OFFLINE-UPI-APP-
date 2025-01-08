#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    printf("\n\nNOTE! This should be done only by user.\nReceivers must request sender to send money\n\n");
    char sender[10];
    char receiver[10];
    float balance;
    float se_m;
    float re_m;
    char line[256];
    int found = 0;
    char *s_token;
    char *r_token;

    const char *filename = "data.txt";
    const char *tempfile = "temp.txt";

    FILE *file = fopen("data.txt", "r");
    FILE *temp = fopen("temp.txt", "w");

    if (file == NULL || temp == NULL)
    {
        perror("Error opening file");
        exit(1);
    }

    printf("Enter Sender Acc number:");
    scanf("%9s", sender);

    // Checking senders acc number and balance
    while (fgets(line, sizeof(line), file))
    {
        // Check if the account number exists in the current line
        if (strstr(line, sender))
        {
            // Split the line into tokens based on commas
            s_token = strtok(line, ",");
            if (s_token != NULL)
            {
                printf("Account Number: %s\n", s_token); // First token (account number)
            }
            s_token = strtok(NULL, ","); // Get next token (mobile)
            s_token = strtok(NULL, ","); // Get next token (name)
            if (s_token != NULL)
            {
                printf("Sender 's Name: %s\t", s_token);
            }

            s_token = strtok(NULL, ","); // Get next token (balance)
            if (s_token != NULL)
            {
                printf("Balance: %s\n", s_token); // Print the balance
            }
            found = 1; // Set the flag to 1 when found
            break;     // Exit the loop after finding the record
        }
    }
    se_m = strtof(s_token, NULL);

    // If the account was not found, print a message
    if (!found)
    {
        printf(" Sender Account not registered!\n");
        fclose(file);
        fclose(temp);
        exit(1);
    }
    found = 0;
    printf("Enter Receiver Acc number:");
    scanf("%9s", receiver);
    // checking only receivers acc number
    rewind(file);
    while (fgets(line, sizeof(line), file))
    {
        // Check if the account number exists in the current line
        if (strstr(line, receiver))
        {
            // Split the line into tokens based on commas
            r_token = strtok(line, ",");
            if (r_token != NULL)
            {
                printf("Account Number: %s\t", r_token); // First token (account number)
            }
            r_token = strtok(NULL, ","); // Get next token (mobile)
            r_token = strtok(NULL, ","); // Get next token (name)
            if (r_token != NULL)
            {
                printf("Receiver's Name: %s\n\n", r_token);
            }

            r_token = strtok(NULL, ","); // Get next token (balance)
            found = 1;                   // Set the flag to 1 when found
            break;                       // Exit the loop after finding the record
        }
    }
    re_m = strtof(r_token, NULL);

    // If the account was not found, print a message
    if (!found)
    {
        printf(" Receiver Account not registered!\n");
        fclose(file);
        fclose(temp);
        exit(1);
    }

    printf("Enter amount to transfer:");
    scanf("%f", &balance);
    if (balance > se_m)
    {
        printf("insufficient balance");
        fclose(file);
        fclose(temp);
        exit(1);
    }
    se_m = se_m - balance;
    re_m = re_m + balance;
    char send_m[20];
    char rec_m[20];
    // Convert float to string
    sprintf(send_m, "%.2f", se_m);
    // Convert float to string
    sprintf(rec_m, "%.2f", re_m);
    // Update the file with new balances
    rewind(file);
    while (fgets(line, sizeof(line), file))
    {
        if (strstr(line, sender))
        {
            // Update sender's balance
            char *pos = strrchr(line, ',');
            if (pos)
            {
                *pos = '\0'; // Remove the old balance
                fprintf(temp, "%s,%s\n", line, send_m);
                continue;
            }
        }
        else if (strstr(line, receiver))
        {
            // Update receiver's balance
            char *pos = strrchr(line, ',');
            if (pos)
            {
                *pos = '\0'; // Remove the old balance
                fprintf(temp, "%s,%s\n", line, rec_m);
                continue;
            }
        }
        // Write other lines unchanged
        fputs(line, temp);
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
    printf("Transaction completed successfully!\n");
    return 0;
}