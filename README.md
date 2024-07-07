# Numberphile's Square-Sum Problem
25-fomula approach.  
It lowers the size.  

## Link 
- [youtube](https://www.youtube.com/watch?v=-vxW42R47bc)  
- [mersenneforum](https://www.mersenneforum.org/showthread.php?s=5bdadcb2704e6c652a765f41dbe1d961&t=22915&page=5)  

## 25-fomula
(**25-coef-fomula-finder.cpp**)  

## 1036-nice-pair
Run **1036-nice-pair-finder.cpp**. It takes about 4 hours(NP).  
The results are in **1036-nice-pair.txt**.  

(However, PS-site(boj) does not support more than 500kb. Compaction is needed.)  

## Heuristic using multi-thread
[oeis](https://oeis.org/A071983/list)  
Run **1036-nice-pair-chain-finder.cpp**. It takes aboud 2 days.  
The results are in **1036-nice-pair-chain.txt**.  

## Compaction
Given that 1036 < 63*63, we can first compress the adjacent square numbers into base64.  
After converting these numbers into characters, we compress repeating strings using unused characters.  
The compaction and decompaction processes are contained in **1036-nice-pair-chain-compressor.cpp** and **1036-nice-pair-chain-decompressor.cpp**, respectively.  
The compressed result is stored in **compressed-1036-nice-pair-chain.txt**.  
