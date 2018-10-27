//
//  tr2u.c
//  
//
//  Created by Anirudh Balasubramaniam on 5/11/18.
//

#include <stdio.h>


int main(int argc, const char * argv[])
{
    for(int i = 0; i < 5000000; i++)
    {
        int num = (rand() % (26)) + 65;
        int ul = (rand() % (1));
        putchar(num+32*ul);
    }
    
    return 0;
    
}
