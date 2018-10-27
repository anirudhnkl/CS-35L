//
//  main.c
//  sfrob
//
//  Created by Anirudh Balasubramaniam on 5/7/18.
//  Copyright © 2018 Anirudh Balasubramaniam. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <limits.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

int frobcmp(char const *a, char const *b)
{
    while(1)
    {
        if(*a == ' ' && *b == ' ')
            return 0;
        if((*a ^ 42) > (*b ^ 42) || *b == ' ')
            return 1;
        if((*b ^ 42) > (*a ^ 42) || *a == ' ')
            return -1;
        
        a++;
        b++;
    }
    return 0;
}

int frobcmp_nocase(char const *a, char const *b) 
{
    if ((*a ^ 42) > UCHAR_MAX || (*b ^ 42) > UCHAR_MAX || (*a ^ 42) < 0 || (*b ^ 42) < 0) 
        checkError(5);
    
    while(1)
    {
        if(*a == ' ' && *b == ' ')
            return 0;
        if(toupper(*a ^ 42) > toupper(*b ^ 42) || *b == ' ')
            return 1;
        if(toupper(*b ^ 42) > toupper(*a ^ 42) || *a == ' ')
            return -1;
        
        a++;
        b++;
    }
    return 0;
}

int comp(const void *a, const void *b)
{
    return(frobcmp(*(const char **) (a), *(const char **) (b)));
}

int comp_nocase(const void *a, const void *b)
{
    return(frobcmp_nocase(*(const char **) (a), *(const char **) (b)));
}

int checkError(int errorType)
{
    char * error = "";
    
    switch(errorType)
    {
        case 1:
            error = "IO error.";
            break;
        case 2:
            error = "Error allocating memory.";
            break;
        case 3:
            error = "Error getting file status.";
            break;
        case 4:
            error = "Invalid arguements";
            break;
        case 5:
            error = "Invalid characters for use with -f";
            break;
    }

    if(errorType < 6)
    {
        fprintf(stderr, error);
        exit(1);
    }
    return 1;
}

int main(int argc, const char * argv[])
{
    struct stat status;
    char ** words;
    ssize_t currState, nextState;
    char * word, * bufWord, curr[2], next[2];    //TODO: free
    int wordIter = 0, wordsIter = 0, bufferSize, newWord = 1, wordsSize, ignoreCase = 0;
    
    if(argc > 2) 
        checkError(4);
    else if(argc == 2) 
    {
        if(strcmp(argv[1], "-f") == 0) 
            ignoreCase = 1;
        else
            checkError(4);
    }

    if(fstat(0, &status) == -1)
        checkError(3);

    bufferSize = status.st_size;
    
    if(S_ISREG(status.st_mode))
    {
        word = (char*) malloc((bufferSize+1) * sizeof(char));
        if(word == NULL)
            checkError(2);
        checkError(read(0, word, bufferSize));
        word[bufferSize] = ' ';
        wordsSize = bufferSize/10;
        words = (char**)malloc(wordsSize * sizeof(char*));
        if(words == NULL)
            checkError(2);

        size_t i = 0;

        for(i = 0; i < bufferSize; i++)
        {
            if(newWord == 0 && word[i] != ' ')
            {
                words[wordsIter] = &word[i];
                wordsIter++;
                newWord = 1;
            }
            else if(newWord == 1 && word[i] == ' ')
                newWord = 0;

            if(wordsIter == wordsSize) 
            {
                wordsSize *= 2;
                words = realloc(words, sizeof(char *) * wordsSize);
                if(words == NULL)
                    checkError(2);
            }
        }
    }

    else
    {
        words = (char**)malloc(sizeof(char*));
    }

    bufWord = (char *) malloc(sizeof(char));
    currState = read(0, curr, 1);
    if(currState < 0)
        checkError(1);

    wordIter = 0;

    while(currState > 0)
    {
        bufWord[wordIter] = curr[0];
        bufWord = realloc(bufWord, (wordIter + 2) * sizeof(char));
        nextState = read(0, next, 1);
        if(nextState < 0)
            checkError(1);
        
        if(bufWord == NULL)
            checkError(2);
        
        if(curr[0] == ' ')
        {
            words[wordsIter] = bufWord;
            words = realloc(words, (wordsIter + 2) * sizeof(char *));
            
            if(words == NULL)
                checkError(2);
            else
            {
                wordsIter++;
                bufWord = NULL;
                bufWord = (char *) malloc(sizeof(char));
                wordIter = -1;
            }
            
            if(nextState == 0)
                break;
            
            if(next[0] == ' ')
            {
                while(curr[0] == ' ')
                {
                    currState = read(0, curr, 1);
                    if(currState < 0)
                        checkError(1);
                }
                nextState = read(0, next, 1);
                if(nextState < 0)
                    checkError(1);
                wordIter++;
                continue;
            }
        }
        
        if(nextState == 0)
        {
            curr[0] = ' ';
            wordIter++;
            continue;
        }
        
        curr[0] = next[0];
        wordIter++;
    }
    
    if(ignoreCase) 
        qsort(words, wordsIter, sizeof(char *), comp_nocase);
    else
        qsort(words, wordsIter, sizeof(char *), comp);
    size_t i = 0;
    size_t j = 0;
    
    for(i = 0; i < wordsIter; i++)
    {
        for(j = 0; ;j++)
        {
            if(putchar(words[i][j]) == EOF)
            {
                fprintf(stderr, "Error  writing character.");
                exit(1);
            }
            if(words[i][j] == ' ')
            {
                break;
            }
        }
    }

    free(words);        
    exit(0);
}
