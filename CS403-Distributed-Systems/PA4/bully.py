import random
import threading
import multiprocessing
import zmq
import sys
import os
import time

# Mert Ture 26800
def leader(aliveNode, isStarter, num_proc, num_starter):

    print("PROCESS STARTS:", os.getpid(), aliveNode, isStarter)
    sharedVar = {
        "respReceived": False,
        "leaderReceived": False,
        "terminateReceived": False,
        "socket": "",
    }
    listenerAndResponder = threading.local()
    listenerAndResponder.thread = None

    listenerAndResponder.thread = threading.Thread(target=responder, args=(aliveNode, num_proc, sharedVar))
    listenerAndResponder.thread.start()

    context = zmq.Context()
    publisher = context.socket(zmq.PUB)
    aliveNodeAddedPort = 5550 + aliveNode
    publisher.bind("tcp://127.0.0.1:{}".format(aliveNodeAddedPort))
    sharedVar["socket"] = publisher

    time.sleep(3)

    isLeaderReceived = False

    timeoutSocket = 11 + (num_starter * num_proc)
    timeoutSocketStart = time.time()

    while ((isLeaderReceived == False) and (time.time() - timeoutSocket < timeoutSocketStart)):

        if (isStarter or sharedVar["leaderReceived"] == True):
            print("PROCESS MULTICASTS LEADER MSG:", aliveNode)

             # Multicast a LEADER message to all nodes with higher IDs.
            for receiverId in range(aliveNode+1, num_proc):
                publisher.send_string("LEADER {} {}".format(aliveNode, receiverId))
                time.sleep(0.05)
            timeoutLeader = 10 + (num_starter * num_proc)
            timeoutStart = time.time()
            
            while (sharedVar['respReceived'] == False):
                currentTime = time.time()
                if(currentTime - timeoutLeader > timeoutStart):
                    break
            
            if (sharedVar['respReceived'] == False):
                print("PROCESS BROADCASTS TERMINATE MSG: {}".format(aliveNode))
                for receiverId in range(num_proc):
                    publisher.send_string("TERMINATE {} {}".format(aliveNode, receiverId))
                    #time.sleep(0.3)
                
            isLeaderReceived = True
        
        
       
    # Wait for the listener thread to terminate.
    listenerAndResponder.thread.join()

def responder(aliveNode, num_proc, sharedVar):
    print("RESPONDER STARTS:", aliveNode)

    # Set up the ZMQ context and subscriber socket.
    context = zmq.Context()

     # Generating subscriber sockets for each process
    subscribers = dict()
    for id in range(num_proc):
            socket = context.socket(zmq.SUB)
            nodeAddedPort = 5550 + id
            socket.connect("tcp://127.0.0.1:{}".format(nodeAddedPort))
            socket.setsockopt(zmq.SUBSCRIBE, b"")
            subscribers[id] = socket


    
    terminate_received = False

    # Run the listener loop.
    while not terminate_received:
        # Wait for a message to be received.
        for id in range(0, num_proc):
            try:
                subscribers[id].RCVTIMEO = 80
                message = subscribers[id].recv_string()
                message_type, sender_id, receiver_id = message.split()
                # If the message is a TERMINATE message, set the terminate flag and notify the main thread.
                if message_type == "TERMINATE" and int(receiver_id) == aliveNode:
                    terminate_received = True
                    sharedVar['terminateReceived'] = True
                
                # If the message is a LEADER message and the sender has a lower ID, respond with a RESP message.
                elif message_type == "LEADER" and int(sender_id) < aliveNode and int(receiver_id) == aliveNode:
                    print("RESPONDER RESPONDS:", aliveNode, sender_id)
                    sharedVar['socket'].send_string("RESP {} {}".format(aliveNode, sender_id))
                    time.sleep(0.05)
                    sharedVar['leaderReceived'] = True

                elif(message_type == "RESP" and int(receiver_id) == aliveNode):
                    sharedVar['leaderReceived'] = True
                    sharedVar['respReceived'] = True
            except:
                pass

        



if __name__ == "__main__"  :

    num_proc = int(sys.argv[1])
    num_alive = int(sys.argv[2])
    num_starter = int(sys.argv[3])

    aliveNodes = random.sample(range(0, num_proc), num_alive)
    starterNodes = random.sample(aliveNodes, num_starter)
                

    print("Alive Nodes:\n",aliveNodes)
    print("Starter Nodes:\n",starterNodes)

    procs = []
    for i in range(0, len(starterNodes)):
        proc = multiprocessing.Process(target=leader, args=(starterNodes[i], True, num_proc, num_starter))
        procs.append(proc)

    for i in range(0, len(aliveNodes)):
        if (aliveNodes[i] not in starterNodes):
            proc = multiprocessing.Process(target=leader, args=(aliveNodes[i], False, num_proc, num_starter))
            procs.append(proc)

    for i in range(0, len(procs)):
        procs[i].start()
    for i in range(0, len(procs)):
        procs[i].join()
