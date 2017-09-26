/*
  Copyright (c) 2017 Daniel Fekete

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/

#include "stm32_def.h"
#include "stm32_dma.h"

#if defined(STM32F2) || defined(STM32F4) || defined(STM32F7)
#include "stm32_dma_F2F4F7.h"
#elif defined(STM32F0) || defined(STM32F1) || defined(STM32F3) || defined(STM32L1)
#include "stm32_dma_F0F1F3L1.h"
#elif defined(STM32L0)
#include "stm32_dma_L0.h"
#elif defined(STM32L4)
#include "stm32_dma_L4.h"
#else
#error "Unknown chip"
#endif

//8 handles for DMA1, 8 handles for DMA2
DMA_HandleTypeDef *dmaHandles[16];
bool reserved[16];

bool stm32DmaAcquire(DMA_HandleTypeDef *handle, dmaRequest request, void *periphInstance,  bool enableIrq) {
    if (handle->Instance != NULL) {
        return true;
    }

    for(size_t i=0; i<sizeof(dmaRequestToStream) / sizeof(dmaRequestToStream[0]); i++) {
        int dmaHandlesIndex = dmaRequestToStream[i].dmaHandlesIndex;
        if (!reserved[dmaHandlesIndex]
              && dmaRequestToStream[i].request == request
              && dmaRequestToStream[i].periphInstance == periphInstance
            ) {

            setDmaInstance(handle, dmaRequestToStream[i]);

            if (enableIrq) {
                HAL_NVIC_SetPriority(dmaRequestToStream[i].irqN, 0, 0);
                HAL_NVIC_EnableIRQ(dmaRequestToStream[i].irqN);
            }

            if (dmaHandles[dmaHandlesIndex]) {
                dmaHandles[dmaHandlesIndex]->Instance = NULL;
            }

            dmaHandles[dmaHandlesIndex] = handle;
            reserved[dmaHandlesIndex] = true;

            return true;
        }
    }
    return false;
}

void stm32DmaRelease(DMA_HandleTypeDef *handle) {
    if (handle->Instance == NULL) {
        return;
    }

    for(int i=0; i<16; i++) {
        if (dmaHandles[i] == handle) {
            reserved[i] = false;
            break;
        }
    }
}


// F2, F4, F7
extern void DMA1_Stream0_IRQHandler() {
    HAL_DMA_IRQHandler(dmaHandles[0]);
}

// F2, F4, F7
extern void DMA1_Stream1_IRQHandler() {
    HAL_DMA_IRQHandler(dmaHandles[1]);
}

// F2, F4, F7
extern void DMA1_Stream2_IRQHandler() {
    HAL_DMA_IRQHandler(dmaHandles[2]);
}

// F2, F4, F7
extern void DMA1_Stream3_IRQHandler() {
    HAL_DMA_IRQHandler(dmaHandles[3]);
}

// F2, F4, F7
extern void DMA1_Stream4_IRQHandler() {
    HAL_DMA_IRQHandler(dmaHandles[4]);
}

// F2, F4, F7
extern void DMA1_Stream5_IRQHandler() {
    HAL_DMA_IRQHandler(dmaHandles[5]);
}

// F2, F4, F7
extern void DMA1_Stream6_IRQHandler() {
    HAL_DMA_IRQHandler(dmaHandles[6]);
}

// F2, F4, F7
extern void DMA1_Stream7_IRQHandler() {
    HAL_DMA_IRQHandler(dmaHandles[7]);
}

// F2, F4, F7
extern void DMA2_Stream0_IRQHandler() {
    HAL_DMA_IRQHandler(dmaHandles[0 + 8]);
}

// F2, F4, F7
extern void DMA2_Stream1_IRQHandler() {
    HAL_DMA_IRQHandler(dmaHandles[1 + 8]);
}

// F2, F4, F7
extern void DMA2_Stream2_IRQHandler() {
    HAL_DMA_IRQHandler(dmaHandles[2 + 8]);
}

// F2, F4, F7
extern void DMA2_Stream3_IRQHandler() {
    HAL_DMA_IRQHandler(dmaHandles[3 + 8]);
}

// F2, F4, F7
extern void DMA2_Stream4_IRQHandler() {
    HAL_DMA_IRQHandler(dmaHandles[4 + 8]);
}

// F2, F4, F7
extern void DMA2_Stream5_IRQHandler() {
    HAL_DMA_IRQHandler(dmaHandles[5 + 8]);
}

// F2, F4, F7
extern void DMA2_Stream6_IRQHandler() {
    HAL_DMA_IRQHandler(dmaHandles[6 + 8]);
}

// F2, F4, F7
extern void DMA2_Stream7_IRQHandler() {
    HAL_DMA_IRQHandler(dmaHandles[7 + 8]);
}



// F0
#ifndef DMA1_Ch1_IRQHandler
extern void DMA1_Ch1_IRQHandler() {
    HAL_DMA_IRQHandler(dmaHandles[1]);
}
#endif

// F0
#ifndef DMA1_Ch2_3_DMA2_Ch1_2_IRQHandler
extern void DMA1_Ch2_3_DMA2_Ch1_2_IRQHandler() {
    HAL_DMA_IRQHandler(dmaHandles[2]);
    HAL_DMA_IRQHandler(dmaHandles[3]);
    HAL_DMA_IRQHandler(dmaHandles[1 + 8]);
    HAL_DMA_IRQHandler(dmaHandles[2 + 8]);
}
#endif

// F0
#ifndef DMA1_Ch4_7_DMA2_Ch3_5_IRQHandler
extern void DMA1_Ch4_7_DMA2_Ch3_5_IRQHandler() {
    HAL_DMA_IRQHandler(dmaHandles[4]);
    HAL_DMA_IRQHandler(dmaHandles[5]);
    HAL_DMA_IRQHandler(dmaHandles[6]);
    HAL_DMA_IRQHandler(dmaHandles[7]);

    HAL_DMA_IRQHandler(dmaHandles[3 + 8]);
    HAL_DMA_IRQHandler(dmaHandles[4 + 8]);
    HAL_DMA_IRQHandler(dmaHandles[5 + 8]);
}
#endif

// F0, F1, F3, L0, L1, L4
extern void DMA1_Channel1_IRQHandler() {
    HAL_DMA_IRQHandler(dmaHandles[1]);
}

// F0, L0
extern void DMA1_Channel2_3_IRQHandler() {
    HAL_DMA_IRQHandler(dmaHandles[2]);
    HAL_DMA_IRQHandler(dmaHandles[3]);
}

// F1, F3, L1, L4
extern void DMA1_Channel2_IRQHandler() {
    HAL_DMA_IRQHandler(dmaHandles[2]);
}

// F1, F3, L1, L4
extern void DMA1_Channel3_IRQHandler() {
    HAL_DMA_IRQHandler(dmaHandles[3]);
}

// F0, L0
#ifndef DMA1_Channel4_5_6_7_IRQHandler
extern void DMA1_Channel4_5_6_7_IRQHandler() {
    HAL_DMA_IRQHandler(dmaHandles[4]);
    HAL_DMA_IRQHandler(dmaHandles[5]);
    HAL_DMA_IRQHandler(dmaHandles[6]);
    HAL_DMA_IRQHandler(dmaHandles[7]);
}
#endif

// F0
extern void DMA1_Channel4_5_IRQHandler() {
    HAL_DMA_IRQHandler(dmaHandles[4]);
    HAL_DMA_IRQHandler(dmaHandles[5]);
}

// F1, F3, L1, L4
extern void DMA1_Channel4_IRQHandler() {
    HAL_DMA_IRQHandler(dmaHandles[4]);
}

// F1, F3, L1, L4
extern void DMA1_Channel5_IRQHandler() {
    HAL_DMA_IRQHandler(dmaHandles[5]);
}

// F1, F3, L1, L4
extern void DMA1_Channel6_IRQHandler() {
    HAL_DMA_IRQHandler(dmaHandles[6]);
}

// F1, F3, L1, L4
extern void DMA1_Channel7_IRQHandler() {
    HAL_DMA_IRQHandler(dmaHandles[7]);
}

// F1, F3, L1, L4
extern void DMA2_Channel1_IRQHandler() {
    HAL_DMA_IRQHandler(dmaHandles[1 + 8]);
}

// F1, F3, L1, L4
extern void DMA2_Channel2_IRQHandler() {
    HAL_DMA_IRQHandler(dmaHandles[2 + 8]);
}

// F1, F3, L1, L4
extern void DMA2_Channel3_IRQHandler() {
    HAL_DMA_IRQHandler(dmaHandles[3 + 8]);
}

// F1
#ifndef DMA2_Channel4_5_IRQHandler  //for f1 by huaweiwx   2017.6.20
extern void DMA2_Channel4_5_IRQHandler() {
    HAL_DMA_IRQHandler(dmaHandles[4 + 8]);
    HAL_DMA_IRQHandler(dmaHandles[5 + 8]);
}
#endif

// F1, F3, L1, L4
#ifndef DMA2_Channel4_IRQHandler     //for f1 by huaweiwx  2017.6.20
extern void DMA2_Channel4_IRQHandler() {
    HAL_DMA_IRQHandler(dmaHandles[4 + 8]);
}
#endif

// F1, F3, L1, L4
extern void DMA2_Channel5_IRQHandler() {
    HAL_DMA_IRQHandler(dmaHandles[5 + 8]);
}

// L4
extern void DMA2_Channel6_IRQHandler() {
    HAL_DMA_IRQHandler(dmaHandles[6 + 8]);
}

// L4
extern void DMA2_Channel7_IRQHandler() {
    HAL_DMA_IRQHandler(dmaHandles[7 + 8]);
}
