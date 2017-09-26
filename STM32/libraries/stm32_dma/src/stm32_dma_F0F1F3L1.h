// F0 RM0091 http://www.st.com/content/ccc/resource/technical/document/reference_manual/c2/f8/8a/f2/18/e6/43/96/DM00031936.pdf/files/DM00031936.pdf/jcr:content/translations/en.DM00031936.pdf#page=194
// F0 RM0360 http://www.st.com/content/ccc/resource/technical/document/reference_manual/cf/10/a8/c4/29/fb/4c/42/DM00091010.pdf/files/DM00091010.pdf/jcr:content/translations/en.DM00091010.pdf#page=159
// F1 RM0008 http://www.st.com/content/ccc/resource/technical/document/reference_manual/59/b9/ba/7f/11/af/43/d5/CD00171190.pdf/files/CD00171190.pdf/jcr:content/translations/en.CD00171190.pdf#page=282
// F1 RM0041 http://www.st.com/content/ccc/resource/technical/document/reference_manual/a2/2d/02/4b/78/57/41/a3/CD00246267.pdf/files/CD00246267.pdf/jcr:content/translations/en.CD00246267.pdf#page=150
// F3 RM0316 http://www.st.com/content/ccc/resource/technical/document/reference_manual/4a/19/6e/18/9d/92/43/32/DM00043574.pdf/files/DM00043574.pdf/jcr:content/translations/en.DM00043574.pdf#page=272
// L1 RM0038 http://www.st.com/content/ccc/resource/technical/document/reference_manual/cc/f9/93/b2/f0/82/42/57/CD00240193.pdf/files/CD00240193.pdf/jcr:content/translations/en.CD00240193.pdf#page=255

#ifndef DMA1_Channel2_IRQn
#define DMA1_Channel2_IRQn DMA1_Channel2_3_IRQn
#define DMA1_Channel3_IRQn DMA1_Channel2_3_IRQn
#endif

#ifndef DMA1_Channel4_IRQn
#define DMA1_Channel4_IRQn DMA1_Channel4_5_6_7_IRQn
#define DMA1_Channel5_IRQn DMA1_Channel4_5_6_7_IRQn
#define DMA1_Channel6_IRQn DMA1_Channel4_5_6_7_IRQn
#define DMA1_Channel7_IRQn DMA1_Channel4_5_6_7_IRQn
#endif


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
