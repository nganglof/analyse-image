#!/bin/bash

./make-se 2 10 disk-10.png
./make-se 2 5 disk-5.png
./make-se 2 3 disk-3.png

./erosion disk-5.png cell.png cell-res.png
./opening disk-5.png cell-res.png cell-res.png
./closing disk-5.png cell-res.png cell-res.png

./binarization 135 255 cell-res.png cell-res.png
./erosion disk-5.png cell-res.png cell-res.png
./erosion disk-3.png cell-res.png cell-res.png
./erosion disk-3.png cell-res.png cell-res.png
./erosion disk-3.png cell-res.png cell-res.png

./dilation disk-10.png cell-res.png cell-res.png
./dilation disk-5.png cell-res.png cell-res.png
./dilation disk-5.png cell-res.png cell-res.png

./dilation disk-3.png cell-res.png cell-res-boundary.png
./substract cell-res-boundary.png cell-res.png cell-res-boundary.png

./labeling-color cell-res.png cell-r.png cell-res-color.png

./add-boundary cell-res-boundary.png cell.png cell-final.png

pvisu cell-res.png
pvisu cell-res-boundary.png
pvisu cell-res-color.png
pvisu cell-final.png
