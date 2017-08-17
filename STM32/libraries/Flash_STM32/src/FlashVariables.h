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

#ifndef FLASHVARIABLES_H_
#define FLASHVARIABLES_H_

#include "FlashBlock.h"

enum class PAGE_STATUS : uint32_t {
    ERASED = 0xFFFFFFFF,        /* Page is empty */
    RECEIVE_DATA = 0xEEEEEEEE,  /* Page is marked to receive data */
    VALID_PAGE = 0x00000000,    /* Page containing valid data */
};

enum class PAGE_OPERATION {
    READ_FROM_VALID_PAGE,
    WRITE_IN_VALID_PAGE
};

enum class PAGE_RESULT {
    VARIABLE_FOUND,
    VARIABLE_NOT_FOUND,
    VARIABLE_WRITTEN,
    INDEX_OUT_OF_RANGE,

    PAGE_VALID,
    NO_VALID_PAGE,
    PAGE_FULL,

    PAGE_WRITE_ERROR
};

#define PAGE_NOT_FOUND -1

template<int DATA_SIZE>
class FlashVariables {
public:
    FlashVariables(FlashBlock *block0, FlashBlock *block1) {
        entryPerPage = (block0->getSize() - statusMarkerSize) / entrySize;

        firstEntryToCheckForWrite = 0;
        lastEntryToCheckForRead = entryPerPage - 1;

        pages[0] = block0;
        pages[1] = block1;
    }

    bool read(uint16_t variableIndex, uint8_t *data);
    bool write(uint16_t variableIndex, uint8_t *data);

    bool format();

    int entryPerPage;

    int firstEntryToCheckForWrite;
    int lastEntryToCheckForRead;

    const int statusMarkerSize = sizeof(PAGE_STATUS);

    const int addressSize = sizeof(uint16_t);
    const int dataSize = DATA_SIZE;
    const int entrySize = ((addressSize + dataSize + 4 - 1) / 4) * 4;

    void setFormatListener(void (*formatListener)(void)) {
        this->formatListener = formatListener;
    }

private:
    bool init();

    PAGE_STATUS getPageStatus(int page);

    int findValidPage(PAGE_OPERATION operation);

    bool verifyPageFullyErased(int page);

    PAGE_RESULT verifyPageFullWriteVariable(uint16_t variableIndex, uint8_t *data);

    PAGE_RESULT pageTransfer(uint16_t address, uint8_t *data);

    FlashBlock *pages[2];

    bool inited = false;
    void (*formatListener)(void);

};

template<int DATA_SIZE>
PAGE_STATUS FlashVariables<DATA_SIZE>::getPageStatus(int page) {
    uint32_t pageStatus;

    pages[page]->read(0, 4, (uint8_t *)&pageStatus);

    return static_cast<PAGE_STATUS>(pageStatus);
}

template<int DATA_SIZE>
int FlashVariables<DATA_SIZE>::findValidPage(PAGE_OPERATION operation) {
    PAGE_STATUS status0 = getPageStatus(0);
    PAGE_STATUS status1 = getPageStatus(1);

    switch (operation) {
        case PAGE_OPERATION::WRITE_IN_VALID_PAGE:
            if (status1 == PAGE_STATUS::VALID_PAGE) {

                if (status0 == PAGE_STATUS::RECEIVE_DATA) {
                    return 0;
                } else {
                    return 1;
                }
            } else if (status0 == PAGE_STATUS::VALID_PAGE)    {

                if (status1 == PAGE_STATUS::RECEIVE_DATA) {
                    return 1;
                } else {
                    return 0;
                }
            } else {
                return PAGE_NOT_FOUND;
            }

        case PAGE_OPERATION::READ_FROM_VALID_PAGE:
            if (status0 == PAGE_STATUS::VALID_PAGE) {
                return 0;
            } else if (status1 == PAGE_STATUS::VALID_PAGE) {
                return 1;
            } else {
                return PAGE_NOT_FOUND ;
            }

        default:
            return 0;
    }
}

