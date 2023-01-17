from MapReduce import MapReduce


class FindMax(MapReduce):
    def Map(self, parts):
        return {max([i[0] for i in parts]) : 1}

    def Reduce(self, kvs):
        if kvs is None:
            return None
        curMax = list(kvs[0])[0]
        for i in range(1, len(kvs)):
            elt = list(kvs[i])[0]
            if curMax < elt :
                curMax = elt
        return curMax

if __name__ == '__main__':
    mr = FindMax(4)
    print("Find max called")
    mr.start("sample_01.txt")