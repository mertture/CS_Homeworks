from ConSet import ConSet
import threading
from random import randint
n = 4

lock = threading.Lock()
global mailboxes
mailboxes = []

for i in range(n):
    mailbox = ConSet()
    mailboxes.append(mailbox)

def nodeWork(idx, n):

    roundContinue = False    
    round = 0

    while (roundContinue == False):
        round += 1
        vote = randint(0, n**2)
        voteTuple = (vote, idx)
        for i in range(n):
            mailboxes[i].add(voteTuple)
        print("Node", idx, "proposes value", vote, "for round", round)
        barrier.wait()
        barrier.reset()

        if (n == 1):
            print("Node", idx, "decided", idx, "as the leader.")
            roundContinue = True
        else:
            returnVote1 = mailboxes[idx].remove()
            returnVote2 = mailboxes[idx].remove()

            if (returnVote1[0] > returnVote2[0]):
                print("Node", idx, "decided", returnVote1[1], "as the leader.")
                roundContinue = True
                barrier_2.wait()

            else:
                print("Node", idx, "could not decide on the leader and moves to the round", round+1, ".")
                for i in range(n-2):
                    mailboxes[idx].remove()
                roundContinue = False
                barrier_2.wait()
                barrier_2.reset()


global barrier, barrier_2
barrier = threading.Barrier(n)
barrier_2 = threading.Barrier(n)
for i in range(n):
    threading.Thread(target=nodeWork, args=(i, n)).start()    

