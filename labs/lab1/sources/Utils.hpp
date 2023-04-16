#ifndef UTILS
#define UTILS

class Utils {
public:
    static int higherSizeBeginIndex(int worldSize, int vectorSize) {
        return worldSize - (vectorSize % worldSize);
    }

    static int chunkSize(int localRank, int worldSize, int vectorSize) {
        int chunkSize = (int) (vectorSize / worldSize);
        if (localRank >= higherSizeBeginIndex(worldSize, vectorSize)) {
            chunkSize += 1;
        }
        return chunkSize;
    }

    static int beginIndex(int localRank, int worldSize, int vectorSize) {
        int index = ((int) (vectorSize / worldSize)) * localRank;
        int hsbi = higherSizeBeginIndex(worldSize, vectorSize);
        if (localRank > hsbi) {
            index += localRank - hsbi; 
        }
        return index;
    }

    static int endIndex(int localRank, int worldSize, int vectorSize) {
        return beginIndex(localRank, worldSize, vectorSize) + chunkSize(localRank, worldSize, vectorSize);
    }
};

#endif
