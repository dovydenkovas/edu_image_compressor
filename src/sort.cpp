#include "sort.h"
#include "entity.h"

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <stdexcept>


// Сортировка Бентли-Седжвика.
void quick_sort(CyclicShift *arr, size_t begin_i, size_t end_i,
                size_t str_size, size_t pos, uint8_t middle_value);

// Вспомогательные массивы для сортировки.
static CyclicShift less[65536];
static CyclicShift equal[65536];
static CyclicShift greater[65536];

void bwt_sort(CyclicShift *arr, size_t size) {
  if (size > 65536)
    throw std::out_of_range("Размер блока не должен превышать 65536 байт.");
  quick_sort(arr, 0, size, size, 0, 128);
}

void quick_sort(CyclicShift *arr, size_t begin_i, size_t end_i, size_t str_size,
               size_t pos, uint8_t middle_value) {
  if (pos == str_size || end_i - begin_i <= 1)
    return;

  // Выделение памяти для массивов less, equal, greater.
  size_t less_size = 0;
  size_t greater_size = 0;
  size_t equal_size = 0;

  // Деление элементов вектора на меньших, равных и больших данному.
  uint8_t less_min = 255;
  uint8_t less_max = 0;
  uint8_t equal_min = 255;
  uint8_t equal_max = 0;
  uint8_t greater_min = 255;
  uint8_t greater_max = 0;

  for (size_t i = begin_i; i < end_i; ++i) {
    uint8_t v = arr[i][pos];
    // Элемент меньше стреднего
    if (v < middle_value) {
      less[less_size] = arr[i];
      ++less_size;
      if (v > less_max)
        less_max = v;
      if (v < less_min)
        less_min = v;
      // Элемент больше среднего
    } else if (v > middle_value) {
      greater[greater_size] = arr[i];
      ++greater_size;
      if (v > greater_max)
        greater_max = v;
      if (v < greater_min)
        greater_min = v;
      // Элемент равен среднему
    } else {
      equal[equal_size] = arr[i];
      ++equal_size;
      if (arr[i][pos + 1] > equal_max)
        equal_max = arr[i][pos + 1];
      if (arr[i][pos + 1] < equal_min)
        equal_min = arr[i][pos + 1];
    }
  }

  int k = begin_i;
  for (int i = 0; i < less_size; ++i) {
    arr[k] = less[i];
    ++k;
  }

  for (int i = 0; i < equal_size; ++i) {
    arr[k] = equal[i];
    ++k;
  }

  for (int i = 0; i < greater_size; ++i) {
    arr[k] = greater[i];
    ++k;
  }

  quick_sort(arr, begin_i, begin_i + less_size, str_size, pos,
            (less_max + less_min) / 2);
  quick_sort(arr, begin_i + less_size, begin_i + less_size + equal_size,
            str_size, pos + 1, (equal_max + equal_min) / 2);
  quick_sort(arr, begin_i + less_size + equal_size, end_i, str_size, pos,
            (greater_max + greater_min) / 2);
}

inline void preprocess_less_elements(CyclicShift *arr, int32_t begin,
                                     int32_t end,
                                     uint16_t *n_elements_less_than) {
  uint16_t n_elements[256] = {0};
  std::fill(n_elements_less_than, n_elements_less_than + 256, begin);

  for (size_t i = begin; i < end; ++i)
    ++n_elements[arr[i][0]];

  for (size_t i = 1; i < 256; ++i) {
    n_elements_less_than[i] = n_elements[i - 1];
    n_elements[i] += n_elements[i - 1];
  }
}

inline void fallback_sort(CyclicShift *arr, uint16_t begin, uint16_t end,
                          int32_t str_size, int32_t init_row) {
  // LOG("Инициализация массивов");

  CyclicShift buffer_1[end - begin];
  CyclicShift buffer_2[end - begin];
  uint16_t n_elements_less_than[256];
  preprocess_less_elements(arr, begin, end, n_elements_less_than);
  uint16_t equal_elements[256];

  CyclicShift *prev = buffer_1;
  CyclicShift *next = buffer_2;
  std::copy(arr + begin, arr + end, prev);
  // LOG("Запуск цикла");
  for (int32_t row = str_size - 1; row >= init_row; --row) {
    // Очистка количеств элементов равных текущему, но стоящих раньше.
    std::fill(equal_elements, equal_elements + 256, uint16_t(0));

    // O(N) сортировка по столбцу row
    for (uint16_t i = begin; i != end; ++i) {
      uint8_t elem = prev[i][row];
      next[n_elements_less_than[elem] + equal_elements[elem]++] = prev[i];
    }
    std::swap(next, prev);
  }
  std::copy(prev, prev + end - begin, arr + begin);
}
