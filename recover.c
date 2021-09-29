#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#define BLOCKSIZE 512
 
bool isjpgheader(uint8_t buffer[])
{
    return buffer[0] == 0xff
    && buffer[1] == 0xd8
    && buffer[2] == 0xff
    && (buffer[3] & 0xf0) == 0xe0; 
}

int main(int argc, char *argv[])
{
    if(argc!=2)
    {
        printf("Usage: ./recover image\n");
        return 1;
    }
    
    char* inputFile = argv[1];
    if(inputFile == NULL)
    {
        printf("Usage: ./recover image\n");
        return 2;
    }
    
    FILE* inputptr = fopen(inputFile, "r");
    
    if(inputptr == NULL)
    {
        printf("Unable to open file\n");
        return 3;
    }
    // xxx.jpg'\0'
    char filename[8];
    FILE* outputptr = NULL;
    uint8_t buffer[BLOCKSIZE];
    int jpgcounter = 0;
    
    while(fread(buffer, sizeof(uint8_t), BLOCKSIZE, inputptr)|| feof(inputptr)==0)
    {
        if(isjpgheader(buffer))
        {
            if(outputptr != NULL)
            {
                fclose(outputptr);
            }
            sprintf(filename, "%03i.jpg", jpgcounter);
            outputptr = fopen(filename, "w");
            jpgcounter++;
        }
        if(outputptr != NULL)
        {
            fwrite(buffer, sizeof(buffer), 1, outputptr);
        }
    }
    if(inputptr == NULL)
    {
        fclose(inputptr);
    }
    if(outputptr == NULL)
    {
        fclose(outputptr);
    }
    return 0;
}