#include "cli.h"
#include "entity.h"

#include <cctype>
#include <iostream>

#include <boost/program_options.hpp>
#include <boost/program_options/errors.hpp>

namespace po = boost::program_options;

using namespace std;

string generate_output_filename(string input_path) {
  if (input_path.size() > 5) {
    string_view extention{input_path.data() + (input_path.size() - 4), 4};
    if (extention == ".bmp")
      return input_path + ".rle";
  }
  cerr << "Невозможно определить имя выходного файла." << endl;
  exit(1);
}

// Обработка аргументов командной строки.
Arguments parse_arguments(int argc, char *argv[]) {
  po::options_description desc("Использование: compressor [args] <INPUT> \nПараметры");
  po::positional_options_description p;
  p.add("input", 1);
  desc.add_options()
    ("input,i", po::value<string>(), "Имя входного файла")
    ("output,o", po::value<string>(), "Имя выходного файла")
    ("force,f", "Перезаписать выходной файл")
    ("encode,c", "Кодировать изображение")
    ("decode,d", "Декодировать изображение")
    ("all,a", "Кодировать весь файл")
    ("matrix,m", "Кодировать только матрицу пикселей")
    ("size,s", po::value<uint64_t>()->default_value(2500), "Размер блока кодирования")
    ("help,h", "Вывод списка параметров");

  po::variables_map vm;
  try {
    po::store(
        po::command_line_parser(argc, argv).positional(p).options(desc).run(),
        vm);
  } catch (po::too_many_positional_options_error &e) {
    cerr << "Можно указать лишь один входной файл." << endl;
    exit(1);
  } catch (po::unknown_option &e) {
    cerr << "Незвестный параметр: " << e.get_option_name() << endl;
    exit(1);
  } catch (po::invalid_option_value &e) {
    cerr << "Ошибка обработки параметров: " << e.get_option_name() << endl;
    exit(1);
  } catch (po::multiple_occurrences &e) {
    cerr << "Параметр " << e.get_option_name() << " установлен более одного раза." << endl;
  }
  po::notify(vm);

  if (vm.count("help")) {
    cout << desc << "\n";
    exit(1);
  }


  Arguments args{false, false, false, 2500, "", ""};
  if (!vm.count("input")) {
    cerr << "Необходимо указать имя входного файла." << endl;
    exit(1);
  }
  args.input = vm["input"].as<string>();

  if (!vm.count("output"))
    args.output = generate_output_filename(args.input);
  else
    args.output = vm["output"].as<string>();

  args.encode = vm.count("encode");
  args.block_size = vm["size"].as<uint64_t>();
  args.all = vm.count("all");
  args.force = vm.count("force");

  if (args.block_size > 65536) {
    cerr << "Размер блока должен быть в диапазоне от 0 до 65536." << endl;
    exit(1);
  }

  if (args.encode && vm.count("decode")) {
    cerr << "Необходимо использовать лишь один параметр - кодирование или "
            "декодирование."
         << endl;
    exit(1);
  }
  args.encode = !vm.count("decode");

  if (args.all && vm.count("matrix")) {
    cerr << "Необходимо использовать лишь один параметр - работа со всем "
            "файлом или только с матрицей пикселей."
         << endl;
    exit(1);
  }
  args.all = !vm.count("matrix");

  return args;
}
