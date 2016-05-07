#!/usr/bin/env python
# -*- coding: utf-8 -*-
from __future__ import print_function

# Standard Libraries
import random, os, re
random.seed(1)

# Third-party Libraries
import numpy as np
np.random.seed(1)

###############################################################################

def backtest(save_loc, classes, input, output):

	for sequence in xrange(input.shape[0]):
		fname = save_loc.split('/')[-1][:-4]
		name = fname.replace('_', ' ')
		save_loc = '/'.join(save_loc.split('/')[:-1])
		cats = '\n'.join(['\t\t\t\t\t<option>'+x+'</option>' for x in classes])
		doc = """
		<!DOCTYPE html>
		<html>
			<head>
				<link rel="stylesheet" href="../styles/main.css">
				<title>%s</title>
				<script src="../js/textfill.js"></script>
				<script src="%s_meta.js"></script>
			</head>
			<body onload="highlight();">
				<div id="header">
					<a href="../index.html">
						<img id="logo" class="center" src="../images/logo.svg" alt="WikiClassify" width="250px"/>
					</a>
					<input type="text" name="search" placeholder="Search" id="search" class="center" >
				</div>
				<div id="content" class="center box">
					<h1>%s</h1>
					Select a category:
					<select id="cat_select" onchange="highlight();">
					%s
					</select><br/><br/>
					<a href="https://en.wikipedia.org/wiki/%s">View on Wikipedia</a>
					<p id="article_text">
					%s
					</p>
				</div>
				<div id="footer">
					<nav>
		  				<a href="../about.html">About</a>
		  				<a href="../featured.html"><img height=12px; src="../images/star.svg"> Featured</a>
						<a href="../login.html">Login</a>
					</nav>
				</div>
			</body>
		</html>""" % (name, fname, name, cats, fname, mat2str(input[sequence]))
		form = '['+','.join(['%0.2f' for x in xrange(output.shape[-1])])+'],'
		last_elem = '['+','.join(['0.0' for x in xrange(output.shape[-1])])+']'
		np.savetxt(save_loc + '/%s_meta.js' % fname, output[sequence],fmt=form,delimiter=',',header='var colors=[',footer=last_elem+']',comments='')
		with open(save_loc + '/%s.html' % fname, 'w+') as f:
			f.write(doc)

# Given a file string 's',
# sample and output a numpy matrix with shape (1, len(s), 256)
def str2mat(s):
	smat = np.fromstring(s, dtype=np.dtype('uint8'))
	result = np.zeros((1, smat.size, 256))
	result[0, np.arange(smat.size), smat] = 1
	return result

def mat2str(smat):
	return (np.where(smat)[-1]).tostring().replace('\x00','')

def clean(dname):

	print('Cleaning %s...' % dname)
	for root, dirs, files in os.walk(dname):
		for file in files:
			fname = root+'/'+file
			with open(fname, 'r') as f:
				text = re.sub('\s+', ' ', f.read())
			with open(fname, 'w') as f:
				f.write(text)

# Sample a directory and all subdirectories

def sample(dname, window=300, size=30000):

	print('Sampling...')
	ncat = {dname:size}  # Samples per category, based on directory tree
	nfile = []           # Samples per file, based on relative filesize
	classes = []         # Named classes, based on folder names
	for root, dirs, files in os.walk(dname):
		for d in dirs:
			ncat[root+'/'+d] = ncat[root]/len(dirs)
		if root.count('/') > dname.count('/'):
			classes.append(root.split('/')[-1])

		# Remove hidden and overly small files
		files = filter(lambda x: not x.startswith('.'), files)
		files = filter(lambda x: os.path.getsize(root+'/'+x)>window, files)

		# Calculate size of category
		catsize = sum([os.path.getsize(root+'/'+file) for file in files])
		for file in files:
			fpath = root+'/'+file
			ntimes = int(round(ncat[root]*os.path.getsize(fpath)/float(catsize)))
			if ntimes:
				nfile.append((fpath, ntimes))

	nsamples = sum(x[1] for x in nfile)
	inMatrix = np.empty((nsamples, window, 256), dtype=np.dtype('float32'))
	targMatrix = np.empty((nsamples, window, len(classes)), dtype=np.dtype('float32'))
	
	i = 0
	shuff = range(nsamples)
	random.shuffle(shuff)
	for file in nfile:
		target = np.tile([1 if x in file[0].split('/') else 0 for x in classes], (1, window, 1))
		for _ in xrange(file[1]):
			if not i % 1000:
				print('%0.2f%% Done' % (100.0*i/nsamples))
			with open(file[0]) as f:
				f.seek(random.randint(0, os.path.getsize(file[0])-window))
				inMatrix[shuff[i]] = str2mat(f.read(window))
				targMatrix[shuff[i]] = target
				i += 1

 	return (inMatrix, targMatrix, classes)