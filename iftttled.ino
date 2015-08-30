// IFTTTLed
// A LED for the Internet
//  triggered by the Maker channel on IFTTT, using POST method.
//  the LED output used here is pin 2 (defined in ledPin).
//  the file webtypes.h should be on the same folder. When you load
//   this file, the Arduino IDE should load it too in another tab.
//
// 2015 Érico Vieira Porto
//
// Based on the code WebServer-multi-page (5 Feb 2011)
// http://playground.arduino.cc/Code/WebServer
// by Martyn Woerner, Alessandro Calzavara and Alberto Capponi.
//

#include <SPI.h>
#include <Ethernet.h>
#include "webtypes.h"

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

// ip represents the fixed IP address to use.
byte ip[] = { 192, 168, 0, 100 };

int ledPin = 2;
bool isLedOn = false;

// Http header token delimiters
char *pSpDelimiters = " \r\n";
char *pStxDelimiter = "\002";    // STX - ASCII start of text character

EthernetServer server(80);

// Read the first line of the HTTP request, setting Uri Index and returning the method type.
// If it is a GET method then we set the requestContent to whatever follows the '?'. For a other
// methods there is no content except it may get set later, after the headers for a POST method.
MethodType readRequestLine(EthernetClient & client, BUFFER & readBuffer, BUFFER & requestContent)
{
  MethodType eMethod;

  getNextHttpLine(client, readBuffer);
  // Split it into the 3 tokens
  char * pMethod  = strtok(readBuffer, pSpDelimiters);
  char * pUri     = strtok(NULL, pSpDelimiters);
  char * pVersion = strtok(NULL, pSpDelimiters);
  // URI may optionally comprise the URI of a queryable object a '?' and a query
  // see http://www.ietf.org/rfc/rfc1630.txt
  strtok(pUri, "?");
  char * pQuery   = strtok(NULL, "?");
  if (pQuery != NULL)
  {
    strcpy(requestContent, pQuery);
    // The '+' encodes for a space, so decode it within the string
    for (pQuery = requestContent; (pQuery = strchr(pQuery, '+')) != NULL; )
      *pQuery = ' ';    // Found a '+' so replace with a space

  }
  if (strcmp(pMethod, "POST") == 0)
    eMethod = MethodPost;
//  else  if (strcmp(pMethod, "GET") == 0)
//      eMethod = MethodGet;
  else
    eMethod = MethodUnknown;

  return eMethod;
}

// Read each header of the request till we get the terminating CRLF
void readRequestHeaders(EthernetClient & client, BUFFER & readBuffer, int & nContentLength, bool & bIsUrlEncoded)
{
  nContentLength = 0;      // Default is zero in cate there is no content length.
  bIsUrlEncoded  = true;   // Default encoding
  // Read various headers, each terminated by CRLF.
  // The CRLF gets removed and the buffer holds each header as a string.
  // An empty header of zero length terminates the list.
  do
  {
    getNextHttpLine(client, readBuffer);
    // Process a header. We only need to extract the (optionl) content
    // length for the binary content that follows all these headers.

    char * pFieldName  = strtok(readBuffer, pSpDelimiters);
    char * pFieldValue = strtok(NULL, pSpDelimiters);

    if (strcmp(pFieldName, "Content-Length:") == 0)
    {
      nContentLength = atoi(pFieldValue);
    }
    else if (strcmp(pFieldName, "Content-Type:") == 0)
    {
      if (strcmp(pFieldValue, "application/x-www-form-urlencoded") != 0)
        bIsUrlEncoded = false;
    }
  } while (strlen(readBuffer) > 0);    // empty string terminates
}

// Read the entity body of given length (after all the headers) into the buffer.
void readEntityBody(EthernetClient & client, int nContentLength, BUFFER & content)
{
  int i;
  char c;

  if (nContentLength >= sizeof(content))
    nContentLength = sizeof(content) - 1;  // Should never happen!

  for (i = 0; i < nContentLength; ++i)
  {
    c = client.read();
    content[i] = c;
  }

  content[nContentLength] = 0;  // Null string terminator
}

void getNextHttpLine(EthernetClient & client, BUFFER & readBuffer)
{
  char c;
  int bufindex = 0; // reset buffer

  // reading next header of HTTP request
  if (client.connected() && client.available())
  {
    // read a line terminated by CRLF
    readBuffer[0] = client.read();
    readBuffer[1] = client.read();
    bufindex = 2;
    for (int i = 2; readBuffer[i - 2] != '\r' && readBuffer[i - 1] != '\n'; ++i)
    {
      // read full line and save it in buffer, up to the buffer size
      c = client.read();
      if (bufindex < sizeof(readBuffer))
        readBuffer[bufindex++] = c;
    }
    readBuffer[bufindex - 2] = 0;  // Null string terminator overwrites '\r'
  }
}


// The HTTP request format is defined at http://www.w3.org/Protocols/HTTP/1.0/spec.html#Message-Types
// Read HTTP request, set the requestContent and returning the method type.
MethodType readHttpRequest(EthernetClient & client, BUFFER & requestContent)
{
  BUFFER readBuffer;    // Just a work buffer into which we can read records
  int nContentLength = 0;
  bool bIsUrlEncoded;

  requestContent[0] = 0;    // Initialize as an empty string
  // Read the first line: Request-Line setting Uri Index and returning the method type.
  MethodType eMethod = readRequestLine(client, readBuffer, requestContent);
  // Read any following, non-empty headers setting content length.
  readRequestHeaders(client, readBuffer, nContentLength, bIsUrlEncoded);

  if (nContentLength > 0)
  {
  // If there is some content then read it and do an elementary decode.
    readEntityBody(client, nContentLength, requestContent);
    if (bIsUrlEncoded)
    {
    // The '+' encodes for a space, so decode it within the string
    for (char * pChar = requestContent; (pChar = strchr(pChar, '+')) != NULL; )
      *pChar = ' ';    // Found a '+' so replace with a space
    }
  }

  return eMethod;
}



void setLed(bool isOn)
{
  isLedOn = isOn;
  digitalWrite(ledPin, isLedOn ? HIGH : LOW);
}


void setup()
{
  Serial.begin(9600);
  //Ethernet.begin(mac);  // Use DHCP to get an IP address

  Ethernet.begin(mac, ip);
  server.begin();
  pinMode(ledPin, OUTPUT);

  Serial.print("started!");
  setLed(true);
}

void loop()
{
  EthernetClient client = server.available();

  if (client)
  {
    // now client is connected to arduino we need to extract the
    // following fields from the HTTP request.
    BUFFER requestContent;    // Request content as a null-terminated string.
    MethodType eMethod = readHttpRequest(client, requestContent);

    Serial.print("Request type: ");
    Serial.print(eMethod);
    Serial.print(" content: ");
    Serial.println(requestContent);

    if(eMethod == MethodPost){
      if(strcmp(requestContent, "on") == 0){
        setLed(true);
      } else {
        setLed(false);
      }
    }
   client.println("HTTP/1.0 200 OK");
   client.println("Content-Type: text/html");
   client.println();
   client.println("<HTML><BODY>TEST OK!</BODY></HTML>");
    // give the web browser time to receive the data
    delay(1);
    client.stop();
  }
}
