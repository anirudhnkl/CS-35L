//
//  tr2u.c
//  
//
//  Created by Anirudh Balasubramaniam on 5/11/18.
//

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

void checkError(ssize_t choose)
{
    if(choose < 0)
    {
        fprintf(stderr, "IO error.");
        exit(1);
    }
}

int main(int argc, const char * argv[])
{
    const char* from, *to;
    char c[2];
    ssize_t r;
    size_t i, j;
    int foundChar = 0;
    int len;
    
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

    r = read(0,c, 1);
    checkError(r);
    
    while(r > 0)
    {
        for(i = 0; i < len; i++)
        {
            if(c[0] == from[i])
            {
                foundChar = 1;
                c[0] = to[i];
                ssize_t w = write(1, c, 1);
                checkError(w);
                break;
            }
            else
                foundChar = 0;
        }
        
        if(!foundChar)
        {
            ssize_t w = write(1, c, 1);
            checkError(w);
        }
        
        r = read(0,c, 1);
        checkError(r);
        foundChar = 0;
    }
    
    exit(0);
    
}
