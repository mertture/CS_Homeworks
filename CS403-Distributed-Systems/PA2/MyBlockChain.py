import Pyro4
from threading import Lock
from Pyro4.util import SerializerBase

Pyro4.config.SERIALIZER = "serpent"


class MyBlock:
    def __init__(self, txType, args):
        self.transaction = {"txType": txType, "args": args}
        self.next = None
    
    def getTxType(self):
        return self.transaction["txType"]

    def getArgs(self):
        return self.transaction["args"]

    def getNext(self):
        return self.next

def MyBlock_class_to_dict(obj):
    return {
        "__class__": "MyBlock",
        "transaction": obj.transaction,
        "next": obj.next
    }

def MyBlock_dict_to_class(classname, d):
    return MyBlock(d["transaction"]["txType"], d["transaction"]["args"])

SerializerBase.register_dict_to_class("MyBlock", MyBlock_dict_to_class)
SerializerBase.register_class_to_dict(MyBlock, MyBlock_class_to_dict)


@Pyro4.expose
class MyBlockChain:
    def __init__(self, chainName):
        self.head = None
        self.chainName = chainName

        self.lock = Lock()
        daemon = Pyro4.Daemon()
        nameserver = Pyro4.locateNS()

        uri = daemon.register(self)
        nameserver.register(chainName, uri)
        
        print("Serving...")
        daemon.requestLoop()
    
    def createAccount(self, amount):
        self.lock.acquire()
        maxId = self.getMaxId()
        newAccount = MyBlock("CREATEACCOUNT", tuple((maxId + 1, amount)))
        newAccount.next = self.head
        self.head = newAccount
        self.lock.release()
        return maxId+1

    def transfer(self, from_, to, amount):
        self.lock.acquire()
        if (self.checkAccountExistence(from_) and self.checkAccountExistence(to)):
            if (amount == 0):
                newBlock = MyBlock("TRANSFER", (from_, to, amount))
                newBlock.next = self.head
                self.head = newBlock
                self.lock.release()
                return 1
            elif (amount >= 0):
                fromBalance = self.calculateBalance(from_)
                if (fromBalance >= amount):
                    newBlockFrom = MyBlock("TRANSFER", (from_, to, amount))
                    newBlockFrom.next = self.head
                    self.head = newBlockFrom
                    self.lock.release()
                    return 1
                else:
                    self.lock.release()
                    return -1
            else:
                toBalance = self.calculateBalance(to)
                if (toBalance >= abs(amount)):
                    newBlockFrom = MyBlock("TRANSFER", (from_, to, amount))
                    newBlockFrom.next = self.head
                    self.head = newBlockFrom
                    self.lock.release()
                    return 1
                else:
                    self.lock.release()
                    return -1
        else:
            self.lock.release()
            return -1


    def exchange(self, from_, to, toChain, amount):
        self.lock.acquire()
        toChain.acquireChainLock()
        toChainClass = Pyro4.Proxy(f"PYRONAME:{toChain.getChainName()}")
        if (self.checkAccountExistence(from_) and toChainClass.checkAccountExistence(to)):
            if (amount == 0):
                newBlock = MyBlock("EXCHANGE", (from_, to, toChain.getChainName(), amount))
                newBlock.next = self.head
                self.head = newBlock

                newToChainBlock = MyBlock("EXCHANGE", (to, from_, self.chainName, amount))
                newToChainBlock.next = toChainClass.getHead()
                toChainClass.makeHead(newToChainBlock)

                self.lock.release()
                toChain.releaseChainLock()
                return 1
            elif (amount >= 0):
                fromBalance = self.calculateBalance(from_)
                if (fromBalance >= amount):
                    newBlockFrom = MyBlock("EXCHANGE", (from_, to, toChain.getChainName(), amount))
                    newBlockFrom.next = self.head
                    self.head = newBlockFrom

                    newBlockFromToChain = MyBlock("EXCHANGE", (to, from_, self.chainName, -amount))
                    toChainClass.makeHead(newBlockFromToChain)
                    self.lock.release()
                    toChain.releaseChainLock()

                    return 1
                else:
                    self.lock.release()
                    toChain.releaseChainLock()
                    return -1
            else:
                toBalance = toChainClass.calculateBalance(to)
                if (toBalance >= abs(amount)):
                    newBlockTo = MyBlock("EXCHANGE", (from_, to, toChain.getChainName(), amount))
                    newBlockTo.next = self.head
                    self.head = newBlockTo

                    newBlockToToChain = MyBlock("EXCHANGE", (to, from_, self.chainName, -amount))
                    toChainClass.makeHead(newBlockToToChain)
                    self.lock.release()
                    toChain.releaseChainLock()
                    return 1
                else:
                    self.lock.release()
                    toChain.releaseChainLock()
                    return -1
        else:
            self.lock.release()
            toChain.releaseChainLock()
            return -1
    
    
    def getMaxId(self):
        block = self.head
        maxId = 0
        while (block != None):
            if (block.getTxType() == "CREATEACCOUNT" and block.getArgs()[0] > maxId):
                maxId = block.getArgs()[0]
            block = block.getNext()
        return maxId
    
    def checkAccountExistence(self, accountNumber):
        block = self.head

        while (block != None):
            if (block.getTxType() == "CREATEACCOUNT"):
                if block.getArgs()[0] == accountNumber:
                    return True
            block = block.getNext()
        return False
    
    def calculateBalance(self, accountNumber):
        block = self.head
        accountBalance = 0

        while (block != None):
            accountArgs = block.getArgs()
            accountTxType = block.getTxType()
            if accountTxType == "CREATEACCOUNT":
                if accountArgs[0] == accountNumber:
                    accountBalance += accountArgs[1]

            elif accountTxType == "TRANSFER":
                _from = accountArgs[0]
                to = accountArgs[1]
                transferAmount = accountArgs[2]

                if _from == accountNumber and to != accountNumber:
                    accountBalance -= transferAmount
    
                elif _from != accountNumber and to == accountNumber:
                    accountBalance += transferAmount

            elif accountTxType == "EXCHANGE":
                _from = accountArgs[0]
                to = accountArgs[1]
                toChainName = accountArgs[2]
                transferAmount = accountArgs[3]
                
                if _from == accountNumber and to != accountNumber:
                    accountBalance -= transferAmount
    
                elif _from != accountNumber and to == accountNumber:
                    accountBalance += transferAmount  
                
                elif toChainName != self.chainName and _from == to:
                    accountBalance -= transferAmount
                    
            block = block.getNext()       
        
        return accountBalance
    
    def printChain(self):
        self.lock.acquire()
        block = self.head
        while (block):
            print(block.transaction["txType"], block.transaction["args"])
            block = block.next
        print("\n")
        self.lock.release()


    def getChainName(self):
        return self.chainName
        
    def makeHead(self, newHead):
        newHead.next = self.head
        self.head = newHead
        
    def acquireChainLock(self):
        self.lock.acquire()

    def releaseChainLock(self):
        self.lock.release()
