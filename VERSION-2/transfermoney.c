#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    printf("\n\nNOTE! This should be done only by user.\nReceivers must request sender to send money\n\n");
    char sender[20];
    char receiver[20];
    float balance;
    float se_m;
    float re_m;
    char send_mobile[13];
    char rece_mobile[13];
    char line[256];
    char sms[256];
    int found = 0;
    int round = 0;
    char *s_token;
    char *r_token;
    char *sms_token;

    const char *filename = "data.txt";
    const char *smsfile = "gsm.txt";
    const char *tempfile = "temp.txt";
    const char *messfile = "mess.txt";

    FILE *gsm  = fopen("gsm.txt","r");
    FILE *file = fopen("data.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    FILE *mess = fopen("mess.txt", "w");
   
    if (file == NULL || temp == NULL || mess == NULL || gsm == NULL)
    {
        perror("Error opening file");
        exit(1);
    }

    fgets(sms,sizeof(sms),gsm);
    sms_token = strtok(sms,",");
    if(sms_token != NULL)
    {
        printf("Sender:%s\n",sms_token);
        strcpy(sender,sms_token);
    }
     sms_token = strtok(NULL,",");
    if(sms_token != NULL)
    {
        printf("Receiver:%s\n",sms_token);
        strcpy(receiver,sms_token);
    }
     sms_token = strtok(NULL,",");
    if(sms_token != NULL)
    {
        printf("Amount Exchange:%s\n",sms_token);
    }
    balance=strtof(sms_token,NULL);


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
            if (s_token != NULL)
            {
                strcpy(send_mobile, s_token); // First token (mobile number)
            }
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
    fprintf(mess,"%s,",send_mobile);
    se_m = strtof(s_token, NULL);

    // If the account was not found, print a message
    if (!found)
    {
        printf(" Sender Account not registered!\n");
        fclose(file);
        fclose(temp);
        fclose(mess);
        fclose(gsm);
        remove(messfile);
        remove(tempfile);
        exit(1);
    }
    
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
                strcpy(receiver,r_token);
            }
            r_token = strtok(NULL, ","); // Get next token (mobile)
            if (r_token != NULL)
            {
                strcpy(rece_mobile, r_token); // First token (mobile number)
            }
            r_token = strtok(NULL, ","); // Get next token (name)
            if (r_token != NULL)
            {
                printf("Receiver's Name: %s\n\n", r_token);
            }

            r_token = strtok(NULL, ","); // Get next token (balance)
            round = 1;                   // Set the flag to 1 when found
            break;                       // Exit the loop after finding the record
        }
    }
    re_m = strtof(r_token, NULL);

    // If the account was not found, print a message
    if (!round)
    {
        printf(" Receiver Account not registered!\n");
        fclose(file);
        fclose(temp);
        fclose(mess);
        fclose(gsm);
        remove(messfile);
        remove(tempfile);
        exit(1);
    }

    // printf("Enter amount to transfer:");
    // scanf("%f", &balance);
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
    // output messages
    // store sender sms
    printf("\nHello User, Rs.%.2f has been debited from your \naccount number: %s. Cr.Bal: %.2f\n", balance, sender, se_m);
    // store receiver sms
    printf("\nHello User, Rs.%.2f has been credited to your \naccount number: %s. Cr.Bal: %.2f\n", balance, receiver, re_m);

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
    printf("\nTransaction completed successfully!\n");
   

    // file for communication
      // Write transaction details to mess file
    int flag = fprintf(mess, "%s,%.2f,%s,%s,%.2f,%.2f\n", sender, se_m, rece_mobile, receiver, re_m, balance);
    if (flag > 0)
    {
        printf("\nContents written successfully\n");
    }
    else
    {
        printf("Error Writing to File!\n");
    }
    fclose(mess);
    fclose(gsm);
    system("Serial.exe");
 /*   int zero;
    printf("0:");
    scanf("%d",&zero);
*/
 remove(messfile);
 remove(smsfile);
    return 0;
}
