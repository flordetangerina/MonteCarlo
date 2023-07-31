# JJA - 18.07.2023
# No terminal: 
# gnuplot T_p.gnu
# pdflatex T_p.tex

set term cairolatex pdf standalone blacktext header '\usepackage{amsmath}'
set output 'T_p.tex'
set key box left top samplen -1 width 2.5 height 0.3 spacing 1.4
set xlabel '\Large $p$'
set ylabel '\Large $T(p,L)$'  
set yrange [0:150]
set ytics 0,50,150
set xrange [0.3:0.9]
set xtics 0,0.1,1
set pointsize 0.7
set arrow from 0.5927, graph 0 to 0.5927, graph 1 nohead dt '-' lc "black"

plot 'tempo_L50.dat' u 1:2 t '$L=50$' w lp lt 1 lw 3 pt 5,\
     'tempo_L100.dat' u 1:2 t '100' w lp lt 2 lw 3 pt 7,\
     'tempo_L150.dat' u 1:2 t '150' w lp lt 3 lw 3 pt 9
