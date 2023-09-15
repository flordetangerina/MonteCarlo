import sys
import numpy as np

L = int(sys.argv[1])
T = float(sys.argv[2])

lista = np.loadtxt(f"L{L}-T{T:.2f}.lis", dtype = 'str')

for arquivo in lista:
	t, ct = np.loadtxt(f"./samples/L{L}/T{T:.2f}/{arquivo}", skiprows = 7, max_rows = 100, usecols = (0, 3), unpack = True)
	r, cr = np.loadtxt(f"./samples/L{L}/T{T:.2f}/{arquivo}", skiprows = 1009, unpack = True)
	if(np.where(lista == arquivo)[0][0] == 0):
		ctmean = ct
		crmean = cr
	else:
		ctmean += ct
		crmean += cr

ctmean /= len(lista)
crmean /= len(lista)

with open(f"correlacaoTemporal-L{L}T{T:.2f}.dat", "w") as f:
	for i in range(len(t)):
		f.write(f"{t[i]:.0f} {ctmean[i]:.3f}\n")

with open(f"correlacaoEspacial-L{L}T{T:.2f}.dat", "w") as f:
	for i in range(len(r)):
		f.write(f"{r[i]:.0f} {crmean[i]:.3f}\n")

