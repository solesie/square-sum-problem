# Numberphile's Square-Sum Problem
25-fomula approach with lower size

## Link 
- [youtube](https://www.youtube.com/watch?v=-vxW42R47bc)  
- [mersenneforum](https://www.mersenneforum.org/showthread.php?s=5bdadcb2704e6c652a765f41dbe1d961&t=22915&page=5)  

## 25-fomula
Run **25-coef-fomula-finder.cpp**.  
The results are in **25-fomula.txt**  

## 1036-nice-pair
Run **1036-nice-pair-finder.cpp**. It takes about 4 hours(NP).  
The results are in **1036-nice-pair.txt**.  

## Additional Problem
It is enough to solve the problem up to this point.   
The following content is for compressing the output values, and can be skipped if not necessary.  

The PS-site(boj) does not support more than 500kb.  
Even 25-fomula & 1036-nice-pair is not afford to that.  
Compaction is needed.  

## 1036-nice-pair-chain
According to this [link](https://oeis.org/A071983/list), it can be predicted that the candidates for the square-sum-array will increase exponentially as the length n increases.   
Therefore, I predicted that there would be an answer in the case where a square-sum-array of length n and a square-sum-array of length n+1 form a chain at some level.  
This prediction was successful.   


Run **1036-nice-pair-chain-finder.cpp**. It uses multi-thread and takes about 2 days.  
The results are in **1036-nice-pair-chain.txt**.  

## Compaction
Given that 1036 < 63*63, we can first compress the adjacent square numbers into base64.  
After converting these numbers into characters, we compress repeating strings using unused characters.  
The compaction and decompaction processes are contained in **1036-nice-pair-chain-compressor.cpp** and **1036-nice-pair-chain-decompressor.cpp**, respectively.  
The compressed result is stored in **compressed-1036-nice-pair-chain.txt**.  
