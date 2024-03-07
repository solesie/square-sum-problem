# Numberphile's Square-Sum Problem
백준 28306 제곱수 순열 [문제](https://www.acmicpc.net/problem/28306)  

## 접근
최소 1036길이의 base square-sum sequence를 통해 무한대의 square-sum sequence 로 확장이 가능합니다.  
이와 관련하여 두개의 링크를 첨부합니다.  
- [링크1](https://www.youtube.com/watch?v=-vxW42R47bc)  
- [링크2](https://www.mersenneforum.org/showthread.php?s=5bdadcb2704e6c652a765f41dbe1d961&t=22915&page=5)  

## 25-fomula
1036길이의 base square-sum sequence 를 무한대 범위의 square-sum sequence 로 확장할 공식이 필요합니다.  
**25-coef-fomula-finder.cpp** 는 그 역할을 담당합니다.  
공식의 결과는 **25-fomula.txt** 입니다.

## 1036-nice-pair
25-fomula 를 적용하기 위해서는 (n 길이의 제곱수 순열, n+1 길이의 제곱수 순열) 쌍을 1036까지 구해야 합니다.   
이 1036 길이 까지의 쌍을 1036-nice-pair 이라 합시다.

이제 1036-nice-pair 를 찾아내야 합니다.  
**1036-nice-pair-finder.cpp** 를 통해 1036-nice-pair를 알아낼 수 있습니다.  
1036 까지는 NP(헤밀턴 경로) 이므로 여러 가지치기 기법들을 적용해야 합니다.  
일반적인 가지치기 기법을 적용하였을 때 4시간 정도에 구할 수 있습니다.  
결과값은 **1036-nice-pair.txt** 에 있습니다.  

우선 말하자면, 여기서 구한 1036-nice-pair 를 통해선 문제를 해결할 수 없습니다.  
백준 online judge 는 500kb 이상의 파일을 지원하지 않습니다.  
그러나 1036-nice-pair는 약 2000개의 순열이 존재하고, 이는 세상의 어떤 압축기법을 적용한다 하더라도 3mb의 이 파일을 500kb 아래로 손상없이 줄일 수 없습니다.  

## 새로운 접근
기존 1036-nice-pair 의 (n, n+1), (n+1, n+2)... 에서 공통되는 n+1길이의 제곱수 순열을 공통되게 만들고 압축을 적용한다면 500kb 아래로 줄일 수 있습니다.  
그러나 이렇게 chain을 만들게 되면 경우의 수가 기하급수적으로 증가합니다.  
외판원 문제에서 n=24 일때 동적계획법을 적용한다 하더라도 800초 가량이 걸리니, 1036까지는 거의 불가능에 가깝습니다.  
하지만 이 [링크](https://oeis.org/A071983/list) 를 보면 n이 커질수록 그 경우의 수만큼 비례하게 제곱수 순열의 개수도 증가하는 것을 볼 수 있습니다.  
멀티스레드를 적용하여 일정 시간 이상 찾을 수 없다면 가지치기를 적용하고, 가지의 위치를 바꾸는 등 여러 기법들을 적용해봅니다.

## 1036-nice-pair-chain
**1036-nice-pair-chain-finder.cpp** 는 1036-nice-pair-chain 을 구하는 코드입니다.  
예상대로 n=700에 도달하고 나서 부터는 구하는 속도가 오히려 빨라지는 것을 관찰할 수 있었습니다.  
2일 정도의 시간을 거치면 1036-nice-pair-chain을 구할 수 있습니다.  
**1036-nice-pair-chain.txt** 에 결과값이 존재합니다.  

## 압축
1036 < 63*63 임을 생각하면, 우선 인접한 제곱수들을 base64로 압축할 수 있습니다.  
이렇게 숫자들을 문자로 바꾼 후, 반복되는 문자열을 사용되지 않는 문자로 압축합니다.  
zip과 unzip 과정은 각각 **1036-nice-pair-chain-compressor.cpp**, **1036-nice-pair-chain-decompressor.cpp** 에 있고, 압축된 결과는 **compressed-1036-nice-pair-chain.txt** 에 있습니다.  

## 마무리
**main.cpp** 은 1036-nice-pair-chain, 1036-nice-pair-chain-decompressor, 25-fomula 등으로 이루어져있고, 실행하면  
![image](https://github.com/solesie/square-sum-problem/assets/33522810/63a6546d-b995-457d-be1d-35583c8b9bc6)
해결할 수 있습니다.
