#pragma once
// Ввод-вывод изображений.

#include "entity.h"
#include <string>

// Проверяет существование файла.
void check_output_file_exist(std::string filename, bool force);

class BMPReader {
  bool encode_all;
  bool is_decode;
  vec_t header;
  size_t width, height;

  vec_t open_image(std::string filename);

public:
  BMPReader(bool encode_all, bool is_decode)
      : encode_all(encode_all), is_decode(is_decode) {}
  vec_t open(std::string input);
  void save(vec_t data, std::string output, bool force);
};


// Описание заголовка BMP файла.

using WORD = uint16_t;
using DWORD = uint32_t;
using LONG = int32_t;

#pragma pack(push, 1)
struct BITMAPFILEHEADER {
  WORD bfType;      // 2
  DWORD bfSize;     // 6
  WORD bfReserved1; // 8
  WORD bfReserved2; // 10
  DWORD bfOffBits;  // 14
};
#pragma pack(pop)

#pragma pack(push, 1)
struct BITMAPINFOHEADER {
  DWORD biSize;
  LONG biWidth;
  LONG biHeight;
  WORD biPlanes;
  WORD biBitCount;
  DWORD biCompression;
  DWORD biSizeImage;
  LONG biXPelsPerMeter;
  LONG biYPelsPerMeter;
  DWORD biClrUsed;
  DWORD biClrImportant;
};
#pragma pack(pop)
