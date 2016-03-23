import os, random, base64

import numpy as np
from model.lstm import lstm

def folderContents(path):
	return [path+'/'+x for x in os.listdir(path) if not x.startswith('.')]

def main():
	
	samples = 100 # Number of sequences
	seqsize = 1000 # Length of each sequences
	
	l = []
	cats = folderContents('fetch/temp') # Categories
	for cat in cats:                    # For each category
		filelist = folderContents(cat)  # List files within category
		for y in xrange(samples/len(cats)): # Get each sample
			with open(random.choice(filelist)) as f:
				contents = f.read()
				indx = random.randint(0, len(contents)-seqsize)
				samp = contents[indx:indx+seqsize]
				print np.fromstring(samp, dtype=np.float32)
				l.append(samp)
	
	# Train model
	#trainLog = lstm.trainModel(inSeries, targSeries)

	# Run model
	# print("Running Model...")
	# prediction = lstm.runModel(mat2)

if __name__ == '__main__':
	main()
