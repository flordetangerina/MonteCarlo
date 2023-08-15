import sys
import numpy as np

L = int(sys.argv[1])
lista = np.loadtxt(f"L{L}.lis", dtype = 'str')

sizes = {5: 0.25, 10: 0.2, 50: 0.01, 100: 0.005}

lim = 0.5
size = sizes[L]
bins = round(2*lim/size)

histm = np.zeros(bins)
histu = np.zeros(bins)

for arquivo in lista:
	m, u = np.loadtxt(f"./samples/L{L}/{arquivo}", skiprows = 6, unpack = True)
	if(abs(m) < lim):
		indice = int((m + lim)/size)
		histm[indice] += 1
	if(abs(u) < lim):
		indice = int((u + lim)/size)
		histu[indice] += 1
		
histm /= (len(lista)*size)
histu /= (len(lista)*size)

with open(f"histogramaL{L}.dat", "w") as f:
	f.write("#bin m u\n")
	for i in range(len(histm)):
		loc = i*size - lim + size/2
		f.write(f"{loc:.3f} {histm[i]:.3f} {histu[i]:.3f}\n")

print(np.sum(histm))
print(np.sum(histu))


