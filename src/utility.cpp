#include "utility.h"
#include <stdio.h>

void parse_config(string filename) // da convertire a cpp e da sistemare, usa regex per parserare il file di config
{
    char key[128];
    char value[128];
    int ret;
    FILE *fconf=fopen(filename,"r");
    
    if(!fconf)
    {
       cout << "no config file "<< filename <<" found, using default settings." << endl;
       return 0; 
    }
    
    cout << "parsing config file..." << endl;
       
        while(1)
        {
            ret = fscanf(fconf, "%[^=\n]=%[^\n]", key, value);
            fgetc(fconf);
            if(ret == 2)
            {
                if(strcmp(key,"clocks") == 0)
                {
                    clocks=atoi(value);
                    printf("config: %s set to: %s\n", key, value);
                }
                if(strcmp(key,"study") == 0)
                {
                    strcpy(study,value);
                    printf("config: %s set to: %s\n", key, value);
                }
            }
            else if (ret == 1)    /* commenti*/
            {
                if (key[0] == '#'); /*printf("Commento: %s\n", key);*/
                else 
                    printf("Configuration error: %s\n", key);
            }
            else
            {
                int c = fgetc(fconf);
                if(c == EOF)
                    break;
                else
                    ungetc(c, fconf);
            }
        }

        fclose(fconf);
}
