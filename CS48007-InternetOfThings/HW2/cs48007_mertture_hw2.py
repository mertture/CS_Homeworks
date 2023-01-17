# Mert Ture 26800 CS48007 HW2

import numpy as np
import pandas as pd
from sklearn.metrics import confusion_matrix
from random import random

from sklearn import metrics

from sklearn.model_selection import GridSearchCV
from sklearn.tree import DecisionTreeClassifier
from sklearn.neighbors import KNeighborsClassifier

features = list()
with open('UCI HAR Dataset/features.txt') as f:
    features = [line.split()[1] for line in f.readlines()]

X_train = pd.read_csv('UCI HAR Dataset/train/X_train.txt', delim_whitespace=True, header=None)
y_train = pd.read_csv('UCI HAR Dataset/train/y_train.txt', names=['Activity'], squeeze=True)

X_train.columns = [features]
y_train = y_train.map({1: 'WALKING', 2:'WALKING_UPSTAIRS',3:'WALKING_DOWNSTAIRS', 4:'SITTING', 5:'STANDING',6:'LAYING'})

# get the data
X_test = pd.read_csv('UCI HAR Dataset/test/X_test.txt', delim_whitespace=True, header=None)
X_test.columns = [features]

# get y with number to activity
y_test = pd.read_csv('UCI HAR Dataset/test/y_test.txt', names=['Activity'], squeeze=True)

y_test = y_test.map({1: 'WALKING', 2:'WALKING_UPSTAIRS',3:'WALKING_DOWNSTAIRS', 4:'SITTING', 5:'STANDING',6:'LAYING'})

labels = ['LAYING', 'STANDING', 'SITTING', 'WALKING', 'WALKING_DOWNSTAIRS', 'WALKING_UPSTAIRS']

def confusionMatrixToDf(cm, labels):
    df = pd.DataFrame()
    # rows
    for i, row_label in enumerate(labels):
        rowdata={}
        # columns
        for j, col_label in enumerate(labels): 
            rowdata[col_label]=cm[i,j]
        df = df.append(pd.DataFrame.from_dict({row_label:rowdata}, orient='index'))
    return df[labels]

def train(model, X_train, y_train, X_test, y_test, model_name, labels):
    
    print("---", model_name, "---")

    #train
    print('---Training---')
    model.fit(X_train, y_train)
    print('---Done---\n')

    #predict
    print('---Predicting---')
    y_pred = model.predict(X_test)
    print('---Done---\n')
   
    #print accuracy
    accuracy = metrics.accuracy_score(y_true=y_test, y_pred=y_pred)
    print("---", model_name, "ACCURACY ---")
    print('Accuracy:- {}\n'.format(accuracy))
   
    #confusion matrix
    confusionMatrix = metrics.confusion_matrix(y_test, y_pred)
    print('\n---CONFUSION MATRIX---')
    dfConfusionMatrix = confusionMatrixToDf(confusionMatrix, labels)

    print('\n {}'.format(dfConfusionMatrix, "\n"))
        
    #classification report
    print('\n---CLASSIFICATION REPORT---')
    classificationReport = metrics.classification_report(y_test, y_pred)
    print(classificationReport)
    
parameters = {'max_depth':np.arange(3,7,2)}
dt = DecisionTreeClassifier()
dt_grid = GridSearchCV(dt,param_grid=parameters, n_jobs=-1)
train(dt_grid, X_train, y_train, X_test, y_test, "DECISION TREE CLASSIFIER", labels)

params = {"n_neighbors": np.arange(3, 16, 4)}
knn = KNeighborsClassifier()
knn_grid = GridSearchCV(estimator=knn, param_grid=params, n_jobs=-1)
train(knn_grid, X_train, y_train, X_test, y_test, "K NEIGHBORS CLASSIFIER", labels)

class Perceptron:
  def __init__(self, no, learningRate = 0.0001):

    # Set Initial Values
    self.learnc = learningRate
    self.bias = 1
    self.weights = []

    # Compute Random Weights
    for i in range(no+1):
      self.weights.append(random() * 2 - 1)


  # Activate Function
  def activate(self, input):
    sum = 0
    for i in range(len(input)):
      sum += input[i] * self.weights[i]
    if (sum > 0):
      return 1
    else:
      return 0


  # Train Function
  def train(self, inputs, desired, epochs):
    for m in range(epochs):
      for i in range(len(inputs)):
          inputs[i].append(self.bias)
          guess = self.activate(inputs[i])
          error = desired[i] - guess
          if (error != 0):
            for j in range(len(inputs[i])):
              self.weights[j] += self.learnc * error * inputs[i][j]
          inputs[i].pop()

orInputs = [[0, 0], [0, 1], [1, 0], [1, 1]]

orOutputs = [0, 1, 1, 1]

andInputs = [[0, 0], [0, 1], [1, 0], [1,1]]

andOutputs = [0, 0, 0, 1]

nandInputs = [[0, 0], [0, 1], [1, 0], [1, 1]]

nandOutputs = [1, 1, 1, 0]

orPerceptron = Perceptron(2)
orModel = orPerceptron.train(orInputs, orOutputs, 15000)

andPerceptron = Perceptron(2)
andModel = andPerceptron.train(andInputs, andOutputs, 15000)

