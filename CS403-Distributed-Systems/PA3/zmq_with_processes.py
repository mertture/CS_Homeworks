from multiprocessing import Process, Value, Array
import os
import math
import time
import zmq
import numpy as np
from abc import ABC, abstractmethod
import re
import sys, getopt
import random

def produce1(iteration_num):
    print ('process id:', os.getpid())
    context = zmq.Context()
    socket = context.socket(zmq.PUSH)
    socket.connect("tcp://127.0.0.1:5558")

    for i in range(iteration_num):
        randomnumber = random.randint(1,1000) 
        work_message = {'num' : randomnumber}
        socket.send_json(work_message)
        print("producer at iteration ", i, randomnumber)


def consume1(iteration_num):
    print ('process id:', os.getpid())
    context = zmq.Context()
    results_receiver = context.socket(zmq.PULL)
    results_receiver.bind("tcp://127.0.0.1:5558")
    
    total = 0
    
    for i in range(iteration_num):
        result = results_receiver.recv_json()
        total += result['num']
        print("collector at iteration ", i, result['num'])
    print("Sum: ", total)

def produce2(iteration_num, random_array):
    print ('process id:', os.getpid())
    for i in range(iteration_num):
        random_array[i] = random.randint(1,1000)
        print("producer at iteration ", i, random_array[i])


def collect2(iteration_num, random_array, total):
    print ('process id:', os.getpid())
    for i in range(iteration_num):
        total.value += random_array[i]
        print("collector at iteration ", i, random_array[i])    

if __name__ == '__main__':
    iter_no = 10
    print("Communication through sockets: ")
    Consumer1 = Process(target=consume1, args=(iter_no,))
    Producer1 = Process(target=produce1, args=(iter_no,))

    Consumer1.start()
    Producer1.start()
            
    Producer1.join()
    Consumer1.join()

    print("main process communicates with its childs through Array and Value: ")
    random_array = Array('i', range(iter_no))
    Sum = Value('i', 0)

    Producer2 = Process(target=produce2, args=(iter_no,random_array,))
    Producer2.start()
    
    Collector2 = Process(target=collect2, args=(iter_no,random_array,Sum,))
    Collector2.start()
    
    Producer2.join()
    Collector2.join()
    print("Sum: ", Sum.value)
    

