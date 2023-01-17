from MapReduce import MapReduce
from threading import Lock

class FindCitations(MapReduce): 
 
    def Map(self, map_input):
        bucketDict = {}
        for i in range (len(map_input)):
            if (map_input[i][1] in bucketDict):
                bucketDict[map_input[i][1]] += 1
            else:
                bucketDict[map_input[i][1]] = 1 

        return bucketDict

    def Reduce(self, reduce_input):
        reducedDict = {}

        for i in range(len(reduce_input)):
            for j in reduce_input[i].keys():
                if (j not in reducedDict):
                    reducedDict[j] = reduce_input[i][j]
                else:
                    reducedDict[j] += reduce_input[i][j]
            
        return reducedDict

