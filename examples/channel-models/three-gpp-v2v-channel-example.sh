#!/bin/bash

# Copyright (c) 2020, University of Padova, Dep. of Information Engineering, SIGNET lab
#
# SPDX-License-Identifier: GPL-2.0-only

#
#  Plot the traces generated by three-gpp-vehicular-channel-condition-model-example
#

cat >aa <<EOL
set terminal gif animate delay 100
set output 'map.gif'
set view map
set style fill transparent solid 0.5
unset key
set style fill  transparent solid 0.35 noborder
set style circle radius 5

do for [i=0:299] {
  set multiplot layout 1,2
    set zrange [i-1:i]
    set xrange [-25:600]
    set yrange [-25:1000]
    set xlabel 'X [m]'
    set ylabel 'Y [m]'
    set xtics
    set ytics
    load 'buildings.txt'
    splot 'example-output.txt' u 2:3:1 with circles lc rgb "blue", 'example-output.txt' u 4:5:1 with circles lc rgb "red"
    set object 101 rect from -25,-25 to 1400,1000 fc rgb "white"
    set xrange [i-0.001:i+0.001]
    set yrange [i-0.001:i+0.001]
    unset xlabel
    unset ylabel
    unset xtics
    unset ytics
    plot 'example-output.txt' using 1:1:1 with labels offset -10, 0, 'example-output.txt' using 1:1:6 with labels offset 10, 0
    unset object 101
  unset multiplot
}

reset
set terminal png
set output 'snr.png'
set xlabel 'Time [s]'
set ylabel 'SNR [dB]'
set xtics
set ytics
set grid
set xrange [0:40]
set yrange [-20:100]
plot 'example-output.txt' u 1:7 w l
EOL
gnuplot aa
rm aa
# rm out.txt
