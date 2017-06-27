// L0 RM0367 http://www.st.com/content/ccc/resource/technical/document/reference_manual/2f/b9/c6/34/28/29/42/d2/DM00095744.pdf/files/DM00095744.pdf/jcr:content/translations/en.DM00095744.pdf#page=271

#define DMA1_Channel2_IRQn DMA1_Channel2_3_IRQn
#define DMA1_Channel3_IRQn DMA1_Channel2_3_IRQn

#define DMA1_Channel4_IRQn DMA1_Channel4_5_6_7_IRQn
#define DMA1_Channel5_IRQn DMA1_Channel4_5_6_7_IRQn
#define DMA1_Channel6_IRQn DMA1_Channel4_5_6_7_IRQn
#define DMA1_Channel7_IRQn DMA1_Channel4_5_6_7_IRQn


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
    {SPI2, SPI_TX, DMA1_Channel5, 5, 2, DMA1_Channel5_IRQn},
    {SPI2, SPI_RX, DMA1_Channel4, 4, 2, DMA1_Channel4_IRQn},
#endif

};

inline static void setDmaInstance(DMA_HandleTypeDef *handle, dma_request_to_instance_t dmaRequestToStream) {
    handle->Instance = dmaRequestToStream.dmaInstance;
    handle->Init.Request = dmaRequestToStream.requestNumber;
}
