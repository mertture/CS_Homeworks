from abc import ABCMeta, abstractmethod
from multiprocessing import Process, Value, Array
import os
import zmq
import json
import time


class MapReduce(metaclass=ABCMeta):
    def __init__(self, num_worker):
        self.num_worker = num_worker
        
    def start(self, filename):
        data = []
        with open(filename) as file:
            for i, line in enumerate(file):
                elts = line.rstrip().split("\t")
                data.append([int(i) for i in elts])

        producer = Process(target=self._Producer, args=(data,))
        consumerList = []
        for i in range(self.num_worker):
            consumer = Process(target=self._Consumer)
            consumerList.append(consumer)

        for consumer in consumerList:
            consumer.start()
        

        producer.start()
       
       
        producer.join()

        resultCollector = Process(target=self._ResultCollector)
        resultCollector.start() 

        for consumer in consumerList:
            consumer.join()

        resultCollector.join()
        return


    
    def _Producer(self, producer_input):

        print ('Producer process id:', os.getpid())
        context = zmq.Context()
        socket = context.socket(zmq.PUSH)
        socket.bind("tcp://127.0.0.1:5556")
        
        bucketSize = len(producer_input) // self.num_worker
        counter = 0
        if (bucketSize * self.num_worker != len(producer_input)):
            counter = len(producer_input) - (bucketSize * self.num_worker)
        bucketCounter = bucketSize
        for i in range(self.num_worker):
            # TEST CAREFULLY FOR [:BUCKETCOUNTER+1]
            # MAYBE OUT OF RANGE ERROR
            time.sleep(0.05)

            if (counter != 0):

                bucket = producer_input[(bucketCounter - bucketSize):bucketCounter+1]
                socket.send_json(bucket)

                bucketCounter += bucketSize + 1
                counter -= 1
            else:
                bucket = producer_input[(bucketCounter - bucketSize):bucketCounter]
                socket.send_json(bucket)

                bucketCounter += bucketSize

    def _Consumer(self):
        print ('Consumer PULL process id:', os.getpid())

        context = zmq.Context()
        results_receiver = context.socket(zmq.PULL)
        results_receiver.connect("tcp://127.0.0.1:5556")

        socket = context.socket(zmq.PUSH)
        socket.connect("tcp://127.0.0.1:5558")



        bucket = results_receiver.recv_json()
        mappedBucket = self.Map(bucket)
        socket.send_json(mappedBucket)
        


    def _ResultCollector(self):
        print ('Result PULL process id:', os.getpid())
        context = zmq.Context()
        results_receiver = context.socket(zmq.PULL)
        results_receiver.bind("tcp://127.0.0.1:5558")

        buckets = []
        for i in range(self.num_worker):
            bucket = results_receiver.recv_json()
            buckets.append(bucket)
       
        reducedBuckets = self.Reduce(buckets)
        print("reduced", reducedBuckets)
        print("open file")
        with open('results.txt', 'w') as results_file:
            results_file.write(str(reducedBuckets))
            results_file.close()
            return

    
    @abstractmethod
    def Map(self, map_input):
        pass

    @abstractmethod
    def Reduce(self, reduce_input):
        pass