#include "../include/Hog.h"
#include "trace.h"
int Hog::suffix_prefix_length(int i, int j){
    i = sorted_order_conversion[i];
    j = sorted_order_conversion[j];
    int cur = _strToTreeIndex[i];
    while(cur>1){
        if(t[cur].from() <= j && t[cur].to() >= j){
            return t[cur].length();
        } 
        cur = t[cur].suf_link();
    }
    return t[cur].length();
}

std::vector<int> Hog::one_to_all(int i){
    i = sorted_order_conversion[i];
    std::vector<int> answer(sorted_order_conversion.size());
    int current = _strToTreeIndex[i];
    std::vector<int> nextRemaining(sorted_order_conversion.size());
    for(int i = 0;i<(int)nextRemaining.size();i++){
        nextRemaining[i] = i;
    }
    while(current>1){
        int start = t[current].from();
        int end = t[current].to();
        int curIndex = start;
        while(curIndex <= end){
            while(curIndex <= end && curIndex != nextRemaining[curIndex]){
                curIndex = nextRemaining[curIndex];
            }
            if(curIndex > end)break;
            answer[curIndex] = t[current].length();
            curIndex = curIndex+1;
        }
        nextRemaining[start] = end+1;
        current = t[current].suf_link();
    }
    std::vector<int>answerFinal(sorted_order_conversion.size());
    for(int i = 0;i<(int)sorted_order_conversion.size();i++){
        answerFinal[sorted_order_conversion[i]] = answer[i];
    }
    return answerFinal;
}

std::vector<int>  Hog::top(int i, int k){
    i = sorted_order_conversion[i];
    std::vector<int> answer;
    int current = _strToTreeIndex[i];
    std::vector<int> nextRemaining(sorted_order_conversion.size());
    for(int i = 0;i<(int)nextRemaining.size();i++){
        nextRemaining[i] = i;
    }
    int currentCount = 0;
    while(true){
        int start = t[current].from();
        int end = t[current].to();
        int curIndex = start;
        while(curIndex <= end){
            while(curIndex <= end && curIndex != nextRemaining[curIndex]){
                curIndex = nextRemaining[curIndex];
            }
            if(curIndex > end)break;
            if(currentCount >= k)break;
            answer.push_back(curIndex);
            currentCount++;
            curIndex = curIndex+1;
        }
        if(currentCount >= k)break;
        nextRemaining[start] = end+1;
        current = t[current].suf_link();
    }
    std::vector<int>answerUserIndex(answer.size());
    for(int i = 0;i<(int)answer.size();i++){
        answerUserIndex[i] = sorted_order[answer[i]];
    }
    return answerUserIndex;
}

std::vector<int> Hog::report(int i, int l){
    i = sorted_order_conversion[i];
    std::vector<int> answer;
    int current = _strToTreeIndex[i];
    std::vector<int> nextRemaining(sorted_order_conversion.size());
    for(int i = 0;i<(int)nextRemaining.size();i++){
        nextRemaining[i] = i;
    }
    int currentCount = 0;
    while(t[current].length() >= l){
        int start = t[current].from();
        int end = t[current].to();
        int curIndex = start;
        while(curIndex <= end){
            while(curIndex <= end && curIndex != nextRemaining[curIndex]){
                curIndex = nextRemaining[curIndex];
            }
            if(curIndex > end)break;
            answer.push_back(curIndex);
            currentCount++;
            curIndex = curIndex+1;
        }
        nextRemaining[start] = end+1;
        current = t[current].suf_link();
    }
    std::vector<int>answerUserIndex(answer.size());
    for(int i = 0;i<(int)answer.size();i++){
        answerUserIndex[i] = sorted_order[answer[i]];
    }
    return answerUserIndex;
}

int Hog::count(int i, int l){
    i = sorted_order_conversion[i];
    int current = _strToTreeIndex[i];
    std::vector<int> nextRemaining(sorted_order_conversion.size());
    for(int i = 0;i<(int)nextRemaining.size();i++){
        nextRemaining[i] = i;
    }
    int currentCount = 0;
    while(t[current].length() >= l){
        int start = t[current].from();
        int end = t[current].to();
        int curIndex = start;
        while(curIndex <= end){
            while(curIndex <= end && curIndex != nextRemaining[curIndex]){
                curIndex = nextRemaining[curIndex];
            }
            if(curIndex > end)break;
            currentCount++;
            curIndex = curIndex+1;
        }
        nextRemaining[start] = end+1;
        current = t[current].suf_link();
    }
    return currentCount;
}
