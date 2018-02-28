import numpy as np

with open('result.txt', 'r') as f:
	lines = f.read().replace(' ', '').split('\n')
	
with open('result.edgelist', 'w') as f:
	for i in range(len(lines)-1):
		for j in range(i+1, len(lines)-1):
			if lines[i][j] == '1':
				f.write(str(i + 1))
				f.write(' ')
				f.write(str(j + 1))
				if len(lines)-1 != i:
					f.write('\n')