template<int DATA_SIZE>
bool FlashVariables<DATA_SIZE>::init() {
    inited = true;

    PAGE_STATUS status0 = getPageStatus(0);
    PAGE_STATUS status1 = getPageStatus(1);

    PRINT_INFO("Initializing FlashVariables. Page statuses: %8x, %8x", (uint32_t)status0, (uint32_t)status1);

    switch(status0) {
        case PAGE_STATUS::ERASED:
            if (status1 == PAGE_STATUS::VALID_PAGE) {

                if (!verifyPageFullyErased(0)) {
                    return pages[0]->erase(0, pages[0]->getSize());
                }

            } else if (status1 == PAGE_STATUS::RECEIVE_DATA) {

                if (!verifyPageFullyErased(0)) {
                    if (!pages[0]->erase(0, pages[0]->getSize())) {
                        return false;
                    }
                }

                uint32_t validPageMarker = static_cast<uint32_t>(PAGE_STATUS::VALID_PAGE);
                if (!pages[1]->write(0, statusMarkerSize, (uint8_t *)&validPageMarker)) {
                    return false;
                }

                return true;

            } else {

                return format();

            }

            break;

        case PAGE_STATUS::RECEIVE_DATA:

            if (status1 == PAGE_STATUS::VALID_PAGE) {

                for(int i=0; i<entryPerPage; i++) {
                    uint8_t buffer[dataSize];
                    if (read(i, buffer)) {
                        if (verifyPageFullWriteVariable(i, buffer) != PAGE_RESULT::VARIABLE_WRITTEN) {
                            return false;
                        }
                    }
                }

                uint32_t validPageMarker = static_cast<uint32_t>(PAGE_STATUS::VALID_PAGE);
                if (!pages[0]->write(0, statusMarkerSize, (uint8_t *)&validPageMarker)) {
                    return false;
                }

                return pages[1]->erase(0, pages[1]->getSize());

            } else if (status1 == PAGE_STATUS::ERASED) {

                if (!pages[1]->erase(0, pages[1]->getSize())) {
                    return false;
                }

                uint32_t validPageMarker = static_cast<uint32_t>(PAGE_STATUS::VALID_PAGE);
                return pages[0]->write(0, statusMarkerSize, (uint8_t *)&validPageMarker);

            } else {
                return format();
            }
            break;

        case PAGE_STATUS::VALID_PAGE:

            if (status1 == PAGE_STATUS::VALID_PAGE) {

                return format();

            } else if (status1 == PAGE_STATUS::ERASED) {

                if (!verifyPageFullyErased(1)) {
                    if (!pages[1]->erase(0, pages[1]->getSize())) {
                        return false;
                    }
                }

            } else {

                for(int i=0; i<entryPerPage; i++) {
                    uint8_t buffer[dataSize];
                    if (read(i, buffer)) {
                        if (verifyPageFullWriteVariable(i, buffer) != PAGE_RESULT::VARIABLE_WRITTEN) {
                            return false;
                        }
                    }
                }

                uint32_t validPageMarker = static_cast<uint32_t>(PAGE_STATUS::VALID_PAGE);
                if (!pages[1]->write(0, statusMarkerSize, (uint8_t *)&validPageMarker)) {
                    return false;
                }

                return pages[0]->erase(0, pages[0]->getSize());

            }
            break;

        default:

            return format();
      }

    return true;
}

template<int DATA_SIZE>
bool FlashVariables<DATA_SIZE>::verifyPageFullyErased(int page) {

  for(size_t offset = 0; offset < pages[page]->getSize(); offset += 4) {

      uint32_t value;

      pages[page]->read(offset, 4, (uint8_t *)&value);

      if (value != static_cast<uint32_t>(PAGE_STATUS::ERASED)) {
          return false;
      }
  }
  return true;
}

template<int DATA_SIZE>
bool FlashVariables<DATA_SIZE>::read(uint16_t variableIndex, uint8_t *data) {
    if (!inited) init();

    int validPage = 0;

    validPage = findValidPage(PAGE_OPERATION::READ_FROM_VALID_PAGE);

    if (validPage == PAGE_NOT_FOUND) {
        PRINT_ERROR("No valid page");

        return false;
    }

    int endOffset = lastEntryToCheckForRead * entrySize + statusMarkerSize;

    for(int pageOffset = endOffset; pageOffset>0; pageOffset -= entrySize) {

        uint16_t index;
        pages[validPage]->read(pageOffset, addressSize, (uint8_t*)&index);

        if (index == variableIndex) {

            pages[validPage]->read(pageOffset + addressSize, dataSize, data);

            return true;
        }

    }

    memset(data, 0, dataSize);

    return false;

}

template<int DATA_SIZE>
bool FlashVariables<DATA_SIZE>::write(uint16_t variableIndex, uint8_t *data) {
    if (!inited) init();

    if (variableIndex > entryPerPage) {
        PRINT_ERROR("Variable index %u out of range, max: %u", variableIndex, entryPerPage);
        return false;
    }

    PAGE_RESULT status = verifyPageFullWriteVariable(variableIndex, data);

    if (status == PAGE_RESULT::PAGE_FULL) {
        status = pageTransfer(variableIndex, data);
    }

    return status == PAGE_RESULT::VARIABLE_WRITTEN;
}

