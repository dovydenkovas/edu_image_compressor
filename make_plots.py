from matplotlib import pyplot as plt
import numpy as np


with open("imgs/img-com.csv") as ic, \
     open("imgs/img-decom.csv") as id, \
     open("imgs/ph-com.csv") as pc, \
     open("imgs/ph-decom.csv") as pd:
    bwt_block_sizes = [1, 2.5, 5, 10, 20, 30, 40, 50, 60]
    n_test_images = 11
    img_encode_speed = []
    img_decode_speed = []
    img_archive_rate = []

    es_sum = 0
    ar_sum = 0
    ds_sum = 0

    for i, line in enumerate(ic):
      _, ar, es = map(float, line.split())
      _, _,  ds = map(float, id.readline().replace("inf", "0").split())
      es_sum += es 
      ar_sum += ar
      ds_sum += ds 
      if (i+1) % n_test_images == 0:
          img_encode_speed.append(es_sum/n_test_images)
          img_decode_speed.append(ds_sum/n_test_images)
          img_archive_rate.append(ar_sum/n_test_images)

          print(round(img_encode_speed[-1], 2), round(img_decode_speed[-1], 2), round(img_archive_rate[-1], 2))
          es_sum = 0
          ar_sum = 0
          ds_sum = 0

    photo_encode_speed = []
    photo_decode_speed = []
    photo_archive_rate = []
    print("Фото")
    for i, line in enumerate(pc):
       _, ar, es = map(float, line.split())
       _, _,  ds = map(float, pd.readline().replace("inf", "0").split())
       es_sum += es 
       ar_sum += ar
       ds_sum += ds 
       if (i+1) % n_test_images == 0:
           photo_encode_speed.append(es_sum/n_test_images)
           photo_decode_speed.append(ds_sum/n_test_images)
           photo_archive_rate.append(ar_sum/n_test_images)

           print(round(photo_encode_speed[-1], 2), round(photo_decode_speed[-1], 2), round(photo_archive_rate[-1], 2))
           es_sum = 0
           ar_sum = 0
           ds_sum = 0

    print("Скорость кодирования схем", img_encode_speed)
    print("Скорость декодирования схем", img_decode_speed)
    print("Скорость кодирования фотографий", photo_encode_speed)
    print("Скорость декодирования фотографий", photo_decode_speed)
    print("% Сжатия", img_archive_rate)
    
    print(bwt_block_sizes)
    print(img_encode_speed)
    print(photo_encode_speed)
    plt.plot(bwt_block_sizes, img_encode_speed, label="Скорость кодирования рисунков")
    plt.plot(bwt_block_sizes, photo_encode_speed, label="Скорость кодирования фотографий")
    plt.grid()
    plt.legend()
    plt.xlabel("Размер блока, Кбайт")
    plt.ylabel("Скорость, Мб/с")

    plt.figure()
    plt.plot(bwt_block_sizes, img_decode_speed, label="Скорость декодирования рисунков")
    plt.plot(bwt_block_sizes, photo_decode_speed, label="Скорость декодирования фотографий")
    plt.grid()
    plt.legend()
    plt.xlabel("Размер блока, Кбайт")
    plt.ylabel("Скорость, Мб/с")

    plt.figure()
    plt.plot(bwt_block_sizes, img_archive_rate, label="Степень сжатия рисунков")
    plt.plot(bwt_block_sizes, photo_archive_rate, label="Степень сжатия фотографий")
    plt.grid()
    plt.legend()
    plt.xlabel("Размер блока, Кбайт")
    plt.ylabel("Степень сжатия, %")

    plt.show()
