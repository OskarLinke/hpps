#include "numlib.h"
#include <stdint.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>

int main(){

    double result = 0; 
    int i = 0;
    double recentnumber; 

    while(1){
        skipspaces(stdin);
        int err = read_double_bin(stdin, &recentnumber); 
        if(err == EOF){
            break;
        }else if (err == 1){
            return 1;
        }
        result = result + recentnumber;
        i++;
    }
    if( i != 0){ 
        result = result/i; 
    } else{
        return 1;
    }

    
    printf("%f \n", result); 

    return 0; 


}