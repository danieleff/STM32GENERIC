// L4 RM0351 http://www.st.com/content/ccc/resource/technical/document/reference_manual/02/35/09/0c/4f/f7/40/03/DM00083560.pdf/files/DM00083560.pdf/jcr:content/translations/en.DM00083560.pdf#page=342

typedef struct {
    void *periphInstance;
    dmaRequest request;
    DMA_Channel_TypeDef *dmaInstance;
    uint8_t dmaHandlesIndex;
    uint8_t requestNumber;
    uint8_t irqN;
} dma_request_to_instance_t;

const dma_request_to_instance_t dmaRequestToStream[] = {

    {SPI1, SPI_TX, DMA1_Channel3, 3, 1, DMA1_Channel3_IRQn},
    {SPI1, SPI_RX, DMA1_Channel2, 2, 1, DMA1_Channel2_IRQn},

#ifdef SPI2
    {SPI2, SPI_TX, DMA1_Channel5, 5, 1, DMA1_Channel5_IRQn},
    {SPI2, SPI_RX, DMA1_Channel4, 4, 1, DMA1_Channel4_IRQn},
#endif

#ifdef SPI3
    {SPI3, SPI_TX, DMA2_Channel2, 2 + 8, 3, DMA2_Channel2_IRQn},
    {SPI3, SPI_RX, DMA2_Channel1, 1 + 8, 3, DMA2_Channel1_IRQn},
#endif

#if defined(SDMMC1) && defined(SD_InitTypeDef)
    {SDMMC1, SDIO_RXTX, DMA2_Channel4, 6 + 8, 7, DMA2_Channel4_IRQn},
    {SDMMC1, SDIO_RXTX, DMA2_Channel5, 3 + 8, 7, DMA2_Channel5_IRQn},
#endif

};

inline static void setDmaInstance(DMA_HandleTypeDef *handle, dma_request_to_instance_t dmaRequestToStream) {
    handle->Instance = dmaRequestToStream.dmaInstance;
    handle->Init.Request = dmaRequestToStream.requestNumber;
}
