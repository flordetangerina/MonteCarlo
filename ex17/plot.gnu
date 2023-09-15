set title 'Susceptibilidade'

set xlabel 'T'
set ylabel '(<m²> - <|m|²>) * N/T'

plot 'L25.mean' u 1:3 pt 7 t 'L25',\
	 'L50.mean' u 1:3 pt 7 t 'L50',\
	 'L75.mean' u 1:3 pt 7 t 'L75',\
	 'L100.mean' u 1:3 pt 7 t 'L100'

