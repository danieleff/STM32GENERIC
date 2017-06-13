// =============================================================================
//
// Copyright (c) 2013-2016 Christopher Baker <http://christopherbaker.net>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// =============================================================================

#ifndef CRC32_H
#define CRC32_H

#if defined(SPARK) || defined(PARTICLE)
    #include "application.h"
#elif defined(ARDUINO)
    #if ARDUINO >= 100
        #include "Arduino.h"
    #else
        #include "WProgram.h"
    #endif
#endif

class CRC32
{
public:
  static uint32_t calculate(const void* data, size_t size);

public:
  CRC32() { reset(); }

  void reset();
  void update(uint8_t data);
  void update(const void* data, size_t size);
  uint32_t finalize(const void* data, size_t size);
  uint32_t finalize() const;

  // Deprecated API
  __attribute__ ((deprecated))
  static uint32_t checksum(const uint8_t* data, size_t size);

  __attribute__ ((deprecated))
  static uint32_t update(uint32_t checksum, uint8_t data);

  __attribute__ ((deprecated))
  static uint32_t update(uint32_t checksum, const uint8_t* data, size_t size);

private:
  uint32_t state;
};

#endif
