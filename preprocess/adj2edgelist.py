import numpy as np

with open('result.txt', 'r') as f:
	lines = f.read().split('\n')

with open('result.edgelist', 'w') as f:
	for i in range(len(lines)):
		for j in range(i+1, len(lines)):
			if lines[i][j] == '1':
				f.write(str(i))
				f.write(' ')
				f.write(str(j))
				if len(lines)-1 != i:
					f.write('\n')


