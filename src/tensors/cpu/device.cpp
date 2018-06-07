#include "tensors/device.h"
#include <iostream>

#ifdef _WIN32
#include <malloc.h>
#endif
#include <stdlib.h>

namespace marian {
namespace cpu {

Device::~Device() {
  free(data_);
  data_ = nullptr;
  size_ = 0;
}

#ifdef _WIN32
void* aligned_alloc(size_t alignment, size_t size) {
    return _aligned_malloc(size, alignment);
}
#endif

void Device::reserve(size_t size) {
  size = align(size);
  ABORT_IF(size < size_ || size == 0,
           "New size must be larger than old size and larger than 0");

  if(data_) {
    uint8_t *temp = static_cast<uint8_t *>(aligned_alloc(alignment_, size));
    std::copy(data_, data_ + size_, temp);
    free(data_);
    data_ = temp;
  } else {
    data_ = static_cast<uint8_t *>(aligned_alloc(alignment_, size));
  }
  size_ = size;
}
}
}