import sys
import numpy as np

L = int(sys.argv[1])
I = int(sys.argv[2])
T = int(sys.argv[3])

transiente = 10
sizes = {50: 0.001, 100: 0.001}
liminf = {'m': -0.200, 'u': -2.010}
limsup = {'m': 1.000, 'u': 0.000}

size = sizes[L]
binsm = round((limsup['m']-liminf['m'])/size)
binsu = round((limsup['u']-liminf['u'])/size)

histm = np.zeros(binsm + 1)
histu = np.zeros(binsu + 1)

m, u = np.loadtxt(f"./dados/T{T}.L{L}.{I}.dat", skiprows = 7 + transiente, usecols = (1, 2), unpack = True)
for i in range(len(m)):
	if(m[i] >= liminf['m'] and m[i] <= limsup['m']):
		indice = int((m[i] - liminf['m'])/size)
		histm[indice] += 1
	if(u[i] >= liminf['u'] and u[i] <= limsup['u']):
		indice = int((u[i] - liminf['u'])/size)
		histu[indice] += 1
			
histm /= (len(m)*size)
histu /= (len(u)*size)

with open(f"./dados/histoMagneT{T}.L{L}.{I}.dat", "w") as f:
	for i in range(len(histm)):
		loc = size*i + liminf['m']
		f.write(f"{loc:.3f} {histm[i]:.3f}\n")

with open(f"./dados/histoEnergT{T}.L{L}.{I}.dat", "w") as f:
	for i in range(len(histu)):
		loc = size*i + liminf['u']
		f.write(f"{loc:.3f} {histu[i]:.3f}\n")

print(np.sum(histm)*size)
print(np.sum(histu)*size)


