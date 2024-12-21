#include <stdio.h>
#include<string.h>
#include "encode.h"
#include "types.h"
#include "decode.h"
//#include "encode.c"

int main(int argc, char *argv[]){
    if(argc==1){
        printf(" Encoding: ./lsb_steg -e <.bmp_file> <.text_file> [output file]\n");
        printf("Decoding: ./lsb_steg -d <.bmp_file> [output file]\n");
        return e_failure;
    }
   
    if(argc<3){
        if( argc>1 && strcmp(argv[1],"-e")==0){
            printf(" Encoding: ./lsb_steg -e <.bmp_file> <.text_file> [output file]\n");
        }
        else{
           printf("Decoding: ./lsb_steg -d <.bmp_file> [output file]\n"); 
        }
        return 1;
    }
    if(strstr(argv[2], ".") == NULL || strcmp(strrchr(argv[2], '.'), ".bmp") != 0)
        {
              printf(" Encoding: ./lsb_steg -e <.bmp_file> <.text_file> [output file]\n");
              printf(" Decoding: ./lsb_steg -d <.bmp_file>[output file]\n");
               return 0;
        }
    if(argc==5)
    {
        if(strstr(argv[4], ".") == NULL || strcmp(strrchr(argv[4], '.'), ".bmp") != 0)
        {
              printf(" Encoding: ./lsb_steg -e <.bmp_file> <.text_file> [output file]\n");
              printf(" Decoding: ./lsb_steg -d <.bmp_file>[output file]\n");
               return 0;
        }
    }
    if(check_operation_type(argv)==e_encode){ // to check -e 
        printf("Select encoding \n");
         EncodeInfo encInfo;
        
        if(read_and_validate_encode_args(argv,&encInfo)==e_success){ // to check .bmp and .txt 
            printf("read and validate sucuessfull\n");
            if(do_encoding(&encInfo)==e_success){ // to check the encoding part 
                printf("encoding sucessfull\n");
            }
            else{
                printf("encoding failure\n");
            }
        }
        else{
            printf("read and validate failure\n");
            return 0;
        }
    }
    else if(check_operation_type(argv)==e_decode){
         printf("Start decoding \n");
           DecodeInfo decInfo;
        if(read_and_validate_decode_args(argv,&decInfo)==e_success){ // to check .bmp
            printf("read and validate sucuessfull\n");
            if(do_decoding(&decInfo)==e_success){ // to check the decoding part 
                printf("decoding sucessfull\n");
            }
            else{
                printf("decoding failure\n");
            }
        }
        else{
            printf("read and validate failure\n");
            return 0;
        }
    }
    else{
        printf("Invalid input\n");
    }
    return 0;
}
    