template<int DATA_SIZE>
bool FlashVariables<DATA_SIZE>::format(void) {
    PRINT_INFO("Formatting");

    if (!verifyPageFullyErased(0)) {
        if (!pages[0]->erase(0, pages[0]->getSize())) {
            PRINT_ERROR("Failed to erase page 0");
            return false;
        }
    }

    uint32_t validPageMarker = static_cast<uint32_t>(PAGE_STATUS::VALID_PAGE);
    if (!pages[0]->write(0, 4, (uint8_t *)&validPageMarker)) {
        PRINT_ERROR("Failed to write page marker");
        return false;
    }


    if (!verifyPageFullyErased(1)) {
        if (!pages[1]->erase(0, pages[1]->getSize())) {
            PRINT_ERROR("Failed to erase page 0");
            return false;
        }
    }

    if (formatListener != NULL) {
        formatListener();
    }

    return true;
}

template<int DATA_SIZE>
PAGE_RESULT FlashVariables<DATA_SIZE>::verifyPageFullWriteVariable(uint16_t variableIndex, uint8_t *data) {

    int validPage = findValidPage(PAGE_OPERATION::WRITE_IN_VALID_PAGE);

    if (validPage == PAGE_NOT_FOUND) {
        PRINT_ERROR("No valid page found");
        return  PAGE_RESULT::NO_VALID_PAGE;
    }

    for(int entryIndex = firstEntryToCheckForWrite; entryIndex < entryPerPage; entryIndex++) {

        int pageOffset = entryIndex * entrySize + statusMarkerSize;

        uint32_t value;
        pages[validPage]->read(pageOffset, 4, (uint8_t*)&value);

        if (value == static_cast<uint32_t>(PAGE_STATUS::ERASED)) {
            PRINT_DEBUG("Empty space found at %u for variable %u", pageOffset, variableIndex);

            for(int i = 4; i < entrySize; i += 4) {
                int remaining = 4;
                if (dataSize + addressSize - i < remaining) remaining = dataSize + addressSize - i;

                uint8_t buf[4] = {0};
                memcpy(buf, data + i - addressSize, remaining);

                pages[validPage]->write(pageOffset + i, 4, buf);
            }

            value = variableIndex;
            value |= data[0] << 16;

            if (dataSize > 1) {
                value |= data[1] << 24;
            }

            pages[validPage]->write(pageOffset, 4, (uint8_t*)&value);

            if (lastEntryToCheckForRead < entryIndex) {
                lastEntryToCheckForRead = entryIndex;
            }

            firstEntryToCheckForWrite = entryIndex;

            return PAGE_RESULT::VARIABLE_WRITTEN;
        }

    }

    return PAGE_RESULT::PAGE_FULL;
}

template<int DATA_SIZE>
PAGE_RESULT FlashVariables<DATA_SIZE>::pageTransfer(uint16_t variableIndex, uint8_t *data) {
    PRINT_INFO("Transferring data to other page");

    uint32_t newPage;

    int validPage = findValidPage(PAGE_OPERATION::READ_FROM_VALID_PAGE);

    if (validPage == 1) {
        newPage = 0;
    } else if (validPage == 0) {
        newPage = 1;
    } else {
        PRINT_ERROR("No valid page");
        return PAGE_RESULT::NO_VALID_PAGE;       /* No valid Page */
    }

    uint32_t receiveDataMarker = static_cast<uint32_t>(PAGE_STATUS::RECEIVE_DATA);

    if (!pages[newPage]->write(0, statusMarkerSize, (uint8_t *)&receiveDataMarker)) {
        PRINT_ERROR("Could not write page marker");

        return PAGE_RESULT::PAGE_WRITE_ERROR;
    }

    firstEntryToCheckForWrite = 0;

    if (verifyPageFullWriteVariable(variableIndex, data) != PAGE_RESULT::VARIABLE_WRITTEN) {
        PRINT_ERROR("Could not write variable");

        return PAGE_RESULT::PAGE_WRITE_ERROR;
    }

    for(int i=0; i<entryPerPage; i++) {

        if (i == variableIndex) {
            continue;
        }

        uint8_t buffer[dataSize];

        if (read(i, buffer)) {

            bool valueSameAsDefault = true;

            for(size_t j=0; j<sizeof(buffer); j++) {
                if (buffer[j] != 0) {
                    valueSameAsDefault = false;
                    break;
                }
            }

            if (valueSameAsDefault) continue;

            if (verifyPageFullWriteVariable(i, buffer) != PAGE_RESULT::VARIABLE_WRITTEN) {
                PRINT_ERROR("Could not transfer variable");

                return PAGE_RESULT::PAGE_WRITE_ERROR;
            }
        }
    }

    lastEntryToCheckForRead = firstEntryToCheckForWrite;

    pages[validPage]->erase(0, pages[validPage]->getSize());

    uint32_t validPageMarker = static_cast<uint32_t>(PAGE_STATUS::VALID_PAGE);
    if (!pages[newPage]->write(0, statusMarkerSize, (uint8_t *)&validPageMarker)) {
        PRINT_ERROR("Could not write page marker");

        return PAGE_RESULT::PAGE_WRITE_ERROR;
    }

    return PAGE_RESULT::VARIABLE_WRITTEN;
}

#endif
