
typedef struct {
    void *periphInstance;
    dmaRequest request;
    DMA_Channel_TypeDef *dmaInstance;
    uint8_t dmaHandlesIndex;
    uint8_t irqN;
} dma_request_to_instance_t;

const dma_request_to_instance_t dmaRequestToStream[] = {

    {SPI1, SPI_TX, DMA1_Channel3, 3, DMA1_Channel3_IRQn},
    {SPI1, SPI_RX, DMA1_Channel2, 2, DMA1_Channel2_IRQn},

#ifdef SPI2
    {SPI2, SPI_TX, DMA1_Channel5, 5, DMA1_Channel5_IRQn},
    {SPI2, SPI_RX, DMA1_Channel4, 4, DMA1_Channel4_IRQn},
#endif

#ifdef SPI3
    {SPI3, SPI_TX, DMA2_Channel2, 2 + 8, DMA1_Channel2_IRQn},
    {SPI3, SPI_RX, DMA2_Channel1, 1 + 8, DMA1_Channel1_IRQn},
#endif

#if defined(SDIO) && defined(SD_InitTypeDef)
    {SDIO, SDIO_RXTX, DMA2_Channel4, 4 + 8, DMA2_Channel4_IRQn},
#endif

};

inline static void setDmaInstance(DMA_HandleTypeDef *handle, dma_request_to_instance_t dmaRequestToStream) {
    handle->Instance = dmaRequestToStream.dmaInstance;
}
