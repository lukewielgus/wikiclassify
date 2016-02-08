import numpy as np
from model.lstm import lstm

def main():
	
	# Convert data to numpy matrices, inSeries & targSeries
	
	# Train model
	trainLog = lstm.trainModel(inSeries, targSeries)

	# Run model
	# print("Running Model...")
	# prediction = lstm.runModel(mat2)

if __name__ == '__main__':
	main()
