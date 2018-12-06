#include <iostream>
#include <stdio.h>

typedef  unsigned char byte;

/* Print n as a binary number */
inline void printBits(unsigned char n)
{
   
        unsigned char i;
        i = 1<<(sizeof(n) * 8 - 1);

        while (i > 0) {
                if (n & i)
                        printf("1 ");
                else
                        printf("0 ");
                i >>= 1;
        }
printf("\n");
}


unsigned char reverse_byte (byte num)
{
	byte bit;
	byte output;
	
   for(int count=1;count<=8;count++)
	{ 
		bit = num & 0x01;
		num = num>>1;
		output = output<<1; 
		if(bit==1) 
		output = output+1;
	}


    return output;
}
