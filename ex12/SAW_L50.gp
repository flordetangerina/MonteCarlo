# JJA - 13.07.2023
# No terminal: 
# gnuplot SAW_L100.gp
# pdflatex SAW_L100.tex

set term cairolatex pdf standalone blacktext header '\usepackage{amsmath}'
set output 'SAW_L100.tex'

set key box left top height 0.4 spacing 1.4
set xlabel '\Large $t$'
set ylabel '\Large $R^2(t)$'  
set yrange [1:1e4]
#set ytics 0,200,1000
set xrange [1:1000]
set xtics 1,10,10000
set log
set label '\LARGE SAW' at 300,4
#set label '$t^{3/2}$' at 500,400
set pointsize 0.5

plot 'msd_c_W10000.dat' u 1:2 ev 20 t '$\overline{R^2(t)}$' w p pt 5 lc rgb "#AD7FA8",\
     'msd_c_W10000.dat' u ($1<40?$1:1/0):2 t '' w p pt 5 lc rgb "#AD7FA8",\
     ((x>200)&&(x<1000)?0.05*x**1.5:1/0) t '$t^{3/2}$' w l lt 1 lc rgb "#3030FF" lw 4,\
     'msd_c_W1.dat' u 1:2 w l lt 1 lc "black" lw 2 t '$R^2(t)$'
