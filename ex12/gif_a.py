import matplotlib.pyplot as plt
import numpy as np
import imageio

time, x, y = np.loadtxt('trajetoria_a.dat', unpack = True)
time = time.astype(int)

L = np.stack((np.absolute(x), np.absolute(y))).max()

def create_frame(t):
	fig = plt.figure(figsize = (6, 6))
	plt.plot(x[:t+1], y[:t+1], color = 'grey')
	plt.plot(x[t], y[t], color = 'orange', marker = 'o')
	plt.xlim([-L, L])
	plt.ylim([-L, L])
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

