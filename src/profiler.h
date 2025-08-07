#pragma once
#include <cstddef>
#include <cstdint>
#include <ctime>

// Измерение характеристик кодирования
class Profiler {
  int64_t timestamp_begin;
  int64_t timestamp_end;

  public:
    void begin_stopwatch();
    void end_stopwatch();
    void print_results(size_t src_size, size_t dst_size);
};
