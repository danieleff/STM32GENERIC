/* Hello World! arduino Stream out demo by huawei<huaweiwx@sina.com> 2016.10.18 */

void setup() {
  Serial.begin(115200);
}

uint16_t count;
void loop() {
    Serial<<"Hello World: "<<count++ <<"sec"<<_endl;
	delay(1000);
}
