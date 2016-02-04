import os

for folder in ['obama']:
	for x in os.listdir(folder):
		if x.endswith('.txt'):
			with open('%s/%s' % (folder, x)) as f:
				speech = f.read()