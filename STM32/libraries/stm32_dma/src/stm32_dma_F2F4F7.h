
typedef struct {
    void *periphInstance;
    dmaRequest request;
    DMA_Stream_TypeDef *dmaInstance;
    uint32_t channel;
    uint8_t dmaHandlesIndex;
    uint8_t irqN;
} dma_request_to_instance_t;

const dma_request_to_instance_t dmaRequestToStream[] = {
    {SPI1, SPI_TX, DMA2_Stream3, DMA_CHANNEL_3, 3 + 8, DMA2_Stream3_IRQn},
    {SPI1, SPI_RX, DMA2_Stream0, DMA_CHANNEL_3, 0 + 8, DMA2_Stream0_IRQn},

    {SPI2, SPI_TX, DMA1_Stream4, DMA_CHANNEL_0, 4, DMA1_Stream4_IRQn},
    {SPI2, SPI_RX, DMA1_Stream3, DMA_CHANNEL_0, 3, DMA1_Stream3_IRQn},

#ifdef SPI3
    {SPI3, SPI_TX, DMA1_Stream5, DMA_CHANNEL_0, 5, DMA1_Stream5_IRQn},
    {SPI3, SPI_RX, DMA1_Stream0, DMA_CHANNEL_0, 0, DMA1_Stream0_IRQn},
#endif

#if defined(SDIO) && defined(SD_InitTypeDef)
    {SDIO, SDIO_RXTX, DMA2_Stream6, DMA_CHANNEL_4, 6 + 8, DMA2_Stream6_IRQn},
    {SDIO, SDIO_RXTX, DMA2_Stream3, DMA_CHANNEL_4, 3 + 8, DMA2_Stream3_IRQn},
#endif

#if defined(SDMMC1) && defined(SD_InitTypeDef)
    {SDMMC1, SDIO_RXTX, DMA2_Stream6, DMA_CHANNEL_4, 6 + 8, DMA2_Stream6_IRQn},
    {SDMMC1, SDIO_RXTX, DMA2_Stream3, DMA_CHANNEL_4, 3 + 8, DMA2_Stream3_IRQn},
#endif

};

inline static void setDmaInstance(DMA_HandleTypeDef *handle, dma_request_to_instance_t dmaRequestToStream) {
    handle->Instance = dmaRequestToStream.dmaInstance;
    handle->Init.Channel = dmaRequestToStream.channel;
}
