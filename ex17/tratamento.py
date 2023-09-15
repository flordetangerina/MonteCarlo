import sys

L = int(sys.argv[1])

with open(f"L{L}.mean", "w") as fwrite:
	
	fwrite.write("T m x\n")

	with open(f"L{L}.dat", "r") as fread:

		lines = fread.readlines()[5:]

		for i in range(101):

			T = 2 + i/100
			ini = 1 + i*52
			fim = 51 + i*52

			data = lines[ini:fim]
		
			m2 = 0
			mabs = 0
			
			for line in data:
				m2 += float(line.split()[1])**2
				mabs += abs(float(line.split()[1]))

			m2 /= 50
			mabs /= 50

			x = L**2*(m2 - mabs**2)/T

			fwrite.write(f"{T:.2f} {mabs:.2f} {x:.2f}\n")


