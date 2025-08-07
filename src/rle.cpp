#include "rle.h"
#include "entity.h"

// Флаг блока неповторяющихся символов.
constexpr uint8_t NON_REPEAT_FLAG = 0b10000000;
// Максимальная длина серии.
constexpr uint8_t MAX_REPEAT_SIZE = 0b01111111;

vec_t rle(vec_t arr) {
  vec_t res;
  res.reserve(arr.size());
  size_t i = 0;
  while (i < arr.size()) {
    size_t j = i + 1;
    // Подсчет количества одинаковых пикселей.
    while (j < arr.size() && arr[i] == arr[j] && j - i + 1 < MAX_REPEAT_SIZE)
      ++j;

    // Если одинаковых элементов нет.
    if (j - i == 1) {
      // Подсчет длины неповторяющейся последовательности.
      while (j < arr.size() && arr[j - 1] != arr[j] &&
             j - i + 1 < MAX_REPEAT_SIZE)
        ++j;
      --j;
      // Запись длины последовательности.
      res.push_back((j - i + 1) | NON_REPEAT_FLAG);
      // Запись элементов последовательности.
      while (i <= j) {
        res.push_back(arr[i]);
        ++i;
      }
    } else { // Сохранение длины последовательности одинаковых элементов.
      res.push_back(j - i + 1);
      res.push_back(arr[i]);
      i = j;
    }
  }
  return res;
}

vec_t reversed_rle(vec_t arr) {
  vec_t res;
  res.reserve(arr.size() * 2);

  int i = 0;
  while (i < arr.size()) {
    if (arr[i] & NON_REPEAT_FLAG) {
      // Восстановление последовательности неповторяющихся элементов.
      uint8_t size = arr[i] & MAX_REPEAT_SIZE;
      for (int j = 0; j != size; ++j)
        res.push_back(arr[i + 1 + j]);
      i += size + 1;
    } else {
      // Восстанововление серии повторяющихся элементов.
      for (int j = 1; j < arr[i]; ++j) {
        res.push_back(arr[i + 1]);
      }
      i += 2;
    }
  }
  return res;
}


vec_t rle3(vec_t arr) {
  vec_t res;
  res.reserve(arr.size());

  size_t i = 0;
  while (i < arr.size()) {
    size_t j = i + 3;
    // Подсчет количества одинаковых пикселей.
    while (j + 2 < arr.size()
      && arr[i] == arr[j]
      && arr[i+1] == arr[j+1]
      && arr[i+2] == arr[j+2]
      && (j - i)/3 + 1 < MAX_REPEAT_SIZE)
      j += 3;

    // Если одинаковых элементов нет.
    if (j - i == 3) {
      // Подсчет длины неповторяющейся последовательности.
      while (j + 2 < arr.size() && arr[j - 1] != arr[j]
            &&(arr[j-3] != arr[j]
            || arr[j-2] != arr[j+1]
            || arr[j-1] != arr[j+2])
            && j - i + 1 < MAX_REPEAT_SIZE)
        ++j;
      if (j + 2 >= arr.size()) j = arr.size();
      --j;

      // Указать длину последовательности.
      res.push_back((j - i + 1) | NON_REPEAT_FLAG);

      while (i <= j) {
        res.push_back(arr[i]);
        ++i;
      }
    } else { // Сохранение длины последовательности одинаковых элементов.
      res.push_back((j - i)/3+1);
      res.push_back(arr[i]);
      res.push_back(arr[i+1]);
      res.push_back(arr[i+2]);
      i = j;
    }
  }

  return res;
}

vec_t reversed_rle3(vec_t arr) {
  vec_t res;
  res.reserve(arr.size() * 2);

  int i = 0;
  while (i < arr.size()) {
    if (arr[i] & NON_REPEAT_FLAG) {
      uint8_t size = arr[i] & MAX_REPEAT_SIZE;
      for (int j = 0; j != size; ++j) {
        res.push_back(arr[i + 1 + j]);
      }
      i += size + 1;
    } else {
      for (int j = 1; j < arr[i]; ++j) {
        res.push_back(arr[i + 1]);
        res.push_back(arr[i + 2]);
        res.push_back(arr[i + 3]);
      }
      i += 4;
    }
  }
  return res;
}
