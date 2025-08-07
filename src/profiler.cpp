#include "profiler.h"
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <time.h>
#include <chrono>

#define DEBUG 

void Profiler::begin_stopwatch() {
 using namespace std::chrono;
 timestamp_begin = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

void Profiler::end_stopwatch() {
  using namespace std::chrono;
  timestamp_end = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
 }

void Profiler::print_results(size_t s, size_t d) {
  double r = 100.0 * (1.0 * s - d) / s;
  double delay = (timestamp_end - timestamp_begin)/1000.0;
  double speed = s / 1024.0 / delay / 1024;
  std::cout << "Операция выполнена за " << std::setprecision(4) << delay
            << " секунд" << std::endl
            << "Степень сжатия: " << r << "% " << std::endl
            << "Скорость обработки: " << std::setprecision(8) << speed << " Мб/с"  << std::endl;
  #ifdef DEBUG
    // Вывод информации для отчета.
    std::cerr << std::setprecision(4) << delay << " " << r <<  " " << std::setprecision(8) << speed << std::endl;
  #endif
}
