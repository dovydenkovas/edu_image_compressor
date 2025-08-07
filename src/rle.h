// Кодирование длин серий
#pragma once

#include "entity.h"

// Побайтовое кодирование
vec_t rle(vec_t arr);
vec_t reversed_rle(vec_t arr);

// Попиксельное кодирование
vec_t rle3(vec_t arr);
vec_t reversed_rle3(vec_t arr);
