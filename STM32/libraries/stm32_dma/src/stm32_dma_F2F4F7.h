// F2 RM0033 http://www.st.com/content/ccc/resource/technical/document/reference_manual/51/f7/f3/06/cd/b6/46/ec/CD00225773.pdf/files/CD00225773.pdf/jcr:content/translations/en.CD00225773.pdf#page=179
// F4 RM0090 http://www.st.com/content/ccc/resource/technical/document/reference_manual/3d/6d/5a/66/b4/99/40/d4/DM00031020.pdf/files/DM00031020.pdf/jcr:content/translations/en.DM00031020.pdf#page=307
// F4 RM0368 http://www.st.com/content/ccc/resource/technical/document/reference_manual/5d/b1/ef/b2/a1/66/40/80/DM00096844.pdf/files/DM00096844.pdf/jcr:content/translations/en.DM00096844.pdf#page=165
// F4 RM0401 http://www.st.com/content/ccc/resource/technical/document/reference_manual/20/f5/7a/01/0a/e1/47/ef/DM00180366.pdf/files/DM00180366.pdf/jcr:content/translations/en.DM00180366.pdfF#page=165
// F4 RM0383 http://www.st.com/content/ccc/resource/technical/document/reference_manual/9b/53/39/1c/f7/01/4a/79/DM00119316.pdf/files/DM00119316.pdf/jcr:content/translations/en.DM00119316.pdf#page=166
// F4 RM0402 http://www.st.com/content/ccc/resource/technical/document/reference_manual/group0/4f/7b/2b/bd/04/b3/49/25/DM00180369/files/DM00180369.pdf/jcr:content/translations/en.DM00180369.pdf#page=196
// F4 RM0390 http://www.st.com/content/ccc/resource/technical/document/reference_manual/4d/ed/bc/89/b5/70/40/dc/DM00135183.pdf/files/DM00135183.pdf/jcr:content/translations/en.DM00135183.pdf#page=199
// F4 RM0386 http://www.st.com/content/ccc/resource/technical/document/reference_manual/29/77/09/5a/b1/60/4e/bd/DM00127514.pdf/files/DM00127514.pdf/jcr:content/translations/en.DM00127514.pdf#page=223
// F7 RM0385 http://www.st.com/content/ccc/resource/technical/document/reference_manual/c5/cf/ef/52/c0/f1/4b/fa/DM00124865.pdf/files/DM00124865.pdf/jcr:content/translations/en.DM00124865.pdf#page=222
// F7 RM0410 http://www.st.com/content/ccc/resource/technical/document/reference_manual/group0/96/8b/0d/ec/16/22/43/71/DM00224583/files/DM00224583.pdf/jcr:content/translations/en.DM00224583.pdf#page=246


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
