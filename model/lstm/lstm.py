from keras.models import Sequential  
from keras.layers.core import TimeDistributedDense, Activation, Dropout  
from keras.layers.recurrent import GRU
from keras.callbacks import EarlyStopping
from keras.models import model_from_json

import numpy as np
np.random.seed(1)

import matplotlib.pyplot as plt

import theano
theano.config.mode = 'FAST_RUN'
theano.config.floatX = 'float32'

#
# Model I/O
#

# Save model
def putModel(model):
	open('model_meta.json', 'w').write(model.to_json())
	model.save_weights('model_data.h5', overwrite=True)
	return None

# Load model
def getModel():
	model = model_from_json(open('model_meta.json').read())
	model.load_weights('model_data.h5')
	return model

# Use model
def runModel(inMatrix, model = None):
	if model is None:
		model = getModel()

	return model.predict(inMatrix)

# Compile model, given IO topology
def compileModel(inSize, outSize):

	model = Sequential() 
	model.add(GRU(outSize/3, input_dim=inSize, return_sequences=True))
	model.add(Dropout(0.2))
	model.add(TimeDistributedDense(outSize))  
	model.add(Activation("linear"))  
	model.compile(loss="mean_squared_error", optimizer="rmsprop")
	
	return model

#
# Train model
#

def trainModel(inMatrix, targMatrix):
	
	# Compile model
	print("Compiling Model...")
	model = compileModel(inMatrix.shape[2], targMatrix.shape[2])
	
	# Train model
	print("Training Model...")
	early_stopping = EarlyStopping(monitor='val_loss', patience=3)
	trainer = model.fit(inMatrix, targMatrix, batch_size=3, validation_split=0.15, callbacks=[early_stopping], verbose=1)
	
	# Save model
	print("Saving Model...")
	putModel(model)

	return trainer.history