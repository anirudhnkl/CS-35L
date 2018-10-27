//
//  tr2b.c
//  
//
//  Created by Anirudh Balasubramaniam on 5/11/18.
//

#include <stdio.h>

void checkError(int choose)
{
    switch(choose)
    {
        case 1:
            if(ferror(stdin))
            {
                fprintf(stderr, "Error reading file.");
                exit(1);
            }
            break;
        case 2:
            if(ferror(stdin))
            {
                fprintf(stderr, "Error reading file.");
                exit(1);
            }
            break;
    }
}

int main(int argc, const char * argv[])
{
    const char* from, *to;
    char c;
    int foundChar = 0;
    int len;
    size_t i, j;
    
    if(argc != 3)
    {
        fprintf(stderr, "Incorrect number of args.");
        exit(1);
    }
    
    from = argv[1];
    to = argv[2];
    len = strlen(from);
    
    if(len != strlen(to))
    {
        fprintf(stderr, "Arguements do not have the SAME length.");
        exit(1);
    }
    
    for(i = 0; i < len; i++)
    {
        for(j = i + 1; j < len; j++)
        {
            if(from[i] == from[j])
            {
                fprintf(stderr, "from has duplicate bytes.");
                exit(1);
            }
        }
    }
    
    c = getchar();
    checkError(1);
    
    while(c != EOF)
    {
        for(i = 0; i < len; i++)
        {
            if(c == from[i])
            {
                foundChar = 1;
                putchar(to[i]);
                checkError(2);
                break;
            }
        }
        
        if(!foundChar)
        {
            putchar(c);
            checkError(2);
        }
        
        c = getchar();
        checkError(1);
        foundChar = 0;
    }
    
    exit(0);
    
}
