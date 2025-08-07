#!/bin/bash -e

cd imgs
rm -f *.rle *.csv uz*
for BWT_BLOCK in 1000 2500 5000 10000 20000 30000 40000 50000 60000; do
    echo $BWT_BLOCK
    echo
    echo "Рисунки (малое количество цветов)"
    echo

    for img in image* ; do
      echo Кодирование $img `../compressor $img -s$BWT_BLOCK -fmo res.rle 2>> img-com.csv`
      echo Декодирование $img `../compressor res.rle -s$BWT_BLOCK -fmdo uz-$img 2>> img-decom.csv`
      diff $img uz-$img
    done

    echo
    echo "Фотографии"
    echo

    for img in photo* ; do
      echo Кодирование $img `../compressor $img -s$BWT_BLOCK -fmo res.rle 2>> ph-com.csv`
      echo Декодирование $img `../compressor res.rle -s$BWT_BLOCK -fmdo uz-$img 2>> ph-decom.csv`
      diff $img uz-$img
    done
done
