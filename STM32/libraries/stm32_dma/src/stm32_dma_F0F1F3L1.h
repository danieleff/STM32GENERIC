
typedef struct {
    void *periphInstance;
    dmaRequest request;
    DMA_Channel_TypeDef *dmaInstance;
    uint8_t dmaHandlesIndex;
    uint8_t irqN;
} dma_request_to_instance_t;

const dma_request_to_instance_t dmaRequestToStream[] = {

// TODO SPI, etc...

#ifdef SDIO
    {SDIO, SDIO_RXTX, DMA2_Channel4, 4 + 8, DMA2_Channel4_IRQn},
#endif

};

inline static setDmaInstance(DMA_HandleTypeDef *handle, dma_request_to_instance_t dmaRequestToStream) {
    handle->Instance = dmaRequestToStream.dmaInstance;
}
