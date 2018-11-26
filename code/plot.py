import sys
from matplotlib import pyplot as plt
import numpy as np

def running_mean(x, N):
	x = [float(i) for i in x]
	cumsum = np.cumsum(np.insert(x, 0, 0)) 
	return list((cumsum[N:] - cumsum[:-N]) / float(N))

def plot(data, fname):
	print(len(data['S']))
	plt.rcParams["figure.figsize"] = [16,3]
	for label in data:
		x = [int(i[0]) for i in data[label]]
		y = [int(i[1]) for i in data[label]]
		y = running_mean(y, 1)
		y.extend([200] * (len(x) - len(y)))
		# plt.plot(x, y, label=label, alpha=1.0)
		fft = np.fft.fft(y)
		# fft = [fft[i] if i < len(y) / 1.5 else 0 for i in range(len(y))]
		# plt.plot(x, fft)
		plt.plot(x, list(map(abs, np.fft.ifft(fft))), label=label, alpha=1.0)

	plt.legend(loc='best')
	plt.xlabel('Time')
	plt.ylabel('Latency')
	plt.show()
	# print("Saved to " + fname)


if __name__ == '__main__':

	data = {}
	start = False
	counter = 1
	for line in sys.stdin:
		line = line.rstrip("\n")
		if (line == "END"):
			plot(data, str(counter) + '.png')
			counter += 1
			data = {}
			start = False
		elif (line == "START"):
			start = True
		elif start:
			label, time, probe_time = line.split(",")
			if (label not in data.keys()):
				data[label] = []
			data[label].append((time, probe_time))
