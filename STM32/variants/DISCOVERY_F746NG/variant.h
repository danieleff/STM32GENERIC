#ifndef VARIANT_H
#define VARIANT_H

#define LED_BUILTIN PI1
#define USER_BTN    PI11

// SPI on SPI2
#define MOSI PB15
#define MISO PB14
#define SCK PI1
#define SS PI0

#define SDA PB9
#define SCL PB8

#define A0 PA0
#define A1 PF10
#define A2 PF9
#define A3 PF8
#define A4 PF7
#define A5 PF6

#define I2C1_100KHZ_TIMING 0x20404768
#define I2C2_100KHZ_TIMING 0x20404768
#define I2C3_100KHZ_TIMING 0x20404768
#define I2C4_100KHZ_TIMING 0x20404768

#define ETHERNET_RMII_MODE_CONFIGURATION

#define ETH_RMII_CRS_DV     PA7
#define ETH_RMII_MDC        PC1
#define ETH_RMII_MDIO       PA2
#define ETH_RMII_REF_CLK    PA1
#define ETH_RMII_RXD0       PC4
#define ETH_RMII_RXD1       PC5
#define ETH_RMII_TXD0       PG13
#define ETH_RMII_TXD1       PG14
#define ETH_RMII_TX_EN      PG11

#define VARIANT_PIN_LIST \
   PIN(C,7 ), /* D0 */ \
   PIN(C,6 ), /* D1 */ \
   PIN(G,6 ), /* D2 */ \
   PIN(B,4 ), /* D3 */ \
   PIN(G,7 ), /* D4 */ \
   PIN(I,0 ), /* D5 */ \
   PIN(H,6 ), /* D6 */ \
   PIN(I,3 ), /* D7 */ \
   PIN(I,2 ), /* D8 */ \
   PIN(A,15), /* D9 */ \
   PIN(A,8 ), /* D10 */ \
   PIN(B,15), /* D11 */ \
   PIN(B,14), /* D12 */ \
   PIN(I,1 ), /* D13 */ \
   PIN(B,9 ), /* D14 */ \
   PIN(B,8 ), /* D15 */ \
   PIN(A,0 ), /* D16 / A0 */ \
   PIN(F,10), /* D17 / A1 */ \
   PIN(F,9 ), /* D18 / A2 */ \
   PIN(F,8 ), /* D19 / A3 */ \
   PIN(F,7 ), /* D20 / A4 */ \
   PIN(F,6 ), /* D21 / A5 */ \
   PIN(A,1 ), \
   PIN(A,2 ), \
   PIN(A,3 ), \
   PIN(A,4 ), \
   PIN(A,5 ), \
   PIN(A,6 ), \
   PIN(A,7 ), \
   PIN(A,9 ), \
   PIN(A,10), \
   PIN(A,11), \
   PIN(A,12), \
   PIN(A,13), \
   PIN(A,14), \
   PIN(B,0 ), \
   PIN(B,1 ), \
   PIN(B,2 ), \
   PIN(B,3 ), \
   PIN(B,5 ), \
   PIN(B,6 ), \
   PIN(B,7 ), \
   PIN(B,10), \
   PIN(B,11), \
   PIN(B,12), \
   PIN(B,13), \
   PIN(C,0 ), \
   PIN(C,1 ), \
   PIN(C,2 ), \
   PIN(C,3 ), \
   PIN(C,4 ), \
   PIN(C,5 ), \
   PIN(C,8 ), \
   PIN(C,9 ), \
   PIN(C,10), \
   PIN(C,11), \
   PIN(C,12), \
   PIN(C,13), \
   PIN(C,14), \
   PIN(C,15), \
   PIN(D,0 ), \
   PIN(D,1 ), \
   PIN(D,2 ), \
   PIN(D,3 ), \
   PIN(D,4 ), \
   PIN(D,5 ), \
   PIN(D,6 ), \
   PIN(D,7 ), \
   PIN(D,8 ), \
   PIN(D,9 ), \
   PIN(D,10), \
   PIN(D,11), \
   PIN(D,12), \
   PIN(D,13), \
   PIN(D,14), \
   PIN(D,15), \
   PIN(E,0 ), \
   PIN(E,1 ), \
   PIN(E,2 ), \
   PIN(E,3 ), \
   PIN(E,4 ), \
   PIN(E,5 ), \
   PIN(E,6 ), \
   PIN(E,7 ), \
   PIN(E,8 ), \
   PIN(E,9 ), \
   PIN(E,10), \
   PIN(E,11), \
   PIN(E,12), \
   PIN(E,13), \
   PIN(E,14), \
   PIN(E,15), \
   PIN(F,0 ), \
   PIN(F,1 ), \
   PIN(F,2 ), \
   PIN(F,3 ), \
   PIN(F,4 ), \
   PIN(F,5 ), \
   PIN(F,11), \
   PIN(F,12), \
   PIN(F,13), \
   PIN(F,14), \
   PIN(F,15), \
   PIN(G,0 ), \
   PIN(G,1 ), \
   PIN(G,2 ), \
   PIN(G,3 ), \
   PIN(G,4 ), \
   PIN(G,5 ), \
   PIN(G,8 ), \
   PIN(G,9 ), \
   PIN(G,10), \
   PIN(G,11), \
   PIN(G,12), \
   PIN(G,13), \
   PIN(G,14), \
   PIN(G,15), \
   PIN(H,0 ), \
   PIN(H,1 ), \
   PIN(H,2 ), \
   PIN(H,3 ), \
   PIN(H,4 ), \
   PIN(H,5 ), \
   PIN(H,7 ), \
   PIN(H,8 ), \
   PIN(H,9 ), \
   PIN(H,10), \
   PIN(H,11), \
   PIN(H,12), \
   PIN(H,13), \
   PIN(H,14), \
   PIN(H,15), \
   PIN(I,4 ), \
   PIN(I,5 ), \
   PIN(I,6 ), \
   PIN(I,7 ), \
   PIN(I,8 ), \
   PIN(I,9 ), \
   PIN(I,10), \
   PIN(I,11), \
   PIN(I,12), \
   PIN(I,13), \
   PIN(I,14), \
   PIN(I,15), \
   PIN(J,0 ), \
   PIN(J,1 ), \
   PIN(J,2 ), \
   PIN(J,3 ), \
   PIN(J,4 ), \
   PIN(J,5 ), \
   PIN(J,6 ), \
   PIN(J,7 ), \
   PIN(J,8 ), \
   PIN(J,9 ), \
   PIN(J,10), \
   PIN(J,11), \
   PIN(J,12), \
   PIN(J,13), \
   PIN(J,14), \
   PIN(J,15), \
   PIN(K,0 ), \
   PIN(K,1 ), \
   PIN(K,2 ), \
   PIN(K,3 ), \
   PIN(K,4 ), \
   PIN(K,5 ), \
   PIN(K,6 ), \
   PIN(K,7 ),


#endif
