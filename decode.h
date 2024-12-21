#ifndef DECODE_H
#define DECODE_H

#include "types.h"

#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 4

typedef struct _DecodeInfo
{
    // char *src_image_fname1;  
    // FILE *fptr_scr_image1;
    // char extn_scr_file1[MAX_FILE_SUFFIX];
    // // char secret_data[MAX_SECRET_BUF_SIZE];
    // long size_scr_file1;

    /* Secret File Info */
    char output1[20];  
    FILE *fptr_secret1;
    int size_output_file1;
    int size_output_ext_file;

    /* Stego Image Info */
    char *stego_image_fname1;
    FILE *fptr_stego_image1;

} DecodeInfo;
/* Read and validate Encode args from argv */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decodeInfo);

/* Perform the encoding */
Status do_decoding(DecodeInfo *decodeInfo);

/* Get File pointers for i/p and o/p files */
Status open_files_D(DecodeInfo *decodeInfo);

/* Store Magic String */
Status decode_magic_string(DecodeInfo *decodeInfo);

/* Encode secret file extenstion */
Status decode_secret_file_extn(DecodeInfo *decodeInfo);

/* encode secret file extension size*/
Status decode_secret_file_extn_size(DecodeInfo *decodeInfo);

/* Encode secret file size */
Status decode_secret_file_size(DecodeInfo *decodeInfo);

/* Encode secret file data*/
Status decode_secret_file_data(DecodeInfo *decodeInfo);

/* Encode a byte into LSB of image data array */
Status decode_byte_from_lsb(char *data, char *image_buffer);

/* encode size to lsb */
Status decode_size_from_lsb(int *size,char *image_buffer);
/*compair the  magic string*/
#endif