nandPerceptron = Perceptron(2)
nandModel = nandPerceptron.train(nandInputs, nandOutputs, 15000)

print("--- OR RESULTS ---")
for i in range(len(orInputs)):
  x = orInputs[i][0]
  y = orInputs[i][1]
  guess = orPerceptron.activate([x, y, orPerceptron.bias])
  print("x:", x, "y:", y, "output:", guess)


print("--- AND RESULTS ---")

for i in range(len(andInputs)):
  x = andInputs[i][0]
  y = andInputs[i][1]
  guess = andPerceptron.activate([x, y, andPerceptron.bias])
  print("x:", x, "y:", y, "output:", guess)


print("--- NAND RESULTS ---")

for i in range(len(nandInputs)):
  x = nandInputs[i][0]
  y = nandInputs[i][1]
  guess = nandPerceptron.activate([x, y, nandPerceptron.bias])
  print("x:", x, "y:", y, "output:", guess)


def XORPrediction(orPerceptron, andPerceptron, nandPerceptron, x, y):
  orResult = orPerceptron.activate([x, y, orPerceptron.bias])
  nandResult = nandPerceptron.activate([x, y, nandPerceptron.bias])
  return andPerceptron.activate([orResult, nandResult, andPerceptron.bias])


XORInputs = np.array([[0,0],[0,1],[1,0],[1,1]])
XOROutputs = np.array([0,1,1,0])
print("---COMBINED XOR RESULTS---")
for i in range(len(XORInputs)):
  x = XORInputs[i][0]
  y = XORInputs[i][1]
  XORresult = XORPrediction(orPerceptron, andPerceptron, nandPerceptron, x, y)
  print("x:", x, "y:", y, "output:", XORresult)

def sigmoid (x):
    return 1/(1 + np.exp(-x))

def derivativeSigmoid(x):
    return x * (1 - x)

XORInputs = np.array([[0,0],[0,1],[1,0],[1,1]])
XOROutputs = np.array([[0],[1],[1],[0]])

inputLayerNeuronCount = 2 
hiddenLayerNeuronCount = 2
outputLayerNeuronCount = 1

#generate random weight for the first epoch
hiddenLayerWeights = []
for i in range(inputLayerNeuronCount):
  hiddenLayerWeightArray = []
  for j in range(hiddenLayerNeuronCount):
    hiddenLayerWeightArray.append(random())
  
  hiddenLayerWeights.append(hiddenLayerWeightArray)

hiddenLayerBias = []
for i in range(1):
  hiddenLayerBiasElement = []
  for j in range(hiddenLayerNeuronCount):
    hiddenLayerBiasElement.append(random())
  
  hiddenLayerBias.append(hiddenLayerBiasElement)

outputWeights = []
for i in range(hiddenLayerNeuronCount):
  outputWeightsElement = []
  for j in range(outputLayerNeuronCount):
    outputWeightsElement.append(random())
  
  outputWeights.append(outputWeightsElement)

outputBias = []
for i in range(1):
  outputBiasElement = []
  for j in range(outputLayerNeuronCount):
    outputBiasElement.append(random())
  
  outputBias.append(outputBiasElement)

#convert array to np arrays
hiddenLayerWeights = np.array(hiddenLayerWeights)

hiddenLayerBias = np.array(hiddenLayerBias)

outputWeights = np.array(outputWeights)

outputBias = np.array(outputBias)

learningRate = 0.1

#epochs
for i in range(50000):

	#run the model and determine the values of each node in the network
	hiddenLayerActivation = np.dot(XORInputs, hiddenLayerWeights)
	hiddenLayerActivation += hiddenLayerBias
	hiddenLayerOutput = sigmoid(hiddenLayerActivation)

	outputLayerActivation = np.dot(hiddenLayerOutput,outputWeights) + outputBias
	guess = sigmoid(outputLayerActivation)

	#Calculate the gradients
  #backpropagation
	error = XOROutputs - guess
	derivateGuess = error * derivativeSigmoid(guess)
	
	errorHiddenL = derivateGuess.dot(outputWeights.T)

	derivativeHiddenL = errorHiddenL * derivativeSigmoid(hiddenLayerOutput)
  #Update weights and biases starting for output to the input
	hiddenLayerWeights += XORInputs.T.dot(derivativeHiddenL) * learningRate
	hiddenLayerBias += np.sum(derivativeHiddenL,axis=0,keepdims=True) * learningRate

	outputWeights += hiddenLayerOutput.T.dot(derivateGuess) * learningRate
	outputBias += np.sum(derivateGuess,axis=0,keepdims=True) * learningRate


print("\nTrained XOR Neural Network:")

for i in range(len(XORInputs)):
  x = XORInputs[i][0]
  y = XORInputs[i][1]
  hiddenLayerActivation = np.dot(XORInputs[i], hiddenLayerWeights) + hiddenLayerBias
  hiddenLayerOutput = sigmoid(hiddenLayerActivation)

  outputLayerActivation = np.dot(hiddenLayerOutput,outputWeights) + outputBias
  guess = sigmoid(outputLayerActivation)
  if (guess >= 0.5):
      print("x:", x, "y:", y, "output:", 1)
  else:
      print("x:", x, "y:", y, "output:", 0)


