#!/bin/bash

./make-se 2 15 disk-15.png
./erosion disk-15.png gear.png gear-er1.png
./dilation disk-15.png gear-er1.png gear-er2.png

./substract gear.png gear-er2.png gear-res-nb.png
./binarization 200 255 gear-res-nb.png gear-res.png
./labeling-color gear-res.png gear-r.png gear-res-color.png

rm gear-er1.png
rm gear-er2.png
rm gear-res-nb.png
pvisu gear-res.png
pvisu gear-res-color.png