set title '(Mean) Squared Displacement'

set xlabel 't'
set ylabel 'R²'

set key left top

set border back
plot 'msd_a_N1.dat' pt 7 t '1 amostra', x lc 'black' lw 2 t '', 'msd_a_N1000.dat' pt 7 t '1000 amostras'

