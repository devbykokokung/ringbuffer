#include "ringbufferwrapper.h"

#include <algorithm>

#ifndef NULL
#define NULL 0;
#endif

RingBufferWrapper::RingBufferWrapper(uint8_t* data, uint32_t len) noexcept
   : valid(false)
{
   _data.reset(data); // take ownership immediately
   if (!isMultipleTwo(len)) {
      return;
   }
   valid = ringBufferInit(&buffer, _data.get(), len);
   if (!valid) {
      _data.reset();
   }
}


RingBufferWrapper::RingBufferWrapper(uint32_t size) noexcept
   : valid(false)
{
   if (!isMultipleTwo(size)) {
      return;
   }
   _data = std::make_unique<uint8_t[]>(size);
   valid = ringBufferInit(&buffer, _data.get(), size);
   if (!valid) {
      _data.reset();
   }
}

RingBufferWrapper::~RingBufferWrapper() noexcept
{
   // _data unique_ptr frees the buffer automatically
}

bool RingBufferWrapper::isValid() noexcept
{
   return valid;
}

bool RingBufferWrapper::empty() noexcept
{
   return ringBufferEmpty(&buffer);
}

size_t RingBufferWrapper::length() noexcept
{
   return ringBufferLen(&buffer);
}

size_t RingBufferWrapper::freeSpace() noexcept
{
   return ringBufferFreeSpace(&buffer);
}

size_t RingBufferWrapper::capacity() noexcept
{
   return ringBufferMaxSize(&buffer);
}

size_t RingBufferWrapper::appendOne(uint8_t data) noexcept
{  
   return appendMultiple(&data, 1);
}

size_t RingBufferWrapper::appendMultiple(const uint8_t *data, size_t len) noexcept
{
   len = std::min(len, freeSpace());
   ringBufferAppendMultiple(&buffer, data, len);
   return len;
}

size_t RingBufferWrapper::peekOne(uint8_t* data) noexcept
{
   return peekMultiple(data, 1);
}

size_t RingBufferWrapper::getOne(uint8_t* data) noexcept
{
   return getMultiple(data, 1);
}

size_t RingBufferWrapper::getMultiple(uint8_t *dst, size_t len) noexcept
{  
   len = std::min(len, length());
   if (len == 0) { 
      return 0; 
   }
   ringBufferGetMultiple(&buffer, dst, len);
   return len; 
}

size_t RingBufferWrapper::peekMultiple(uint8_t *dst, size_t len) noexcept
{
   len = std::min(len, length());
   if (len == 0) { 
      return 0; 
   }
   ringBufferPeekMultiple(&buffer, dst, len);
   return len; 
}

size_t RingBufferWrapper::discardMultiple(size_t len) noexcept
{  
   len = std::min(len, length());
   if (len == 0) { 
      return 0; 
   }
   ringBufferDiscardMultiple(&buffer, len);
   return len;
}

void RingBufferWrapper::clean() noexcept
{
   ringBufferClear(&buffer);
}
