import matplotlib.pyplot as plt
import numpy as np
import imageio

L = 20

time, x, y, s = np.loadtxt('trajetoria_c.dat', unpack = True)

xs = (s % L) + 0.5; 
ys = (L - s//L) - 0.5;

cross = []
for i in range(len(time) - 1):
	displacement = abs(xs[i + 1] - xs[i]) + abs(ys[i + 1] - ys[i])
	if(displacement > 1):
		cross.append(time[i + 1])
cross = np.array(cross)

time = time.astype(int)
cross = cross.astype(int)

xsplit = np.split(xs, cross)
ysplit = np.split(ys, cross)
tsplit = np.split(time, cross)

def create_frame(t):
	fig = plt.figure(figsize = (6, 6))
	for i in range(len(xsplit)):
		if (tsplit[i][-1] <= t):
			plt.plot(xsplit[i], ysplit[i], color = 'grey')
		elif (tsplit[i][0] <= t):
			imax = np.where(tsplit[i] == t)[0][0] + 1
			plt.plot(xsplit[i][:imax], ysplit[i][:imax], color = 'grey')
	plt.plot(xs[t], ys[t], color = 'orange', marker = 'o')
	plt.xlim([0, L])
	plt.ylim([0, L])
	plt.title(f'step {t}', fontsize = 12)
	plt.savefig(f'./img/img_{t}.png', transparent = False, facecolor = 'white')
	plt.close()

for t in time:
    create_frame(t)

frames = []

for t in time:
    image = imageio.v2.imread(f'./img/img_{t}.png')
    frames.append(image)

imageio.mimsave('./example.gif', frames, duration = 0.005)
