#include "bmp.h"
#include "entity.h"

#include <cstdint>
#include <endian.h>
#include <filesystem>
#include <fstream>
#include <ios>
#include <iostream>
#include <iterator>
#include <ostream>
#include <string>

// Проверяет существование файла.
void check_output_file_exist(std::string filename, bool force) {
  if (!force && std::filesystem::exists(filename)) {
    std::cout << "Файл " << filename
              << " существует. Укажите другое имя выходного файла или "
                 "импользуйте -f для перезаписи."
              << std::endl;
    exit(1);
  }
}

// Открывает изображение, сохраняет заголовок
// и выполняет обход матрицы пикселей.
vec_t BMPReader::open_image(std::string filename) {
  std::ifstream ifile(filename, std::fstream::binary);
  if (!ifile) {
    std::cerr << "Не могу открыть файл\n";
    exit(1);
  }

  ifile >> std::noskipws;

  // Чтение заголовка файла
  char bytes_file[sizeof(BITMAPFILEHEADER)];
  for (auto &byte : bytes_file)
    ifile >> byte;

  BITMAPFILEHEADER *bmp_header =
      reinterpret_cast<BITMAPFILEHEADER *>(bytes_file);

  if (bmp_header->bfType != 0x4d42) {
    std::cerr << "Неправильный формат файла\n";
    exit(1);
  }
  // Чтение заголовка изображения
  char bytes_info[sizeof(BITMAPINFOHEADER)];
  for (auto &byte : bytes_info)
    ifile >> byte;

  BITMAPINFOHEADER *bmp_info = reinterpret_cast<BITMAPINFOHEADER *>(bytes_info);

  // Сохранение заголовка файла и размеров изображения
  ifile.seekg(0);
  header.resize(bmp_header->bfOffBits);
  ifile.read((char *)header.data(), bmp_header->bfOffBits);
  width = bmp_info->biWidth;
  height = bmp_info->biHeight;

  // Чтение пикселей
  ifile.seekg(bmp_header->bfOffBits);
  vec_t result;
  uint8_t b;
  int x = 0;
  while (ifile >> b)
    result.push_back(b);

  LOG2("Размер заголовка:", bmp_header->bfOffBits);
  LOG2("Ширина изображения:", bmp_info->biWidth);
  LOG2("Высота изображения:", bmp_info->biHeight);
  LOG2("Глубина цвета:", bmp_info->biBitCount);
  return result;
}

// Читает файл изображения в вектор.
vec_t BMPReader::open(std::string input) {
  if (encode_all) {
    std::ifstream ifile(input, std::ios::binary);
    if (!ifile) {
      std::cerr << "Входного файла не существует." << std::endl;
      exit(1);
    }

    ifile >> std::noskipws;
    vec_t res(std::istream_iterator<uint8_t>(ifile), {});
    return res;
  } else
    return open_image(input);
}

// Сохранение изображения в файл.
void BMPReader::save(vec_t data, std::string output, bool force) {
  std::ofstream ofile(output, std::ios::binary);
  if (!ofile)
    return;

  if (!encode_all)
    ofile.write((char *)header.data(), header.size());

  ofile.write((char *)data.data(), data.size());
}
