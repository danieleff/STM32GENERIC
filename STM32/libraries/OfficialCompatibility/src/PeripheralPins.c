#include "PeripheralPins.h"

#ifdef ETHERNET_RMII_MODE_CONFIGURATION

    const PinMap PinMap_Ethernet[]= {
        {ETH_RMII_CRS_DV , GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_AF11_ETH},
        {ETH_RMII_MDC    , GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_AF11_ETH},
        {ETH_RMII_MDIO   , GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_AF11_ETH},
        {ETH_RMII_REF_CLK, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_AF11_ETH},
        {ETH_RMII_RXD0   , GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_AF11_ETH},
        {ETH_RMII_RXD1   , GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_AF11_ETH},
        {ETH_RMII_TXD0   , GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_AF11_ETH},
        {ETH_RMII_TXD1   , GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_AF11_ETH},
        {ETH_RMII_TX_EN  , GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_AF11_ETH},
        {NC, 0, 0, 0}
    };

#endif

const PinMap *pinmap_function(PinName pin, const PinMap* map) {
    while(map->pin != NC) {
        if (map->pin == pin) {
            return map;
        }
        map++;
    }
    return NULL;
}
