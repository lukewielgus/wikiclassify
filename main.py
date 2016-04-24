#!/usr/bin/env python
# -*- coding: utf-8 -*-
from __future__ import print_function

# Standard Libraries
import os

# Local Libraries
from classifier import classifier
from data import data

###############################################################################

def main():

	data_folder = 'data/sources/wikipedia'
	models_folder = 'classifier/models'
	save_loc = 'site/wikiclassify/wiki'

	if not os.path.exists(data_folder):
		os.makedirs(data_folder)

	if next(os.walk(data_folder))[1]:
		
		retrain = False
		if retrain:
			input, target, classes = data.sample(data_folder)
			model = classifier.build(input.shape, target.shape)
			classifier.train(model, input, target)
			classifier.save(models_folder, model, classes)

		else:
			model, classes = classifier.load(models_folder, sorted(os.listdir(models_folder))[-1])
		
		for root, dirs, files in os.walk(data_folder):
			for file in files:
				if not file.startswith('.'):
					with open(root+'/'+file) as f:
						input = data.str2mat(f.read())
						output = classifier.run(model, input)
						data.backtest(save_loc, classes, input, output)
	else:
		print("""\nNo data found.\nPut subfolders of files by class, within the 'data' folder.""")

if __name__ == "__main__":
	main()