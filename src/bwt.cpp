#include "bwt.h"
#include "entity.h"
#include "sort.h"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <exception>
#include <span>
#include <vector>


using namespace std;

// Преобразует uint8_t[8] в uint64_t
uint64_t view2offset(const view_t &data) {
  uint8_t *arr = data.data();
  uint64_t offset = *(uint64_t *)arr;
  return offset;
}

// Преобразует uint64_t в uint8_t[8]
vec_t offset2vec(uint64_t &offset) {
  uint8_t *arr = (uint8_t *)(&offset);
  vec_t vec(8);
  copy(arr, arr + 8, vec.begin());
  return vec;
}

// Преобразование Барроуза-Уилера для одного блока
vec_t single_bwt(uint8_t *src, size_t size, uint64_t &offset) {
  // Создание таблицы сдвигов.
  CyclicShift *table = new CyclicShift[size];
  for (int i = 0; i < size; ++i)
    table[i].init(src, size, i);

  // Сортировка таблицы сдвигов.
  bwt_sort(table, size);

  // Последний столбец таблицы сдвигов - результат работы алгоритма.
  vec_t res(size);
  size_t last_index = size - 1;
  for (uint64_t i = 0; i < size; ++i)
    res[i] = table[i][last_index];

  // Определение номера исходной строки.
  for (uint64_t i = 0; i < size; ++i)
    if (table[i].is_shift0()) {
      offset = i;
      return res;
    }

  throw exception();
}

// Преобразование Барроуза-Уилера с разбиением по блокам.
vec_t bwt(vec_t source_data, uint64_t block_size) {
  vec_t result;
  result.reserve(source_data.size() +
                 sizeof(uint64_t) * (source_data.size() / block_size + 1));

  uint64_t source_size = source_data.size();
  for (uint64_t i = 0; i < source_size; i += block_size) {
    uint64_t offset = 0;
    vec_t block_bwt = single_bwt(source_data.data() + i,
                                 min(block_size, source_size - i), offset);

    vec_t encoded_offset = offset2vec(offset); // Перевод uint64_t в uint8_t[8]
    for (auto &a : block_bwt)
      result.push_back(a);
    for (auto &a : encoded_offset)
      result.push_back(a);
  }
  return result;
}

// Обратное преобразование Барроуза-Уилера для одного блока
vec_t single_reversed_bwt(const view_t arr, uint64_t offset) {
  // Cчитаем частоты символов.
  vector<uint64_t> count(256);
  for (auto &a : arr)
    ++count[a];

  // Упорядочиваем символы, чтобы получить первый столбец исходной матрицы
  uint64_t sum = 0; // Количество символов меньших i-го.
  for (uint64_t i = 0; i < 256; ++i) {
    sum += count[i];
    // первая позиция символа i в первом столбце матрицы сдвигов.
    count[i] = sum - count[i];
  }

  // Cоздаем вектор обратного преобразования.
  vector<uint64_t> t(arr.size());
  for (uint64_t i = 0; i < t.size(); ++i) {
    // t[индекс элемента в первом столбце] = индекс элемента в последнем столбце.
    t[count[arr[i]]] = i;
    // Индекс элемента в первом столбце перемещаем на следующий такой элемент.
    count[arr[i]]++;
  }

  // Восстанавливаем исходный текст.
  uint64_t j = t[offset];
  vec_t result(t.size());
  for (uint64_t i = 0; i < result.size(); ++i) {
    result[i] = arr[j];
    j = t[j];
  }
  return result;
}

// Обратное преобразование Барроуза-Уилера с разбиением по блокам.
vec_t reversed_bwt(vec_t source_data, uint64_t block_size) {
  vec_t result;
  result.reserve(source_data.size());

  uint64_t source_size = source_data.size();
  for (uint64_t i = 0; i < source_size; i += block_size + sizeof(uint64_t)) {
    auto block_view = span{source_data}.subspan(
        i, min(block_size, source_size - i - sizeof(uint64_t)));
    auto offset_view = span{source_data}.subspan(
        i + min(block_size, source_size - i - sizeof(uint64_t)),
        sizeof(uint64_t));

    uint64_t offset = view2offset(offset_view);
    vec_t decoded = single_reversed_bwt(block_view, offset);
    for (auto &a : decoded)
      result.push_back(a);
  }
  return result;
}
