// Example for the onboard ethernet connector
//
// Install https://github.com/stm32duino/LwIP
//     and https://github.com/stm32duino/STM32Ethernet
//
// If they are empty, install the work in progress versions using git:
//    cd <your arduino library folder>
//    git clone https://github.com/fprwi6labs/LwIP.git
//    cd LwIP
//    git checkout LwIP_src
//
//    cd ..
//    git clone https://github.com/fprwi6labs/STM32Ethernet.git
//    cd STM32Ethernet
//    git checkout add_src
//
//
// For more examples, check the STM32Ethernet examples folder

#include <LwIP.h>
#include <STM32Ethernet.h>

EthernetClient client;

// random MAC address
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

void setup() {
    Serial.begin(115200);

    Serial.println("Connecting to Ethernet");

    if (!Ethernet.begin(mac)) {
        Serial.println("ERROR: Could not connect to ethernet!");

        while(1);
    }

    Serial.println("Connecting to example.com");

    if (!client.connect("example.com", 80)) {
        Serial.println("ERROR: Could not connect to example.com!");

        while(1);
    }

    Serial.println("Connected, sending request");

    client.println("GET / HTTP/1.1");
    client.println("Host: example.com");
    client.println();
    client.flush();

    Serial.println("Request sent, printing response");
}

void loop() {
    if (client.available()) {
        char c = client.read();
        Serial.print(c);
    }

    if (!client.connected()) {
        Serial.println();
        Serial.println("disconnecting.");
        client.stop();

        while (1);
    }
}
