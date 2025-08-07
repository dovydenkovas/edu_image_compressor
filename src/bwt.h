#pragma once
#include "entity.h"


// Преобразование Барроуза-Уилера
vec_t bwt(vec_t arr, uint64_t block_size);

// Обратное преобразование Барроуза-Уилера
vec_t reversed_bwt(vec_t arr, uint64_t  block_size);
