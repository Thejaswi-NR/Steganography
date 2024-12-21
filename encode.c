#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "encode.h"
#include "types.h"
#include "common.h"

/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    printf("height = %u\n", height);

    // Return image capacity
    return width * height * 3;
}

/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

    	return e_failure;

    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

    	return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

    	return e_failure;
    }

    // No failure return e_success
	printf("INFO: Done\n");
    return e_success;
}
OperationType check_operation_type(char *str[]){
	if(strcmp(str[1],"-e")==0){
		printf("Encoding\n");
		return e_encode;
	}
	else if(strcmp(str[1],"-d")==0){
		printf("Decoding\n");
		return e_decode;
	}
	else{
		return e_unsupported;
	}
}
    
Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
		if( argv[2] != NULL && strcmp ((strstr(argv[2],".")),".bmp" )==0)
		{
            encInfo->src_image_fname=argv[2];
		}
		else 
		{
		    return e_failure;
		}
	    if(argv[3] != NULL && (strchr(argv[3],'.'))!=NULL)
		{
			encInfo->secret_fname=argv[3];
			strcpy(encInfo->extn_secret_file, strchr(argv[3],'.'));
		}
		else
		{
				return e_failure;
		}
		if(argv[4] != NULL)
		{
			if(strcmp ((strstr(argv[4],".")),".bmp" )==0)
			{
				encInfo->stego_image_fname=argv[4];
			}
			else 
			{
				printf("ERROR: Please enter the .bmp file name for encoded output file name\n");
				return e_failure;
			}
		}
		else
		{
			encInfo->stego_image_fname="stgeno.bmp";
			printf("INFO: Encoded file Created as %s\n", encInfo->stego_image_fname);
		}
		return e_success;
}
Status do_encoding(EncodeInfo *encInfo)
{
	if(open_files(encInfo)==e_success){
		printf("file opened sucessfuly\n");
		if(check_capacity(encInfo) == e_success)
		{
		printf("Checking capacity is done\n");
		if(copy_bmp_header(encInfo->fptr_src_image,encInfo->fptr_stego_image)==e_success){
		 printf("copy of bmp header sucessfull\n ");
			if(encode_magic_string(MAGIC_STRING ,encInfo)==e_success){
				printf("Magic string encoding sucessfull\n");
				if(encode_secret_file_extn_size(strlen(encInfo->extn_secret_file),encInfo)==e_success){
				   printf("Encoding secret file extension size sucessfull\n");
				    if(encode_secret_file_extn(encInfo->extn_secret_file,encInfo)==e_success){
					printf("Encoding secret extension size sucessfull\n");
					    if(encode_secret_file_size(encInfo->size_secret_file,encInfo)==e_success){
						printf("Encoding of secret file size sucessful\n");
						    if(encode_secret_file_data(encInfo)==e_success){
							printf("Encoding of secret file data sucessfull\n");
							    if(copy_remaining_img_data(encInfo->fptr_src_image,encInfo->fptr_stego_image)==e_success){
                                 printf("coping of the data source to desination\n"); 
							    }
								else{
									printf("coping of the data from source to destination failed\n");
									return e_failure;
								}
						    }
						    else{
							printf("Encoding of secret file data failed\n");
						    }
					    }
					    else{
						   printf("Encodeing of Secret file size failed\n");
						   return e_failure;
					    }
					}
					else{
					  printf("Encoding secret extension size failed\n");
					  return e_failure;
				    }
				}
				else{
					printf("Encoding secret file extension size failed\n");
					return e_failure;
				}
				
			}
			else{
				printf("Magic string encoding failure\n");
				return e_failure;
			}
		}
	    else{
		printf("copy of bmp header failed\n ");
		return e_failure;
	    }
		}
	else
	{
		printf("Check  capacity failed\n");
	}
    }
    else{
	  printf("file not opened\n");
	  return e_failure;
    }
	printf("INFO: Done\n");
	return e_success;
}
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image){
	char *ptr=malloc(54*sizeof(char));  // create 54 bytes of char type
    fseek(fptr_src_image,0,SEEK_SET);  // to move the curser to the start position of bmp of source file 
	fread(ptr,54,1,fptr_src_image);
	 //printf("1....%ld",ftell(fptr_src_image));                               // read 54 bytes from source image
	fwrite(ptr,54,1,fptr_dest_image);  // write 54 bytes in the destination file
   
	free(ptr);  // to free the 54 bytes 
	printf("INFO: Done\n");
	return e_success;
}
Status check_capacity(EncodeInfo *encInfo){
	encInfo->image_capacity=get_image_size_for_bmp(encInfo->fptr_src_image);
	encInfo-> size_secret_file=get_file_size(encInfo->fptr_secret);
	if(encInfo->image_capacity > (54+(2+4+4+4+encInfo-> size_secret_file)*8)){
		printf("INFO: Done\n");
		return e_success;
	}
	else{
		return e_failure;
	}
}
uint get_file_size(FILE *fptr)  //<---- function definiton for get file size
{
    int size;
    fseek(fptr, 0, SEEK_END);   //<---- file pointer move from 0 position to end position 
    size = ftell(fptr);        //<---- getting the total size of the file
    rewind(fptr);             //<--- moves the file pointer to the beginning of the file
    return size;             //<----- returning the size value
}
Status encode_magic_string( char *magic_string, EncodeInfo *encInfo){
	   char buffer2[32];
	   int size=strlen(magic_string);
		fread(buffer2,1,32,encInfo->fptr_src_image);
		encode_size_to_lsb(size,buffer2);
		 //printf("pointer ____________-%ld\n",ftell(encInfo->fptr_src_image));
		fwrite(buffer2,1,32,encInfo->fptr_stego_image);
	   encode_data_to_image(magic_string,size,encInfo->fptr_src_image,encInfo->fptr_stego_image);
	   printf("INFO: Done\n");
	return e_success;
}
Status encode_data_to_image(char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image){
	char buffer[8];
	for(int i=0;i<size;i++){
		fread(buffer,1,8,fptr_src_image);
		encode_byte_to_lsb(data[i],buffer);
		//printf("................%c\n",data[i]); 
		fwrite(buffer,1,8,fptr_stego_image);

	}
}
Status encode_byte_to_lsb(char data, char *image_buffer){
	for(int i=0;i<8;i++){
		image_buffer[i]=(((unsigned)data & (1<<(7-i)))>>(7-i)) | (image_buffer[i] & (~(1)));
	}
}


