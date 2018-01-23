import csv
import matplotlib.pyplot as plt

with open('../botnet_test/capture20110819.binetflow', newline='') as f:
	data = list(csv.reader(f))

bot = []
back_ground = []
for i, line in enumerate(data):
	if line[-1].find('Botnet') != -1:
		bot.append(i)
	if line[-1].find('Background') != -1:
		back_ground.append(i)

print(len(bot))
print(len(back_ground))

for i in bot:
	plt.axvline(x=i)


with open('../botnet_test/botOnly20110819.binetflow', 'w') as f:
	w = csv.writer(f)
	for i in bot:
		w.writerow(data[i])

with open('../botnet_test/backOnly20110819.binetflow', 'w') as f:
	w = csv.writer(f)
	for i in back_ground:
		w.writerow(data[i])
