//
//  main.c
//  sfrob
//
//  Created by Anirudh Balasubramaniam on 5/7/18.
//  Copyright © 2018 Anirudh Balasubramaniam. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

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

int comp(const void *a, const void *b)
{
    return(frobcmp(*(const char **) (a), *(const char **) (b)));
}

int checkError(int errorType)
{
    switch(errorType)
    {
        case 1:
            if(ferror(stdin))
            {
                fprintf(stderr, "Error reading file.");
                exit(1);
            }
            break;
        case 2:
            fprintf(stderr, "Error allocating memory.");
            exit(1);
            break;
    }
    return 1;
}

int main()
{
    char * word = (char*)malloc(sizeof(char));
    char ** words = (char**)malloc(sizeof(char*));
    char curr = getchar();
    checkError(1);
    
    int wordIter = 0, wordsIter = 0;
    
    while(curr != EOF && checkError(1) == 1)
    {
        word[wordIter] = curr;
        char * temp = realloc(word, (wordIter + 2) * sizeof(char));
        char next = getchar();
        checkError(1);
        
        if(temp == NULL)
        {
            free(word);
            checkError(2);
        }
        else
            word = temp;
        
        if(curr == ' ')
        {
            words[wordsIter] = word;
            char ** tempA = realloc(words, (wordsIter + 2) *sizeof(char *));
            
            if(tempA == NULL)
            {
                free(words);
                checkError(2);
            }
            
            else
            {
                words = tempA;
                wordsIter++;
                word = NULL;
                word = (char * ) malloc(sizeof(char));
                wordIter = -1;
            }
            
            if(next == EOF)
                break;
            
            if(next == ' ')
            {
                while(curr == ' ')
                {
                    curr = getchar();
                    checkError(1);
                }
                next = getchar();
                checkError(1);
                wordIter++;
                continue;
            }
        }
        
        if(next == EOF)
        {
            curr = ' ';
            wordIter++;
            continue;
        }
        
        curr = next;
        wordIter++;
    }
    
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

    
    for(i = 0; i < wordsIter; i++)
    {
        free(words[i]);
    }
    
    free(words);
    
    exit(0);
}
