## SDIO / SDMMC

To use the hardware SDIO, include it in your code:
```c++
#include "SDIO.h"

SDIOClass sdio;
```

#### **`begin();`**

Connects to the SDIO card, using 4-bit mode.

Returns true on success, false on failure.
On failure, use `getError()`.

#### **`uint8_t end();`**

Disconnects from the SDIO card.

#### **`uint32_t getBlockSize();`**

Returns the block size.

#### **`uint8_t readBlocks(uint32_t block, uint8_t* dst, size_t blocks);`**

Read the given number of blocks into dst buffer in blocking mode.

#### **`uint8_t writeBlocks(uint32_t block, const uint8_t* src, size_t blocks);`**

Writes the given number of blocks from the buffer in blocking mode.

#### **`uint8_t eraseBlocks(uint32_t startBlock, uint32_t endBlock);`**

Erases the blocks from start block to end block.

#### **`HAL_SD_ErrorTypedef getError();`**

Returns the last error type.

#### **`uint64_t getCapacity();`**

Returns the SD Card capacity.

#### **`uint8_t getType();`**

Returns the type of the SD Card 

#### **`uint16_t getRCA();`**

Returns the RCA register.

#### **`HAL_SD_CIDTypedef getCID();`**

Returns the CID register.

#### **`HAL_SD_CSDTypedef getCSD();`**

Returns the CSD register.
