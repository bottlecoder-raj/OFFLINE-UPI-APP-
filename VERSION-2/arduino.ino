#include <SoftwareSerial.h>

SoftwareSerial mySerial(9, 10);  // RX, TX

String send_mob, send_acc, send_bal;
String rec_mob, rec_acc, rec_bal;
String balance;

bool smsSent = false;  // Flag to track if SMS has been sent
bool messageStart=false;//Flag to take ony data message

void setup() {
  mySerial.begin(9600);  // GSM Module baud rate
  Serial.begin(9600);    // Serial Monitor baud rate
  delay(100);
  // Serial.println("Arduino started....");
}

void loop() {

  // Incoming sms
  mySerial.println("AT+CMGF=1"); // Set SMS mode to TEXT
  delay(1000);
  mySerial.println("AT+CNMI=2,2,0,0,0"); // Enable live SMS reception
  delay(1000);
  if (mySerial.available()>0) {
    String receivedLine = mySerial.readStringUntil('\n'); // Read line-by-line

    receivedLine.trim(); // Remove leading/trailing spaces or newlines

    if (receivedLine.startsWith("+CMT:")) { 
      messageStart = true; // Next line contains the actual message
    } 
    else if (messageStart) { 
      Serial.print("SMS Content: ");
      Serial.println(receivedLine); // Print only the message content
      messageStart = false; // Reset flag after printing message
    }
  }
  // Check if there's a new command from the Serial Monitor
  else if (Serial.available() > 0) {
    //mySerial.println("AT+CMGF=1");
    // Read the incoming command
    String data = Serial.readStringUntil('\n');
    data.trim();  // Remove any leading/trailing whitespace

    // Print the received command
    Serial.println();
    Serial.print("Command received: ");
    Serial.println(data);

    // Parse the input data
    parseData(data);

    // Reset the flag to allow SMS sending
    smsSent = false;
  }

  // Execute SMS functions only if not already sent
  if (!smsSent) {
    Serial.println("SenderAlert");
    sendSMS(send_mob, send_acc, send_bal, balance, "debited");
    delay(5000);
    Serial.println("ReceiverAlert");
    receiveSMS(rec_mob, rec_acc, rec_bal, balance, "credited");

    // Set the flag to true to prevent further execution
    smsSent = true;
  }


}

void parseData(String input) {
  int index = 0;
  int start = 0;

  while (index < 7) {
    int end = input.indexOf(',', start);
    String value = (end == -1) ? input.substring(start) : input.substring(start, end);
    start = end + 1;

    switch (++index) {
      case 1: send_mob = value; break;
      case 2: send_acc = value; break;
      case 3: send_bal = value; break;
      case 4: rec_mob = value; break;
      case 5: rec_acc = value; break;
      case 6: rec_bal = value; break;
      case 7: balance = value; break;
    }
  }
}

void sendSMS(String mobile, String account, String balance, String amount, String type) {
  delay(1000);
  mySerial.println("AT+CMGS=\"" + mobile + "\"\r");
  delay(1000);

  String message = "Hello User, Rs." + amount + " has been " + type + " from your account " + account + ". Available balance: Rs." + balance;
  mySerial.println(message);
  delay(1000);

  mySerial.println((char)26);  // End of message (CTRL+Z)
  delay(1000);

  Serial.println("Message sent to: " + mobile);
}

void receiveSMS(String mobile, String account, String balance, String amount, String type) {
  delay(1000);
  mySerial.println("AT+CMGS=\"" + mobile + "\"\r");
  delay(1000);

  String message = "Hello User, Rs." + amount + " has been " + type + " to your account " + account + ". Available balance: Rs." + balance;
  mySerial.println(message);
  delay(1000);

  mySerial.println((char)26);  // End of message (CTRL+Z)
  delay(1000);

  Serial.println("Message sent to: " + mobile);
}


