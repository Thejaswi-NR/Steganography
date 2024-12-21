#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "decode.h"
#include "types.h"
#include "common.h"

Status open_files_D(DecodeInfo *deInfo)
{
    deInfo->fptr_stego_image1 = fopen(deInfo->stego_image_fname1, "r");
    // Do Error handling
    if (deInfo->fptr_stego_image1 == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", deInfo->stego_image_fname1);

    	return e_failure;
    }

    // No failure return e_success
    printf("INFO: Done\n");
    return e_success;
}
Status do_decoding(DecodeInfo *deInfo){
    if(open_files_D(deInfo)==e_success){
		printf("file opened sucessfuly\n");
        if(decode_magic_string(deInfo)==e_success){
        printf("magic string decoded sucessfull\n");
            if(decode_secret_file_extn_size(deInfo)==e_success){
            printf("decoding secret file extension size sucessfull\n");
            if(decode_secret_file_extn(deInfo)==e_success){
                printf("decoding of extension sucessfull\n");
                if(decode_secret_file_size(deInfo)==e_success){
                    printf("decoding of output file size sucessfull\n");
                    if(decode_secret_file_data(deInfo)==e_success){
                        printf("decoding of output file data sucessfull\n");
                    }
                    else{
                        printf("decoding of output file data failed\n");
                        return e_failure;
                    }
                }
                else{
                   printf("decoding of output file size failed\n"); 
                   return e_failure;
                }
            }
            else{
                printf("decoding of extension failed\n");
            }
            }
            else{
            printf("decoding secret file extension size failed\n");
            return e_failure;
            }
        }
        else{
            printf("magic string decode is failed");
            return e_failure;
        }
    }
    else{
        printf("file opened failed");
        return e_failure;
    }
    printf("INFO: Done\n");
    return e_success;
}
Status decode_magic_string(DecodeInfo *deInfo){
   fseek(deInfo->fptr_stego_image1,54,SEEK_SET);
   char buffer[32];
   int size;
   fread(buffer,32,1,deInfo->fptr_stego_image1); //reading 32 bytes from desination
   decode_size_from_lsb(&size,buffer);// to get the size of the magic string 
   //printf("size of magic_string %d\n",size);

   char arr[size+1];
   char buffer2[8];
    for(int i=0;i<size;i++){
       fread(buffer2,8,1,deInfo->fptr_stego_image1);// reading 8 bytes from the desination
       decode_byte_from_lsb(&arr[i],buffer2); //to get the magic string #*
    }
    arr[size]='\0';// to put null at the end of magic string
    //printf("%s\n",arr);// to print the magic string 
    if(strcmp(arr,"#*")==0){
        printf(" Magic string matched\n");
    }
    else{
         printf("Magic string not matched \n");
         return e_failure;
    }
    printf("INFO: Done\n");
    return e_success;
   
}
Status read_and_validate_decode_args(char *argv[], DecodeInfo *deceInfo)
{
    if(argv[2] != NULL && strcmp((strstr(argv[2],".")),".bmp")==0)
    {
        deceInfo->stego_image_fname1=argv[2];
    }
    else
    {
        return e_failure;
    }
    if(argv[3] != NULL)
    {
       strcpy(deceInfo->output1,argv[3]);
    }
    else{
         strcpy(deceInfo->output1,"output");
    }
    return e_success;
}
Status decode_secret_file_extn_size(DecodeInfo *deInfo){
    int size1=0;
    char buffer1[32];
	
		fread(buffer1,1,32,deInfo->fptr_stego_image1);
		decode_size_from_lsb(&size1,buffer1);
        //printf("1:::%ld\n",ftell(deInfo->fptr_stego_image1));
	    deInfo->size_output_ext_file=size1;
        //printf(".......%d\n",size1);
        printf("INFO: Done\n");
	    return e_success;
}
Status decode_secret_file_extn(DecodeInfo *deInfo){
    char buffer3[8];
    //int size2=0;
    char arr[deInfo->size_output_ext_file];
	for(int i=0;i<deInfo->size_output_ext_file;i++){
		fread(buffer3,1,8,deInfo->fptr_stego_image1);
		decode_byte_from_lsb(&arr[i],buffer3);
	}
    arr[deInfo->size_output_ext_file]='\0';
    char *str=strcat(deInfo->output1,arr);
    deInfo->fptr_secret1 =fopen(str,"w");
    //printf("%s\n",arr);
    //printf("!!!!!!%s\n",str);
    printf("INFO: Done\n");
	return e_success;
}
Status decode_secret_file_size(DecodeInfo *deInfo){
    int size2=0;
    char buffer6[32];
	
		fread(buffer6,1,32,deInfo->fptr_stego_image1);
		decode_size_from_lsb(&size2,buffer6);
        //printf("2:::%ld\n",ftell(deInfo->fptr_stego_image1));
	    deInfo->size_output_file1=size2;
        
        //printf(".......%d\n",size2);
        printf("INFO: Done\n");
	    return e_success;

}
Status decode_secret_file_data(DecodeInfo *deInfo){
   char buffer5[8];
    //int size2=0;
    char arr1[deInfo->size_output_file1];
	for(int i=0;i<(deInfo->size_output_file1);i++){
		fread(buffer5,1,8,deInfo->fptr_stego_image1);
		decode_byte_from_lsb(&arr1[i],buffer5);
       // printf("%c\n",arr1[i]);
	}
    arr1[deInfo->size_output_file1]='\0';
    //printf(" %s",arr1); 
    fwrite(arr1,deInfo->size_output_file1,1,deInfo->fptr_secret1);
    fclose(deInfo->fptr_secret1);
    //printf("INFO: Done\n");
    return e_success;
}
Status decode_byte_from_lsb(char *data, char *image_buffer){
    char ch=0;
    for(int i=0;i<8;i++){
        ch=((image_buffer[i] & 1)<<(7-i))|ch;
      //  printf("print--->%c\n",ch);
        
    }
    *data=ch;
    printf("INFO: Done\n");
    return e_success;
}
Status decode_size_from_lsb(int *size,char *image_buffer){
    char ch=0;
    for(int i=0;i<31;i++){
        ch=((image_buffer[i] & 1)<<(31-i) )| ch;
    }
    *size=ch;
    printf("INFO: Done\n");
    return e_success; 
}