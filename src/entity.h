// Основные типы и структуры данных
#pragma once

#include <vector>
#include <cstdint>
#include <string>
#include <span>

// #define DEBUG 1
#ifdef DEBUG
  #define LOG(X) { std::cout << __FILE__ << ":" << __LINE__ << ": " << X << std::endl; }
  #define LOG2(K, V) { std::cout << __FILE__ << ":" << __LINE__ << ": " << K << " " << V << std::endl; }
#else
  #define LOG(X)
  #define LOG2(K, V)
#endif

using vec_t = std::vector<uint8_t>;
using view_t = std::span<uint8_t>;

// Параметры запуска кодера/декодера
struct Arguments {
  bool encode;    // true -c or false -d
  bool force;     // -f
  bool all;       // true -a or false -m
  uint64_t block_size; // -s
  std::string input;
  std::string output;
};


// Элемент циклического сдвига.
class CyclicShift {
  uint8_t *arr;
  size_t arr_size;
  size_t shift;

public:
  void init(uint8_t *arr_, size_t size_, size_t shift_) {
    arr = arr_;
    arr_size = size_;
    shift = shift_;
  }

  inline uint8_t operator[](size_t index) const {
    size_t buff = shift + index;
    return arr[(arr_size <= buff ? buff - arr_size : buff)];
  }

  inline bool is_shift0() const {
    return shift == 0;
  }
};
