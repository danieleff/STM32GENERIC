//Streaming func testing

void setup()
{
  Serial.begin(115200);
  int lettera = 'A';
  int month = 5, day = 19, year = 2016;
  
  Serial << "This is an example of the new streaming" << _endl;
  Serial << "library.  This allows you to print variables" << _endl;
  Serial << "and strings without having to type line after" << _endl;
  Serial << "line of Serial.print() calls.  Examples: " << _endl;
  
  Serial << "A is " << lettera << "." << _endl;
  Serial << "The current date is " << day << "-" << month << "-" << year << "." << _endl;
  
  Serial << "You can use modifiers too, for example:" << _endl;
  Serial << _BYTE(lettera) << " is " << _HEX(lettera) << " in hex. " << _endl;
}

void loop()
{}
