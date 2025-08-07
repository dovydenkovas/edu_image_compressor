#include "bmp.h"
#include "bwt.h"
#include "cli.h"
#include "entity.h"
#include "profiler.h"
#include "rle.h"

// Кодирование изображения.
vec_t encode(vec_t input, uint64_t block_size) {
  // При отключенном BWT
  if (block_size == 0)
   return rle(input);

  // Преобразование RLE по пикселям.
  vec_t first_rle = rle3(input);
  // Если оно устраняет избыточность и сжимает более 5%,
  // применяем его при дальнейшем кодировании.
  bool is_rle3 = first_rle.size() < 0.95 * input.size();

  vec_t prepared_data;
  if (is_rle3)
    prepared_data = bwt(first_rle, block_size);
  else
    prepared_data = bwt(input, block_size);

  vec_t encodeed = rle(prepared_data);
  // Добавляем информацию о первичном RLE
  // преобразовании в сохраняемое изображение.
  encodeed.insert(encodeed.begin(), is_rle3 ? 1 : 0);
  return encodeed;
}

// Декодирование изображения.
vec_t decode(vec_t input, uint64_t block_size) {
  // При отключенном BWT.
  if (block_size == 0)
    return reversed_rle(input);
  // Чтение флага первичного RLE преобразования.
  bool is_rle3 = input[0];
  input.erase(input.begin());

  vec_t decoded = reversed_rle(input);
  vec_t data = reversed_bwt(decoded, block_size);

  if (is_rle3)
    return reversed_rle3(data);
  return data;
}

void run(Arguments &args) {
  check_output_file_exist(args.output, args.force);
  BMPReader parser(args.all, !args.encode);
  vec_t data = parser.open(args.input);

  Profiler prof;
  vec_t res;
  prof.begin_stopwatch();
  if (args.encode) {
    res = encode(data, args.block_size);
    prof.end_stopwatch();
    prof.print_results(data.size(), res.size());
  }
  else {
    res = decode(data, args.block_size);
    prof.end_stopwatch();
    prof.print_results(res.size(), data.size());
  }

  parser.save(res, args.output, args.force);
}

int main(int argc, char *argv[]) {
  Arguments args = parse_arguments(argc, argv);
  run(args);
}
