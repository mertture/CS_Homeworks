import threading 
import sys
class Node:
    def __init__(self, value, next_node=None, key=None):
        self.value = value
        self.next = next_node
        self.key = key

class ConSet:
    def __init__(self, values=None):
        self.head = Node(-sys.maxsize - 1)
        self.head.next = Node(sys.maxsize)
        self.cv = threading.Condition()
        self.lock = threading.Lock()
    def add(self, item):
        key = hash(item[0])
        self.cv.acquire()
        try:
            pred = self.head
            curr = pred.next
            while (curr.key != None and (curr.key > key)):
                pred = curr
                curr = curr.next
            
            node = Node(value=item, key=key)
            node.next = curr
            pred.next = node
            self.cv.notify()

            return None
        finally:
            self.cv.release()

    def remove(self):

        self.lock.acquire()
        self.cv.acquire()
        try:
            pred = self.head
            curr = pred.next
            if (curr == None or curr.value == sys.maxsize or curr.value == (-sys.maxsize - 1)):
                self.cv.wait()
            pred = self.head
            curr = pred.next
            if (curr.key != None):
                pred.next = curr.next
                return curr.value
            else:
                return False
        finally:
            self.cv.release()
            self.lock.release()









