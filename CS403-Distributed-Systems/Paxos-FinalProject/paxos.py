import sys
import random
from multiprocessing import Process, Barrier
import zmq
import time

#Mert Ture 26800
# send with failing chance to all
def broadcastFailure(message, proposer, N, prob):
    for i in range(N):
        sendFailure(message, proposer, i, prob)

# calculate failing chance and send
def sendFailure(message, proposer, target, prob):
    randomProb = random.uniform(0,1)
    if (randomProb > prob):
        pushes[target].send_json({"sender": proposer, "receiver": target, "message": message})
    else:
        pushes[target].send_json({"sender": proposer, "receiver": target, "message": ("CRASH {}".format(proposer))})

def getMaxVotedRound(element):
    return element['maxVotedRound']

def PaxosNode(barrier, id, prob, numProcs, bin, numRounds):
    maxVotedRound = -1
    maxVotedVal = None
    proposeVal = None
    decision = None
    proposeSent = False
    #pull socket
    context = zmq.Context()
    global pushes
    global pullSockets
    pullSockets = context.socket(zmq.PULL)
    port = 5550 + id
    pullSockets.bind("tcp://127.0.0.1:{}".format(port))
    pushes = []
    #push sockets
    for i in range(numProcs):
        pushSocket = context.socket(zmq.PUSH)
        port = 5550 + i
        pushSocket.connect("tcp://127.0.0.1:{}".format(port))
        pushes.append(pushSocket)
    # 10 seconds timeout is fine
    timeout = 10
    timestart= time.time()
    for round in range(numRounds):
        role = "ACCEPTOR"
        # round number is leader
        if id == (round % numProcs):
            role = "PROPOSER"
        if (role == "PROPOSER"):
            print("ROUND {} STARTED WITH INITIAL VALUE {}".format(round, bin))
            # start for all nodes
            broadcastFailure("START", (round % numProcs), numProcs, prob)
            # wait for message receive
            time.sleep(1)
            receivedMessages = 0
            joinMessages = []
            numOfSameMessages = 0
            while(receivedMessages != numProcs):
                try:
                    pullSockets.RCVTIMEO = 500
                    message = pullSockets.recv_json()
                    print("LEADER OF {} RECEIVED IN JOIN PHASE: {}".format(round, message['message']))
                    rootMessage = (message['message']).split()   
                    receivedMessages += 1
                    if (rootMessage[0] == "JOIN"):
                        numOfSameMessages += 1
                        sendingMaxVotedRound = int(rootMessage[1])
                        sendingMaxVotedValue = rootMessage[2]
                        # gonna calculate max voted round
                        joinMessages.append({"sender": message['sender'], "maxVotedRound": sendingMaxVotedRound, "maxVotedVal": sendingMaxVotedValue})
                    elif (rootMessage[0] == "START"):
                        numOfSameMessages += 1                                 
                except Exception as e:
                    #print("79 Error:", e)
                    # in any error timeout
                    deadline = timestart + timeout
                    if(time.time() > deadline):
                        break
                    pass
            # passed quorum (N/2)
            if (numOfSameMessages > (numProcs / 2)):
                highestVotedRoundMessage= max(joinMessages, key=getMaxVotedRound)
                if highestVotedRoundMessage['maxVotedVal'] != "None":
                    proposeVal = int(highestVotedRoundMessage['maxVotedVal'])
                else:
                    proposeVal = None
                # if no voted round before, put value
                if (highestVotedRoundMessage['maxVotedRound'] == -1):
                    proposeVal = bin
                currId = round % numProcs
                # propose for proposer with propose val
                broadcastFailure("PROPOSE {}".format(proposeVal), currId, numProcs, prob)
                #propose is sent previously by n
                proposeSent = True
            else:
                currId = round % numProcs
                # send function
                for i in range(numProcs):  
                    if (i != currId):  
                        pushes[i].send_json({"sender": currId, "receiver": i, "message": "ROUNDCHANGE"})
                print("LEADER OF ROUND {} CHANGED ROUND".format(round))
                try:
                    # barrier for phase ending
                    barrier.wait()
                except Exception as e:
                    #print("106 Error:", e)
                    return
                continue
            #get votes etc. if propose sent as proposer
            if (proposeSent):
                leftMessages = numProcs
                voteOrProposeCounter = 0
                while(leftMessages != 0):
                    try:
                        pullSockets.RCVTIMEO = 500
                        message = pullSockets.recv_json()
                        leftMessages -= 1
                        print("LEADER OF {} RECEIVED IN VOTE PHASE: {}".format(round, message['message']))
                        rootMessage = (message['message']).split()
                        if (rootMessage[0] == "VOTE"):
                            voteOrProposeCounter += 1
                        # own propose message
                        elif (rootMessage[0] == "PROPOSE" and message['sender'] == id and message['receiver'] == id):
                            voteOrProposeCounter += 1
                            maxVotedVal = proposeVal
                            maxVotedRound = round
                        elif (rootMessage[0] == "PROPOSE"):
                            voteOrProposeCounter += 1
                    except Exception as e:
                        #print("133 Error:", e)
                        deadline = timestart + timeout
                        if(time.time() > deadline):
                            break
                        pass
                # decide the round result
                if (voteOrProposeCounter > (numProcs / 2)):
                    decision = proposeVal
                    print("LEADER OF {} DECIDED ON VALUE: {}".format(round, decision))
                    try:
                        barrier.wait()
                    except Exception as e:
                       #print("143 Error:", e)
                        return
        elif (role == "ACCEPTOR"):
            #PHASE 1
            phase1ReceivedMessages = 0
            currId = round % numProcs
            # get start and join as acceptor
            while (phase1ReceivedMessages == 0):
                try:
                    pullSockets.RCVTIMEO = 500
                    message = pullSockets.recv_json()
                    phase1ReceivedMessages += 1
                    rootMessage = (message['message']).split()
                    currId = round % numProcs
                    print("ACCEPTOR {} RECEIVED IN JOIN PHASE: {}".format(id, message['message']))
                    # join
                    if (rootMessage[0] == "START"):
                        sendFailure("JOIN {} {}".format(maxVotedRound, maxVotedVal), currId, currId, prob)
                    # upsie
                    elif (rootMessage[0] == "CRASH"):
                        pushes[currId].send_json({"sender": currId, "receiver": currId, "message": "CRASH {}".format(currId)})
                except Exception as e:
                    #print("163 Error:", e)
                    deadline = timestart + timeout
                    if(time.time() > deadline):
                        break
                    pass
            #PHASE 2
            phase2ReceivedMessages = 0
            currId = round % numProcs
            # voting phase for acceptor
            while (phase2ReceivedMessages == 0):
                try:
                    pullSockets.RCVTIMEO = 500
                    message = pullSockets.recv_json()
                    print("ACCEPTOR {} RECEIVED IN VOTE PHASE: {}".format(id, message['message']))
                    phase2ReceivedMessages += 1
                    rootMessage = (message['message']).split()
                    # upsie
                    if (rootMessage[0] == "CRASH"):
                        pushes[currId].send_json({"sender": currId, "receiver": currId, "message": "CRASH {}".format(currId)})
                    # no quorum, change round
                    elif (rootMessage[0] == "ROUNDCHANGE"):
                        try:
                            barrier.wait()
                        except Exception as e:
                            #print("185 Error:", e)
                            return
                        continue
                    # propose is there, start voting and update maxVotedValue and maxVotedRound
                    elif (rootMessage[0] == "PROPOSE"):
                        if (rootMessage[1] != None):
                            maxVotedVal = int(rootMessage[1])
                        else:
                            maxVotedVal = None
                        maxVotedRound = round
                        sendFailure("VOTE", currId, currId, prob)
                        try:
                            barrier.wait()
                        except Exception as e:
                            #print("198 Error:", e)
                            return
                        continue
                except Exception as e:
                    #print("Error:", e)
                    deadline = timestart + timeout
                    if(time.time() > deadline):
                        break
                    pass

if __name__ == '__main__':
    args = sys.argv

    numProcs = int(args[1])
    prob = float(args[2])
    numRounds = int(args[3])
    print("NUMNODES: {}, CRASHPROB: {}, NUMROUNDS: {}".format(numProcs, prob, numRounds))
    bin = random.randint(0,1)
    # barrier for waiting for the phase
    barrier = Barrier(numProcs, timeout=10)
    processes = []
    for i in range(numProcs):
        processes.append(Process(target=PaxosNode, args=(barrier, i, prob, numProcs, bin, numRounds)))
    for i in range(len(processes)):
        processes[i].start()
    for i in range (len(processes)):
        processes[i].join()