Status encode_secret_file_extn(char *file_extn, EncodeInfo *encInfo){
	//printf("2.....%ld",ftell(encInfo->fptr_src_image));
	encode_data_to_image(file_extn,strlen(file_extn),encInfo->fptr_src_image, encInfo->fptr_stego_image);
	printf("INFO: Done\n");
	return e_success;
}
Status encode_secret_file_extn_size(int size, EncodeInfo *encInfo){
	char buffer2[32];
	//printf("'%d\n",size);
		fread(buffer2,1,32,encInfo->fptr_src_image);
		encode_size_to_lsb(size,buffer2);
		//printf("1....%ld",ftell(encInfo->fptr_src_image));
		fwrite(buffer2,1,32,encInfo->fptr_stego_image);
		printf("INFO: Done\n");
	return e_success;
}
Status encode_secret_file_size(int size, EncodeInfo *encInfo){
	char buffer3[32];
	//printf("'%d\n",size);
	  //printf("1....%ld\n",ftell(encInfo->fptr_src_image));
		fread(buffer3,1,32,encInfo->fptr_src_image);
		encode_size_to_lsb(size,buffer3);
		//printf("1....%ld",ftell(encInfo->fptr_src_image));
		fwrite(buffer3,1,32,encInfo->fptr_stego_image);
		printf("INFO: Done\n");
	return e_success;
}
Status encode_secret_file_data(EncodeInfo *encInfo){
	rewind(encInfo->fptr_secret);
    char buffer3[encInfo->size_secret_file+1];
	fread(buffer3,encInfo->size_secret_file,1,encInfo->fptr_secret);
	//printf("    %ld",ftell(encInfo->fptr_src_image));
	encode_data_to_image(buffer3,encInfo->size_secret_file,encInfo->fptr_src_image,encInfo->fptr_stego_image);
	printf("INFO: Done\n");
	return e_success;
}
Status encode_size_to_lsb(int size, char *image_buffer){
	for(int i=0;i<31;i++){
		image_buffer[i]=(((unsigned)size & (1<<(31-i)))>>(31-i)) | (image_buffer[i] & (~(1)));
	}
}
Status copy_remaining_img_data(FILE *fptr_src_image, FILE *fptr_dest_image){
	char ch;
	while(fread(&ch,1,1,fptr_src_image)!=0){
		fwrite(&ch,1,1,fptr_dest_image);
	}
	printf("INFO: Done\n");
	return e_success;
}



