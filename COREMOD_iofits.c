/**
 * @file    COREMOD_iofits.c
 * @brief   I/O for FITS files
 *
 * Uses CFITSIO library heavily
 *
 *
 */




/* ================================================================== */
/* ================================================================== */
/*            MODULE INFO                                             */
/* ================================================================== */
/* ================================================================== */

// module default short name
// all CLI calls to this module functions will be <shortname>.<funcname>
// if set to "", then calls use <funcname>
#define MODULE_SHORTNAME_DEFAULT ""

// Module short description
#define MODULE_DESCRIPTION       "Read/Write FITS files"





#include <stdint.h>
#include <fitsio.h> /* required by every program that uses CFITSIO  */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>

#include "CommandLineInterface/CLIcore.h"
#include "COREMOD_tools/COREMOD_tools.h"
#include "COREMOD_iofits/COREMOD_iofits.h"
#include "COREMOD_memory/COREMOD_memory.h"


#define SBUFFERSIZE 1000


#define COREMOD_IOFITS_DEBUG 1

#ifdef COREMOD_IOFITS_DEBUG
#define COREMOD_IOFITS_PRINTDEBUG do {                      \
    printf("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__); \
    } while(0)
#else
#define COREMOD_IOFITS_PRINTDEBUG
#endif







/* ================================================================== */
/* ================================================================== */
/*            INITIALIZE LIBRARY                                      */
/* ================================================================== */
/* ================================================================== */

// Module initialization macro in CLIcore.h
// macro argument defines module name for bindings
//
INIT_MODULE_LIB(COREMOD_iofits)



/* ================================================================== */
/* ================================================================== */
/*            COMMAND LINE INTERFACE (CLI) FUNCTIONS                  */
/* ================================================================== */
/* ================================================================== */



errno_t load_fits_cli()
{
    if(
        CLI_checkarg(1, CLIARG_STR) +
        CLI_checkarg(2, CLIARG_STR_NOT_IMG)
        == 0)
    {
        load_fits(
            data.cmdargtoken[1].val.string,
            data.cmdargtoken[2].val.string,
            0);

        return CLICMD_SUCCESS;
    }
    else
    {
        return CLICMD_INVALID_ARG;
    }

    return CLICMD_SUCCESS;
}


errno_t save_fl_fits_cli()
{
    char fname[STRINGMAXLEN_FILENAME];

    switch(data.cmdNBarg)
    {
        case 3:
            save_fl_fits(data.cmdargtoken[1].val.string,  data.cmdargtoken[2].val.string);
            break;
        case 2:
            WRITE_FILENAME(fname, "%s.fits", data.cmdargtoken[1].val.string);
            save_fl_fits(data.cmdargtoken[1].val.string, fname);
            break;
    }
    return CLICMD_SUCCESS;
}




errno_t save_db_fits_cli()
{
    char fname[STRINGMAXLEN_FILENAME];

    switch(data.cmdNBarg)
    {
        case 3:
            save_db_fits(data.cmdargtoken[1].val.string,  data.cmdargtoken[2].val.string);
            break;
        case 2:
            WRITE_FILENAME(fname, "%s.fits", data.cmdargtoken[1].val.string);
            save_db_fits(data.cmdargtoken[1].val.string, fname);
            break;
    }

    return CLICMD_SUCCESS;
}



errno_t save_sh16_fits_cli()
{
    char fname[STRINGMAXLEN_FILENAME];

    switch(data.cmdNBarg)
    {
        case 3:
            save_sh16_fits(data.cmdargtoken[1].val.string,  data.cmdargtoken[2].val.string);
            break;
        case 2:
            WRITE_FILENAME(fname, "%s.fits", data.cmdargtoken[1].val.string);
            save_sh16_fits(data.cmdargtoken[1].val.string, fname);
            break;
    }

    return CLICMD_SUCCESS;
}




errno_t save_fits_cli()
{
    char fname[STRINGMAXLEN_FILENAME];

    switch(data.cmdNBarg)
    {
        case 3:
            save_fits(data.cmdargtoken[1].val.string,  data.cmdargtoken[2].val.string);
            break;
        case 2:
            WRITE_FILENAME(fname, "%s.fits", data.cmdargtoken[1].val.string);
            save_fits(data.cmdargtoken[1].val.string, fname);
            break;
    }

    return CLICMD_SUCCESS;
}





errno_t break_cube_cli()
{
    if(0
            + CLI_checkarg(1, CLIARG_IMG)
            == 0)
    {
        break_cube(data.cmdargtoken[1].val.string);

        return CLICMD_SUCCESS;
    }
    else
    {
        return CLICMD_INVALID_ARG;
    }

    return CLICMD_SUCCESS;

    break_cube(data.cmdargtoken[1].val.string);

    return CLICMD_SUCCESS;
}



errno_t images_to_cube_cli()
{
    /*  if(data.cmdargtoken[1].type != 4)
      {
        printf("Image %s does not exist\n", data.cmdargtoken[1].val.string);
        return -1;
        }*/

    if(data.cmdargtoken[2].type != 2)
    {
        printf("second argument has to be integer\n");
        return -1;
    }

    images_to_cube(data.cmdargtoken[1].val.string, data.cmdargtoken[2].val.numl,
                   data.cmdargtoken[3].val.string);

    return CLICMD_SUCCESS;
}









static errno_t init_module_CLI()
{


    /* =============================================================================================== */
    /*                                                                                                 */
    /* 1. LOAD / SAVE                                                                                  */
    /*                                                                                                 */
    /* =============================================================================================== */

    RegisterCLIcommand(
        "loadfits",
        __FILE__,
        load_fits_cli,
        "load FITS format file",
        "input output",
        "loadfits im.fits im",
        "long load_fits()"
    );


    RegisterCLIcommand(
        "saveflfits",
        __FILE__,
        save_fl_fits_cli,
        "save FITS format file, float",
        "input output",
        "saveflfits im im.fits",
        "int save_fl_fits(char *ID_name, char *file_name)"
    );


    RegisterCLIcommand(
        "savedbfits",
        __FILE__,
        save_db_fits_cli,
        "save FITS format file, double",
        "input output",
        "savedbfits im im.fits",
        "int save_db_fits(char *ID_name, char *file_name)"
    );

    RegisterCLIcommand(
        "saveshfits",
        __FILE__,
        save_sh16_fits_cli,
        "save FITS format file, short",
        "input output",
        "saveshfits im im.fits",
        "int save_sh16_fits(char *ID_name, char *file_name)"
    );

    RegisterCLIcommand(
        "savefits",
        __FILE__,
        save_fits_cli,
        "save FITS format file",
        "input output",
        "savefits im im.fits",
        "int save_fits(char *ID_name, char *file_name)"
    );

    /* =============================================================================================== */
    /*                                                                                                 */
    /* 2. CUBES                                                                                        */
    /*                                                                                                 */
    /* =============================================================================================== */

    RegisterCLIcommand(
        "breakcube",
        __FILE__,
        break_cube_cli,
        "break cube into individual images (slices)",
        "<input image>",
        "breakcube imc",
        "int break_cube(char *ID_name)"
    );

    RegisterCLIcommand(
        "imgs2cube",
        __FILE__,
        images_to_cube_cli,
        "combine individual images into cube, image name is prefix followed by 5 digits",
        "<input image format> <max index> <output cube>", "imgs2cube im_ 100 imc",
        "int images_to_cube(char *img_name, long nbframes, char *cube_name)"
    );


    // add atexit functions here

    return RETURN_SUCCESS;
}



















static int FITSIO_status = 0;

// set print to 0 if error message should not be printed to stderr
// set print to 1 if error message should be printed to stderr
int check_FITSIO_status(
    const char *restrict cfile,
    const char *restrict cfunc,
    long cline,
    int print)
{
    int Ferr = 0;

    if(FITSIO_status != 0)
    {
        if(print == 1)
        {
            char errstr[SBUFFERSIZE];
            fits_get_errstatus(FITSIO_status, errstr);
            fprintf(stderr,
                    "%c[%d;%dmFITSIO error %d [%s, %s, %ld]: %s%c[%d;m\n\a",
                    (char) 27, 1, 31, FITSIO_status, cfile, cfunc, cline, errstr, (char) 27, 0);
        }
        Ferr = FITSIO_status;
    }
    FITSIO_status = 0;

    return(Ferr);
}



int file_exists(
    const char *restrict file_name
)
{
    FILE *fp;
    int exists = 1;

    if((fp = fopen(file_name, "r")) == NULL)
    {
        exists = 0;
        /*      printf("file %s does not exist\n",file_name);*/
    }
    else
    {
        fclose(fp);
    }

    return(exists);
}





int is_fits_file(
    const char *restrict file_name
)
{
    int value = 0;
    fitsfile *fptr;


    if(!fits_open_file(&fptr, file_name, READONLY, &FITSIO_status))
    {
        fits_close_file(fptr, &FITSIO_status);
        value = 1;
    }
    if(check_FITSIO_status(__FILE__, __func__, __LINE__, 1) == 1)
    {
        PRINT_ERROR("Error in function is_fits_file(%s)", file_name);
    }

    return(value);
}




int read_keyword(
    const char *restrict file_name,
    const char *restrict KEYWORD,
    char *restrict content
)
{
    fitsfile *fptr;         /* FITS file pointer, defined in fitsio.h */
    int exists = 0;
    int n;

    if(!fits_open_file(&fptr, file_name, READONLY, &FITSIO_status))
    {
        char comment[SBUFFERSIZE];
        char str1[SBUFFERSIZE];

        if(fits_read_keyword(fptr, KEYWORD, str1, comment, &FITSIO_status))
        {
            PRINT_ERROR("Keyword \"%s\" does not exist in file \"%s\"", KEYWORD, file_name);
            exists = 1;
        }
        else
        {
            n = snprintf(content, SBUFFERSIZE, "%s\n", str1);
            if(n >= SBUFFERSIZE)
            {
                PRINT_ERROR("Attempted to write string buffer with too many characters");
            }
        }
        fits_close_file(fptr, &FITSIO_status);
    }
    if(check_FITSIO_status(__FILE__, __func__, __LINE__, 0) == 1)
    {
        PRINT_ERROR("Error reading keyword \"%s\" in file \"%s\"", KEYWORD, file_name);       
    }

    return(exists);
}




errno_t read_keyword_alone(
    const char *restrict file_name,
    const char *restrict KEYWORD
)
{
    char *content = (char *) malloc(sizeof(char) * SBUFFERSIZE);

    if(content == NULL)
    {
        PRINT_ERROR("malloc error");
        exit(0);
    }

    read_keyword(file_name, KEYWORD, content);
    printf("%s\n", content);

    free(content);
    content = NULL;

    return RETURN_SUCCESS;
}





int data_type_code(
    int bitpix
)
{
    int code;
    /*
      bitpix      Datatype             typecode    Mnemonic
      1           bit, X                   1        TBIT
      8           byte, B                 11        TBYTE
                  logical, L              14        TLOGICAL
                  ASCII character, A      16        TSTRING
      16          short integer, I        21        TSHORT
      32          integer, J              41        TLONG
     -32          real, E                 42        TFLOAT
     -64          double precision, D     82        TDOUBLE
                  complex, C              83        TCOMPLEX
                  double complex, M      163        TDBLCOMPLEX
                  */
    code = 0;
    if(bitpix == 1)
    {
        code = 1;
    }
    if(bitpix == 8)
    {
        code = 11;
    }
    if(bitpix == 16)
    {
        code = 21;
    }
    if(bitpix == 32)
    {
        code = 41;
    }
    if(bitpix == -32)
    {
        code = 42;
    }
    if(bitpix == -64)
    {
        code = 82;
    }
    return(code);
}












/* =============================================================================================== */
/*                                                                                                 */
/* 1. LOAD / SAVE                                                                                  */
/*                                                                                                 */
/* =============================================================================================== */




/// if errcode = 0, do not show error messages
/// errcode = 1: print error, continue
/// errcode = 2: exit program at error
/// errcode = 3: do not show error message, try = 1, no wait
imageID load_fits(
    const char *restrict file_name,
    const char *restrict ID_name,
    int         errcode
)
{
    fitsfile *fptr = NULL;       /* pointer to the FITS file; defined in fitsio.h */
    int       nulval, anynul;
    long      bitpixl = 0;
    long      naxis = 0;
    uint32_t  naxes[3];
    imageID   ID;
    double    bscale;
    double    bzero;
    unsigned char *barray = NULL;
    long     *larray = NULL;
    //    unsigned short *sarray = NULL;
    //    long      NDR = 1; /* non-destructive reads */


    int fileOK;
    int PrintErrorMsg = 1;
    int ExitOnErr = 0;

    nulval = 0;
    anynul = 0;
    bscale = 1;
    bzero = 0;

    naxes[0] = 0;
    naxes[1] = 0;
    naxes[2] = 0;




    fileOK = 0;
    int NBtry = 3;

    if(errcode == 0)
    {
        PrintErrorMsg = 0;
        ExitOnErr = 0;
    }

    if(errcode == 1)
    {
        PrintErrorMsg = 1;
        ExitOnErr = 0;
    }

    if(errcode == 2)
    {
        PrintErrorMsg = 1;
        ExitOnErr = 1;
    }

    if(errcode == 3)
    {
        NBtry = 1;
        PrintErrorMsg = 0;
        ExitOnErr = 0;
    }



    for(int try = 0;
                try < NBtry;
                    try++)
                {
                    if(fileOK == 0)
                    {
                        if(fits_open_file(&fptr, file_name, READONLY, &FITSIO_status))
                        {
                            if(check_FITSIO_status(__FILE__, __func__, __LINE__, PrintErrorMsg) != 0)
                            {
                                if(ExitOnErr == 1)
                                {
                                    exit(0);
                                }
                                if(try != NBtry - 1) // don't wait on last try
                                        usleep(10000);
                            }
                            ID = -1;
                        }
                        else
                        {
                            fileOK = 1;
                        }
                    }
                }

    if(fileOK == 0)
    {
        if(PrintErrorMsg == 1)
        {
            fprintf(stderr, "%c[%d;%dm Error while calling \"fits_open_file\" %c[%d;m\n",
                    (char) 27, 1, 31, (char) 27, 0);
            fprintf(stderr, "%c[%d;%dm within load_fits ( %s, %s ) %c[%d;m\n", (char) 27, 1,
                    31, ID_name, file_name, (char) 27, 0);
            fprintf(stderr, "%c[%d;%dm Printing Cfits image buffer content: %c[%d;m\n",
                    (char) 27, 1, 31, (char) 27, 0);
            list_image_ID();
        }
        else
        {
            PRINT_WARNING("Image \"%s\" could not be loaded from file \"%s\"", ID_name,
                          file_name);
        }

    }


    if(fileOK == 1)
    {
        char keyword[SBUFFERSIZE];
        long  fpixel = 1;
        long i;
        long ii;
        char comment[SBUFFERSIZE];
        long  nelements;

        fits_read_key(fptr, TLONG, "NAXIS", &naxis, comment, &FITSIO_status);
        if(errcode != 0)
        {
            if(check_FITSIO_status(__FILE__, __func__, __LINE__, 1) != 0)
            {
                fprintf(stderr,
                        "%c[%d;%dm Error while calling \"fits_read_key\" NAXIS %c[%d;m\n", (char) 27, 1,
                        31, (char) 27, 0);
                fprintf(stderr, "%c[%d;%dm within load_fits ( %s, %s ) %c[%d;m\n", (char) 27, 1,
                        31, ID_name, file_name, (char) 27, 0);
                fprintf(stderr, "%c[%d;%dm Printing Cfits image buffer content: %c[%d;m\n",
                        (char) 27, 1, 31, (char) 27, 0);
                list_image_ID();
                if(errcode > 1)
                {
                    exit(0);
                }
            }
        }


        for(i = 0; i < naxis; i++)
        {
            int n;

            n = snprintf(keyword, SBUFFERSIZE, "NAXIS%ld", i + 1);
            if(n >= SBUFFERSIZE)
            {
                PRINT_ERROR("Attempted to write string buffer with too many characters");
            }
            fits_read_key(fptr, TLONG, keyword, &naxes[i], comment, &FITSIO_status);
            if(check_FITSIO_status(__FILE__, __func__, __LINE__, 1) != 0)
            {
                if(errcode != 0)
                {
                    fprintf(stderr,
                            "%c[%d;%dm Error while calling \"fits_read_key\" NAXIS%ld %c[%d;m\n", (char) 27,
                            1, 31, i, (char) 27, 0);
                    fprintf(stderr, "%c[%d;%dm within load_fits ( %s, %s ) %c[%d;m\n", (char) 27, 1,
                            31, ID_name, file_name, (char) 27, 0);
                    fprintf(stderr, "%c[%d;%dm Printing Cfits image buffer content: %c[%d;m\n",
                            (char) 27, 1, 31, (char) 27, 0);
                    list_image_ID();
                    if(errcode > 1)
                    {
                        exit(0);
                    }
                }
            }
        }

        fits_read_key(fptr, TLONG, "BITPIX", &bitpixl, comment, &FITSIO_status);
        if(check_FITSIO_status(__FILE__, __func__, __LINE__, 1) != 0)
        {
            if(errcode != 0)
            {
                fprintf(stderr,
                        "%c[%d;%dm Error while calling \"fits_read_key\" BITPIX %c[%d;m\n", (char) 27,
                        1, 31, (char) 27, 0);
                fprintf(stderr, "%c[%d;%dm within load_fits ( %s, %s ) %c[%d;m\n", (char) 27, 1,
                        31, ID_name, file_name, (char) 27, 0);
                fprintf(stderr, "%c[%d;%dm Printing Cfits image buffer content: %c[%d;m\n",
                        (char) 27, 1, 31, (char) 27, 0);
                list_image_ID();
                if(errcode > 1)
                {
                    exit(0);
                }
            }
        }



        int bitpix = (int) bitpixl;
        fits_read_key(fptr, TDOUBLE, "BSCALE", &bscale, comment, &FITSIO_status);
        if(check_FITSIO_status(__FILE__, __func__, __LINE__, 0) == 1)
        {
            //fprintf(stderr,"Error reading keyword \"BSCALE\" in file \"%s\"\n",file_name);
            bscale = 1.0;
        }
        fits_read_key(fptr, TDOUBLE, "BZERO", &bzero, comment, &FITSIO_status);
        if(check_FITSIO_status(__FILE__, __func__, __LINE__, 0) == 1)
        {
            //fprintf(stderr,"Error reading keyword \"BZERO\" in file \"%s\"\n",file_name);
            bzero = 0.0;
        }

        fits_set_bscale(fptr, bscale, bzero, &FITSIO_status);
        check_FITSIO_status(__FILE__, __func__, __LINE__, 1);

        if(1)
        {
            printf("[%ld", (long) naxes[0]);
            for(i = 1; i < naxis; i++)
            {
                printf(",%ld", (long) naxes[i]);
            }
            printf("] %d %f %f\n", bitpix, bscale, bzero);
            fflush(stdout);
        }

        nelements = 1;
        for(i = 0; i < naxis; i++)
        {
            nelements *= naxes[i];
        }

        /* bitpix = -32  TFLOAT */
        if(bitpix == -32)
        {
            ID = create_image_ID(ID_name, naxis, naxes, _DATATYPE_FLOAT, data.SHARED_DFT,
                                 data.NBKEWORD_DFT);
            fits_read_img(fptr, data_type_code(bitpix), fpixel, nelements, &nulval,
                          data.image[ID].array.F, &anynul, &FITSIO_status);

            if(check_FITSIO_status(__FILE__, __func__, __LINE__, 1) != 0)
            {
                if(errcode != 0)
                {
                    fprintf(stderr, "%c[%d;%dm Error while calling \"fits_read_img\" %c[%d;m\n",
                            (char) 27, 1, 31, (char) 27, 0);
                    fprintf(stderr, "%c[%d;%dm within load_fits ( %s, %s ) %c[%d;m\n", (char) 27, 1,
                            31, ID_name, file_name, (char) 27, 0);
                    fprintf(stderr, "%c[%d;%dm Printing Cfits image buffer content: %c[%d;m\n",
                            (char) 27, 1, 31, (char) 27, 0);
                    list_image_ID();
                    if(errcode > 1)
                    {
                        exit(0);
                    }
                }
            }



            fits_close_file(fptr, &FITSIO_status);
            if(check_FITSIO_status(__FILE__, __func__, __LINE__, 1) != 0)
            {
                if(errcode != 0)
                {
                    fprintf(stderr, "%c[%d;%dm Error while calling \"fits_close_file\" %c[%d;m\n",
                            (char) 27, 1, 31, (char) 27, 0);
                    fprintf(stderr, "%c[%d;%dm within load_fits ( %s, %s ) %c[%d;m\n", (char) 27, 1,
                            31, ID_name, file_name, (char) 27, 0);
                    fprintf(stderr, "%c[%d;%dm Printing Cfits image buffer content: %c[%d;m\n",
                            (char) 27, 1, 31, (char) 27, 0);
                    list_image_ID();
                    if(errcode > 1)
                    {
                        exit(0);
                    }
                }
            }

            check_FITSIO_status(__FILE__, __func__, __LINE__, 1);
        }

        /* bitpix = -64  TDOUBLE */
        if(bitpix == -64)
        {
            ID = create_image_ID(ID_name, naxis, naxes, _DATATYPE_DOUBLE, data.SHARED_DFT,
                                 data.NBKEWORD_DFT);

            fits_read_img(fptr, data_type_code(bitpix), fpixel, nelements, &nulval,
                          data.image[ID].array.D, &anynul, &FITSIO_status);
            if(check_FITSIO_status(__FILE__, __func__, __LINE__, 1) != 0)
            {
                if(errcode != 0)
                {
                    fprintf(stderr, "%c[%d;%dm Error while calling \"fits_read_img\" %c[%d;m\n",
                            (char) 27, 1, 31, (char) 27, 0);
                    fprintf(stderr, "%c[%d;%dm within load_fits ( %s, %s ) %c[%d;m\n", (char) 27, 1,
                            31, ID_name, file_name, (char) 27, 0);
                    fprintf(stderr, "%c[%d;%dm Printing Cfits image buffer content: %c[%d;m\n",
                            (char) 27, 1, 31, (char) 27, 0);
                    list_image_ID();
                    if(errcode > 1)
                    {
                        exit(0);
                    }
                }
            }

            fits_close_file(fptr, &FITSIO_status);
            if(check_FITSIO_status(__FILE__, __func__, __LINE__, 1) != 0)
            {
                if(errcode != 0)
                {
                    fprintf(stderr, "%c[%d;%dm Error while calling \"fits_close_file\" %c[%d;m\n",
                            (char) 27, 1, 31, (char) 27, 0);
                    fprintf(stderr, "%c[%d;%dm within load_fits ( %s, %s ) %c[%d;m\n", (char) 27, 1,
                            31, ID_name, file_name, (char) 27, 0);
                    fprintf(stderr, "%c[%d;%dm Printing Cfits image buffer content: %c[%d;m\n",
                            (char) 27, 1, 31, (char) 27, 0);
                    list_image_ID();
                    if(errcode > 1)
                    {
                        exit(0);
                    }
                }
            }

            check_FITSIO_status(__FILE__, __func__, __LINE__, 1);
        }

        /* bitpix = 16   TSHORT */
        if(bitpix == 16)
        {
            // ID = create_image_ID(ID_name, naxis, naxes, Dtype, data.SHARED_DFT, data.NBKEWORD_DFT);
            ID = create_image_ID(ID_name, naxis, naxes, _DATATYPE_UINT16, data.SHARED_DFT,
                                 data.NBKEWORD_DFT);

            //           fits_read_img(fptr, 20, fpixel, nelements, &nulval, sarray, &anynul, &FITSIO_status);
            fits_read_img(fptr, 20, fpixel, nelements, &nulval, data.image[ID].array.UI16,
                          &anynul, &FITSIO_status);
            if(check_FITSIO_status(__FILE__, __func__, __LINE__, 1) != 0)
            {
                if(errcode != 0)
                {
                    fprintf(stderr, "%c[%d;%dm Error while calling \"fits_read_img\" %c[%d;m\n",
                            (char) 27, 1, 31, (char) 27, 0);
                    fprintf(stderr, "%c[%d;%dm within load_fits ( %s, %s ) %c[%d;m\n", (char) 27, 1,
                            31, ID_name, file_name, (char) 27, 0);
                    fprintf(stderr, "%c[%d;%dm Printing Cfits image buffer content: %c[%d;m\n",
                            (char) 27, 1, 31, (char) 27, 0);
                    list_image_ID();
                    if(errcode > 1)
                    {
                        exit(0);
                    }
                }
            }

            fits_close_file(fptr, &FITSIO_status);
            if(check_FITSIO_status(__FILE__, __func__, __LINE__, 1) != 0)
            {
                if(errcode != 0)
                {
                    fprintf(stderr, "%c[%d;%dm Error while calling \"fits_close_file\" %c[%d;m\n",
                            (char) 27, 1, 31, (char) 27, 0);
                    fprintf(stderr, "%c[%d;%dm within load_fits ( %s, %s ) %c[%d;m\n", (char) 27, 1,
                            31, ID_name, file_name, (char) 27, 0);
                    fprintf(stderr, "%c[%d;%dm Printing Cfits image buffer content: %c[%d;m\n",
                            (char) 27, 1, 31, (char) 27, 0);
                    list_image_ID();
                    if(errcode > 1)
                    {
                        exit(0);
                    }
                }
            }

            check_FITSIO_status(__FILE__, __func__, __LINE__, 1);
            /*        for (ii = 0; ii < nelements; ii++)
                        data.image[ID].array.F[ii] = 1.0*sarray[ii];
                    free(sarray);
                    sarray = NULL;*/
        }


        /* bitpix = 32   TLONG */
        if(bitpix == 32)
        {
            /*fits_read_key(fptr, TLONG, "NDR", &NDR, comment, &FITSIO_status);
            if(check_FITSIO_status(__FILE__, __func__, __LINE__, 0) == 1) {
                NDR = 1;
            }*/
            ID = create_image_ID(ID_name, naxis, naxes, _DATATYPE_INT32, data.SHARED_DFT,
                                 data.NBKEWORD_DFT);
            larray = (long *) malloc(sizeof(long) * nelements);
            if(larray == NULL)
            {
                PRINT_ERROR("malloc error");
                exit(0);
            }

            fits_read_img(fptr, data_type_code(bitpix), fpixel, nelements, &nulval, larray,
                          &anynul, &FITSIO_status);
            if(check_FITSIO_status(__FILE__, __func__, __LINE__, 1) != 0)
            {
                if(errcode != 0)
                {
                    fprintf(stderr, "%c[%d;%dm Error while calling \"fits_read_img\" %c[%d;m\n",
                            (char) 27, 1, 31, (char) 27, 0);
                    fprintf(stderr, "%c[%d;%dm within load_fits ( %s, %s ) %c[%d;m\n", (char) 27, 1,
                            31, ID_name, file_name, (char) 27, 0);
                    fprintf(stderr, "%c[%d;%dm Printing Cfits image buffer content: %c[%d;m\n",
                            (char) 27, 1, 31, (char) 27, 0);
                    list_image_ID();
                    if(errcode > 1)
                    {
                        exit(0);
                    }
                }
            }

            fits_close_file(fptr, &FITSIO_status);
            if(check_FITSIO_status(__FILE__, __func__, __LINE__, 1) != 0)
            {
                if(errcode != 0)
                {
                    fprintf(stderr, "%c[%d;%dm Error while calling \"fits_close_file\" %c[%d;m\n",
                            (char) 27, 1, 31, (char) 27, 0);
                    fprintf(stderr, "%c[%d;%dm within load_fits ( %s, %s ) %c[%d;m\n", (char) 27, 1,
                            31, ID_name, file_name, (char) 27, 0);
                    fprintf(stderr, "%c[%d;%dm Printing Cfits image buffer content: %c[%d;m\n",
                            (char) 27, 1, 31, (char) 27, 0);
                    list_image_ID();
                    if(errcode > 1)
                    {
                        exit(0);
                    }
                }
            }

            bzero = 0.0;
            for(ii = 0; ii < nelements; ii++)
            {
                data.image[ID].array.SI32[ii] = larray[ii] * bscale + bzero;
            }
            free(larray);
            larray = NULL;
        }

        /* bitpix = 8   TBYTE */
        if(bitpix == 8)
        {
            ID = create_image_ID(ID_name, naxis, naxes, Dtype, data.SHARED_DFT,
                                 data.NBKEWORD_DFT);
            barray = (unsigned char *) malloc(sizeof(unsigned char) * naxes[1] * naxes[0]);
            if(barray == NULL)
            {
                PRINT_ERROR("malloc error");
                exit(0);
            }

            fits_read_img(fptr, data_type_code(bitpix), fpixel, nelements, &nulval, barray,
                          &anynul, &FITSIO_status);
            if(check_FITSIO_status(__FILE__, __func__, __LINE__, 1) != 0)
            {
                if(errcode != 0)
                {
                    fprintf(stderr, "%c[%d;%dm Error while calling \"fits_read_img\" %c[%d;m\n",
                            (char) 27, 1, 31, (char) 27, 0);
                    fprintf(stderr, "%c[%d;%dm within load_fits ( %s, %s ) %c[%d;m\n", (char) 27, 1,
                            31, ID_name, file_name, (char) 27, 0);
                    fprintf(stderr, "%c[%d;%dm Printing Cfits image buffer content: %c[%d;m\n",
                            (char) 27, 1, 31, (char) 27, 0);
                    list_image_ID();
                    if(errcode > 1)
                    {
                        exit(0);
                    }
                }
            }

            fits_close_file(fptr, &FITSIO_status);
            if(check_FITSIO_status(__FILE__, __func__, __LINE__, 1) != 0)
            {
                if(errcode != 0)
                {
                    fprintf(stderr, "%c[%d;%dm Error while calling \"fits_close_file\" %c[%d;m\n",
                            (char) 27, 1, 31, (char) 27, 0);
                    fprintf(stderr, "%c[%d;%dm within load_fits ( %s, %s ) %c[%d;m\n", (char) 27, 1,
                            31, ID_name, file_name, (char) 27, 0);
                    fprintf(stderr, "%c[%d;%dm Printing Cfits image buffer content: %c[%d;m\n",
                            (char) 27, 1, 31, (char) 27, 0);
                    list_image_ID();
                    if(errcode > 1)
                    {
                        exit(0);
                    }
                }
            }


            for(ii = 0; ii < nelements; ii++)
            {
                data.image[ID].array.F[ii] = (1.0 * barray[ii] * bscale + bzero);
            }
            free(barray);
            barray = NULL;
        }
    }

    return(ID);
}






/* saves an image in a double format */

errno_t save_db_fits(
    const char *restrict ID_name,
    const char *restrict file_name
)
{
    fitsfile *fptr;
    long nelements;
    imageID ID;
    char file_name1[STRINGMAXLEN_FULLFILENAME];

    if((data.overwrite == 1) && (file_name[0] != '!')
            && (file_exists(file_name) == 1))
    {
        PRINT_WARNING("automatic overwrite on file \"%s\"\n", file_name);               
        WRITE_FULLFILENAME(file_name1, "!%s", file_name);       
    }
    else
    {
		WRITE_FULLFILENAME(file_name1, "!%s", file_name);       
    }

    ID = image_ID(ID_name);

    if(ID != -1)
    {
        long  fpixel = 1;
        uint32_t naxes[3];
        long naxesl[3];
        double *array;
        uint8_t datatype;
        long naxis;
        long i;


        datatype = data.image[ID].md[0].datatype;
        naxis = data.image[ID].md[0].naxis;
        for(i = 0; i < naxis; i++)
        {
            naxes[i] = data.image[ID].md[0].size[i];
            naxesl[i] = (long) naxes[i];
        }



        nelements = 1;
        for(i = 0; i < naxis; i++)
        {
            nelements *= naxes[i];
        }


        if(datatype != _DATATYPE_DOUBLE)   // data conversion required
        {
            long ii;

            array = (double *) malloc(SIZEOF_DATATYPE_DOUBLE * nelements);
            if(array == NULL)
            {
                PRINT_ERROR("malloc error");
                exit(0);
            }

            switch(datatype)
            {
                case _DATATYPE_UINT8 :
                    for(ii = 0; ii < nelements; ii++)
                    {
                        array[ii] = (double) data.image[ID].array.UI8[ii];
                    }
                    break;

                case _DATATYPE_INT8 :
                    for(ii = 0; ii < nelements; ii++)
                    {
                        array[ii] = (double) data.image[ID].array.SI8[ii];
                    }
                    break;

                case _DATATYPE_UINT16 :
                    for(ii = 0; ii < nelements; ii++)
                    {
                        array[ii] = (double) data.image[ID].array.UI16[ii];
                    }
                    break;

                case _DATATYPE_INT16 :
                    for(ii = 0; ii < nelements; ii++)
                    {
                        array[ii] = (double) data.image[ID].array.SI16[ii];
                    }
                    break;

                case _DATATYPE_UINT32 :
                    for(ii = 0; ii < nelements; ii++)
                    {
                        array[ii] = (double) data.image[ID].array.UI32[ii];
                    }
                    break;

                case _DATATYPE_INT32 :
                    for(ii = 0; ii < nelements; ii++)
                    {
                        array[ii] = (double) data.image[ID].array.SI32[ii];
                    }
                    break;

                case _DATATYPE_UINT64 :
                    for(ii = 0; ii < nelements; ii++)
                    {
                        array[ii] = (double) data.image[ID].array.UI64[ii];
                    }
                    break;

                case _DATATYPE_INT64 :
                    for(ii = 0; ii < nelements; ii++)
                    {
                        array[ii] = (double) data.image[ID].array.SI64[ii];
                    }
                    break;

                case _DATATYPE_FLOAT :
                    for(ii = 0; ii < nelements; ii++)
                    {
                        array[ii] = (double) data.image[ID].array.F[ii];
                    }
                    break;

                default :
                    list_image_ID();
                    PRINT_ERROR("atype value not recognised");
                    printf("ID %ld  datatype = %d\n", ID, datatype);
                    free(array);
                    exit(0);
                    break;
            }
        }



        fits_create_file(&fptr, file_name1, &FITSIO_status);
        if(check_FITSIO_status(__FILE__, __func__, __LINE__, 1) != 0)
        {
            fprintf(stderr, "%c[%d;%dm Error while calling \"fits_create_file\" %c[%d;m\n",
                    (char) 27, 1, 31, (char) 27, 0);
            fprintf(stderr, "%c[%d;%dm within save_db_fits ( %s, %s ) %c[%d;m\n", (char) 27,
                    1, 31, ID_name, file_name, (char) 27, 0);
            fprintf(stderr, "%c[%d;%dm Printing Cfits image buffer content: %c[%d;m\n",
                    (char) 27, 1, 31, (char) 27, 0);
            list_image_ID();
        }


        fits_create_img(fptr, DOUBLE_IMG, naxis, naxesl, &FITSIO_status);
        if(check_FITSIO_status(__FILE__, __func__, __LINE__, 1) != 0)
        {
            fprintf(stderr, "%c[%d;%dm Error while calling \"fits_create_img\" %c[%d;m\n",
                    (char) 27, 1, 31, (char) 27, 0);
            fprintf(stderr, "%c[%d;%dm within save_db_fits ( %s, %s ) %c[%d;m\n", (char) 27,
                    1, 31, ID_name, file_name, (char) 27, 0);
            fprintf(stderr, "%c[%d;%dm Printing Cfits image buffer content: %c[%d;m\n",
                    (char) 27, 1, 31, (char) 27, 0);
            list_image_ID();
        }

        if(datatype == _DATATYPE_DOUBLE)
        {
            fits_write_img(fptr, TDOUBLE, fpixel, nelements, data.image[ID].array.D,
                           &FITSIO_status);
        }
        else
        {
            fits_write_img(fptr, TDOUBLE, fpixel, nelements, array, &FITSIO_status);
            free(array);
            array = NULL;
        }

        if(check_FITSIO_status(__FILE__, __func__, __LINE__, 1) != 0)
        {
            fprintf(stderr, "%c[%d;%dm Error while calling \"fits_write_img\" %c[%d;m\n",
                    (char) 27, 1, 31, (char) 27, 0);
            fprintf(stderr, "%c[%d;%dm within save_db_fits ( %s, %s ) %c[%d;m\n", (char) 27,
                    1, 31, ID_name, file_name, (char) 27, 0);
            fprintf(stderr, "%c[%d;%dm Printing Cfits image buffer content: %c[%d;m\n",
                    (char) 27, 1, 31, (char) 27, 0);
            list_image_ID();
        }

        fits_close_file(fptr, &FITSIO_status);
        if(check_FITSIO_status(__FILE__, __func__, __LINE__, 1) != 0)
        {
            fprintf(stderr, "%c[%d;%dm Error while calling \"fits_close_file\" %c[%d;m\n",
                    (char) 27, 1, 31, (char) 27, 0);
            fprintf(stderr, "%c[%d;%dm within save_db_fits ( %s, %s ) %c[%d;m\n", (char) 27,
                    1, 31, ID_name, file_name, (char) 27, 0);
            fprintf(stderr, "%c[%d;%dm Printing Cfits image buffer content: %c[%d;m\n",
                    (char) 27, 1, 31, (char) 27, 0);
            list_image_ID();
        }
    }
    else
    {
        printf("image does not exist in memory\n");
    }

    return RETURN_SUCCESS;
}





/* saves an image in a float format */

errno_t save_fl_fits(
    const char *restrict ID_name,
    const char *restrict file_name
)
{
    fitsfile *fptr;
    long      naxis, nelements;
    float    *array = NULL;
    imageID   ID;
    long      ii;
    long      i;
    char      file_name1[STRINGMAXLEN_FULLFILENAME];

    if((data.overwrite == 1) && (file_name[0] != '!')
            && (file_exists(file_name) == 1))
    {		
        PRINT_WARNING("automatic overwrite on file \"%s\"\n", file_name);
        WRITE_FULLFILENAME(file_name1, "!%s", file_name);      
    }
    else
    {
		WRITE_FULLFILENAME(file_name1, "%s", file_name);
    }

    ID = image_ID(ID_name);

    if(ID != -1)
    {
        uint8_t datatype;
        long naxesl[3];
        uint32_t naxes[3];
        long  fpixel = 1;

        datatype = data.image[ID].md[0].datatype;
        naxis = data.image[ID].md[0].naxis;
        for(i = 0; i < naxis; i++)
        {
            naxes[i] = data.image[ID].md[0].size[i];
            naxesl[i] = (long) naxes[i];
        }

        nelements = 1;
        for(i = 0; i < naxis; i++)
        {
            nelements *= naxes[i];
        }


        if(datatype != _DATATYPE_FLOAT)   // data conversion required
        {
            array = (float *) malloc(SIZEOF_DATATYPE_FLOAT * nelements);
            if(array == NULL)
            {
                PRINT_ERROR("malloc error");
                exit(0);
            }

            switch(datatype)
            {
                case _DATATYPE_UINT8 :
                    for(ii = 0; ii < nelements; ii++)
                    {
                        array[ii] = (float) data.image[ID].array.UI8[ii];
                    }
                    break;

                case _DATATYPE_INT8 :
                    for(ii = 0; ii < nelements; ii++)
                    {
                        array[ii] = (float) data.image[ID].array.SI8[ii];
                    }
                    break;

                case _DATATYPE_UINT16 :
                    for(ii = 0; ii < nelements; ii++)
                    {
                        array[ii] = (float) data.image[ID].array.UI16[ii];
                    }
                    break;

                case _DATATYPE_INT16 :
                    for(ii = 0; ii < nelements; ii++)
                    {
                        array[ii] = (float) data.image[ID].array.SI16[ii];
                    }
                    break;

                case _DATATYPE_UINT32 :
                    for(ii = 0; ii < nelements; ii++)
                    {
                        array[ii] = (float) data.image[ID].array.UI32[ii];
                    }
                    break;

                case _DATATYPE_INT32 :
                    for(ii = 0; ii < nelements; ii++)
                    {
                        array[ii] = (float) data.image[ID].array.SI32[ii];
                    }
                    break;

                case _DATATYPE_UINT64 :
                    for(ii = 0; ii < nelements; ii++)
                    {
                        array[ii] = (float) data.image[ID].array.UI64[ii];
                    }
                    break;

                case _DATATYPE_INT64 :
                    for(ii = 0; ii < nelements; ii++)
                    {
                        array[ii] = (float) data.image[ID].array.SI64[ii];
                    }
                    break;

                case _DATATYPE_DOUBLE :
                    for(ii = 0; ii < nelements; ii++)
                    {
                        array[ii] = (float) data.image[ID].array.D[ii];
                    }
                    break;

                default :
                    list_image_ID();
                    PRINT_ERROR("atype value not recognised");
                    printf("ID %ld  datatype = %d\n", ID, datatype);
                    free(array);
                    exit(0);
                    break;
            }
        }


        FITSIO_status = 0;
        fits_create_file(&fptr, file_name1, &FITSIO_status);
        if(check_FITSIO_status(__FILE__, __func__, __LINE__, 1) != 0)
        {
            fprintf(stderr,
                    "%c[%d;%dm Error while calling \"fits_create_file\" with filename \"%s\" %c[%d;m\n",
                    (char) 27, 1, 31, file_name1, (char) 27, 0);
            if(file_exists(file_name1) == 1)
            {
                fprintf(stderr,
                        "%c[%d;%dm File \"%s\" already exists. Make sure you remove this file before attempting to write file with identical name. %c[%d;m\n",
                        (char) 27, 1, 31, file_name1, (char) 27, 0);
                exit(0);
            }
            else
            {
                fprintf(stderr, "%c[%d;%dm Printing Cfits image buffer content: %c[%d;m\n",
                        (char) 27, 1, 31, (char) 27, 0);
                list_image_ID();
            }
        }

        fits_create_img(fptr, FLOAT_IMG, (int) naxis, naxesl, &FITSIO_status);
        if(check_FITSIO_status(__FILE__, __func__, __LINE__, 1) != 0)
        {
            fprintf(stderr, "%c[%d;%dm Error while calling \"fits_create_img\" %c[%d;m\n",
                    (char) 27, 1, 31, (char) 27, 0);
            fprintf(stderr, "%c[%d;%dm within save_fl_fits ( %s, %s ) %c[%d;m\n", (char) 27,
                    1, 31, ID_name, file_name, (char) 27, 0);
            fprintf(stderr, "%c[%d;%dm Printing Cfits image buffer content: %c[%d;m\n",
                    (char) 27, 1, 31, (char) 27, 0);
            list_image_ID();
        }

        if(datatype == _DATATYPE_FLOAT)
        {
            fits_write_img(fptr, TFLOAT, fpixel, nelements, data.image[ID].array.F,
                           &FITSIO_status);
        }
        else
        {
            fits_write_img(fptr, TFLOAT, fpixel, nelements, array, &FITSIO_status);
            free(array);
            array = NULL;
        }

        if(check_FITSIO_status(__FILE__, __func__, __LINE__, 1) != 0)
        {
            fprintf(stderr, "%c[%d;%dm Error while calling \"fits_write_img\" %c[%d;m\n",
                    (char) 27, 1, 31, (char) 27, 0);
            fprintf(stderr, "%c[%d;%dm within save_fl_fits ( %s, %s ) %c[%d;m\n", (char) 27,
                    1, 31, ID_name, file_name, (char) 27, 0);
            fprintf(stderr, "%c[%d;%dm Printing Cfits image buffer content: %c[%d;m\n",
                    (char) 27, 1, 31, (char) 27, 0);
            list_image_ID();
        }

        fits_close_file(fptr, &FITSIO_status);
        if(check_FITSIO_status(__FILE__, __func__, __LINE__, 1) != 0)
        {
            fprintf(stderr, "%c[%d;%dm Error while calling \"fits_close_file\" %c[%d;m\n",
                    (char) 27, 1, 31, (char) 27, 0);
            fprintf(stderr, "%c[%d;%dm within save_fl_fits ( %s, %s ) %c[%d;m\n", (char) 27,
                    1, 31, ID_name, file_name, (char) 27, 0);
            fprintf(stderr, "%c[%d;%dm Printing Cfits image buffer content: %c[%d;m\n",
                    (char) 27, 1, 31, (char) 27, 0);
            list_image_ID();
        }
    }
    else
    {
        fprintf(stderr, "%c[%d;%dm image \"%s\" does not exist in memory %c[%d;m\n",
                (char) 27, 1, 31, ID_name, (char) 27, 0);
    }

    return RETURN_SUCCESS;
}





/* saves an image in a short int format */

errno_t save_sh16_fits(
    const char *restrict ID_name,
    const char *restrict file_name
)
{
    fitsfile *fptr;
    long  fpixel = 1, naxis, nelements;
    uint32_t naxes[3];
    long naxesl[3];
    int16_t *array = NULL;
    imageID ID;
    long ii;
    long i;
    uint8_t datatype;
    char file_name1[STRINGMAXLEN_FULLFILENAME];


    if((data.overwrite == 1) && (file_name[0] != '!')
            && (file_exists(file_name) == 1))
    {
        PRINT_WARNING("automatic overwrite on file \"%s\"\n", file_name);
        WRITE_FULLFILENAME(file_name1, "!%s", file_name);        
    }
    else
    {
		WRITE_FULLFILENAME(file_name1, "%s", file_name);
    }

    ID = image_ID(ID_name);

    if(ID != -1)
    {
        datatype = data.image[ID].md[0].datatype;
        naxis = data.image[ID].md[0].naxis;
        for(i = 0; i < naxis; i++)
        {
            naxes[i] = data.image[ID].md[0].size[i];
            naxesl[i] = (long) naxes[i];
        }

        nelements = 1;
        for(i = 0; i < naxis; i++)
        {
            nelements *= naxes[i];
        }

        if(datatype != _DATATYPE_INT16)   // data conversion required
        {

            printf("Data conversion required\n"); //TEST
            fflush(stdout);


            array = (int16_t *) malloc(SIZEOF_DATATYPE_INT16 * nelements);
            if(array == NULL)
            {
                PRINT_ERROR("malloc error");
                exit(0);
            }

            switch(datatype)
            {
                case _DATATYPE_UINT8 :
                    for(ii = 0; ii < nelements; ii++)
                    {
                        array[ii] = (int16_t) data.image[ID].array.UI8[ii];
                    }
                    break;

                case _DATATYPE_INT8 :
                    for(ii = 0; ii < nelements; ii++)
                    {
                        array[ii] = (int16_t) data.image[ID].array.SI8[ii];
                    }
                    break;

                case _DATATYPE_UINT16 :
                    for(ii = 0; ii < nelements; ii++)
                    {
                        array[ii] = (int16_t) data.image[ID].array.UI16[ii];
                    }
                    break;

                case _DATATYPE_UINT32 :
                    for(ii = 0; ii < nelements; ii++)
                    {
                        array[ii] = (int16_t) data.image[ID].array.UI32[ii];
                    }
                    break;

                case _DATATYPE_INT32 :
                    for(ii = 0; ii < nelements; ii++)
                    {
                        array[ii] = (int16_t) data.image[ID].array.SI32[ii];
                    }
                    break;

                case _DATATYPE_UINT64 :
                    for(ii = 0; ii < nelements; ii++)
                    {
                        array[ii] = (int16_t) data.image[ID].array.UI64[ii];
                    }
                    break;

                case _DATATYPE_INT64 :
                    for(ii = 0; ii < nelements; ii++)
                    {
                        array[ii] = (int16_t) data.image[ID].array.SI64[ii];
                    }
                    break;

                case _DATATYPE_FLOAT :
                    for(ii = 0; ii < nelements; ii++)
                    {
                        array[ii] = (int16_t) data.image[ID].array.F[ii];
                    }
                    break;

                case _DATATYPE_DOUBLE :
                    for(ii = 0; ii < nelements; ii++)
                    {
                        array[ii] = (int16_t) data.image[ID].array.D[ii];
                    }
                    break;

                default :
                    list_image_ID();
                    PRINT_ERROR("atype value not recognised");
                    free(array);
                    printf("ID %ld  datatype = %d\n", ID, datatype);
                    exit(0);
                    break;
            }
        }
        else
        {
            printf("No data conversion required\n"); //TEST
            fflush(stdout);
        }


        fits_create_file(&fptr, file_name1, &FITSIO_status);
        if(check_FITSIO_status(__FILE__, __func__, __LINE__, 1) != 0)
        {
            fprintf(stderr,
                    "%c[%d;%dm Error while calling \"fits_create_file\" with filename \"%s\" %c[%d;m\n",
                    (char) 27, 1, 31, file_name1, (char) 27, 0);
            if(file_exists(file_name1) == 1)
            {
                fprintf(stderr,
                        "%c[%d;%dm File \"%s\" already exists. Make sure you remove this file before attempting to write file with identical name. %c[%d;m\n",
                        (char) 27, 1, 31, file_name1, (char) 27, 0);
                exit(0);
            }
            else
            {
                fprintf(stderr,
                        "%c[%d;%dm Printing Cfits image buffer content: %c[%d;m\n",
                        (char) 27, 1, 31, (char) 27, 0);
                list_image_ID();
            }
        }


        //    16          short integer, I        21        TSHORT
        fits_create_img(fptr, SHORT_IMG, naxis, naxesl, &FITSIO_status);
        if(check_FITSIO_status(__FILE__, __func__, __LINE__, 1) != 0)
        {
            fprintf(stderr,
                    "%c[%d;%dm Error while calling \"fits_create_img\" %c[%d;m\n",
                    (char) 27, 1, 31, (char) 27, 0);
            fprintf(stderr,
                    "%c[%d;%dm Printing Cfits image buffer content: %c[%d;m\n",
                    (char) 27, 1, 31, (char) 27, 0);
            list_image_ID();
        }

        if(datatype == _DATATYPE_INT16)
        {
            printf("Direct copy --- \n");
            fflush(stdout);

            fits_write_img(fptr, TSHORT, fpixel, nelements, data.image[ID].array.SI16,
                           &FITSIO_status);
        }
        else
        {
            fits_write_img(fptr, TSHORT, fpixel, nelements, array, &FITSIO_status);
            free(array);
            array = NULL;
        }


        if(check_FITSIO_status(__FILE__, __func__, __LINE__, 1) != 0)
        {
            fprintf(stderr,
                    "%c[%d;%dm Error while calling \"fits_write_img\" %c[%d;m\n",
                    (char) 27, 1, 31, (char) 27, 0);
            fprintf(stderr,
                    "%c[%d;%dm Printing Cfits image buffer content: %c[%d;m\n",
                    (char) 27, 1, 31, (char) 27, 0);
            list_image_ID();
        }

        fits_close_file(fptr, &FITSIO_status);
        if(check_FITSIO_status(__FILE__, __func__, __LINE__, 1) != 0)
        {
            fprintf(stderr,
                    "%c[%d;%dm Error while calling \"fits_close_file\" %c[%d;m\n",
                    (char) 27, 1, 31, (char) 27, 0);
            fprintf(stderr,
                    "%c[%d;%dm Printing Cfits image buffer content: %c[%d;m\n",
                    (char) 27, 1, 31, (char) 27, 0);
            list_image_ID();
        }
    }
    else
    {
        fprintf(stderr,
                "%c[%d;%dm image \"%s\" does not exist in memory %c[%d;m\n",
                (char) 27, 1, 31, ID_name, (char) 27, 0);
    }


    return EXIT_SUCCESS;
}






/* saves an image in a unsigned short int format */

errno_t save_ush16_fits(
    const char *restrict ID_name,
    const char *restrict file_name
)
{
    fitsfile *fptr;
    long  fpixel = 1, naxis, nelements;
    uint32_t naxes[3];
    long naxesl[3];
    uint16_t *array = NULL;
    imageID ID;
    long ii;
    long i;
    uint8_t datatype;
    char file_name1[STRINGMAXLEN_FULLFILENAME];

    if((data.overwrite == 1) && (file_name[0] != '!')
            && (file_exists(file_name) == 1))
    {
		PRINT_WARNING("automatic overwrite on file \"%s\"\n", file_name);
        WRITE_FULLFILENAME(file_name1, "!%s", file_name);
    }
    else
    {
		WRITE_FULLFILENAME(file_name1, "%s", file_name);
    }

    ID = image_ID(ID_name);

    if(ID != -1)
    {
        datatype = data.image[ID].md[0].datatype;
        naxis = data.image[ID].md[0].naxis;
        for(i = 0; i < naxis; i++)
        {
            naxes[i] = data.image[ID].md[0].size[i];
            naxesl[i] = (long) naxes[i]; // CFITSIO expects long int *
        }

        nelements = 1;
        for(i = 0; i < naxis; i++)
        {
            nelements *= naxes[i];
        }

        if(datatype != _DATATYPE_UINT16)   // data conversion required
        {
            array = (uint16_t *) malloc(SIZEOF_DATATYPE_UINT16 * nelements);
            if(array == NULL)
            {
                PRINT_ERROR("malloc error");
                exit(0);
            }

            switch(datatype)
            {
                case _DATATYPE_UINT8 :
                    for(ii = 0; ii < nelements; ii++)
                    {
                        array[ii] = (uint16_t) data.image[ID].array.UI8[ii];
                    }
                    break;

                case _DATATYPE_INT8 :
                    for(ii = 0; ii < nelements; ii++)
                    {
                        array[ii] = (uint16_t) data.image[ID].array.SI8[ii];
                    }
                    break;

                case _DATATYPE_INT16 :
                    for(ii = 0; ii < nelements; ii++)
                    {
                        array[ii] = (uint16_t) data.image[ID].array.SI16[ii];
                    }
                    break;

                case _DATATYPE_UINT32 :
                    for(ii = 0; ii < nelements; ii++)
                    {
                        array[ii] = (uint16_t) data.image[ID].array.UI32[ii];
                    }
                    break;

                case _DATATYPE_INT32 :
                    for(ii = 0; ii < nelements; ii++)
                    {
                        array[ii] = (uint16_t) data.image[ID].array.SI32[ii];
                    }
                    break;

                case _DATATYPE_UINT64 :
                    for(ii = 0; ii < nelements; ii++)
                    {
                        array[ii] = (uint16_t) data.image[ID].array.UI64[ii];
                    }
                    break;

                case _DATATYPE_INT64 :
                    for(ii = 0; ii < nelements; ii++)
                    {
                        array[ii] = (uint16_t) data.image[ID].array.SI64[ii];
                    }
                    break;

                case _DATATYPE_FLOAT :
                    for(ii = 0; ii < nelements; ii++)
                    {
                        array[ii] = (uint16_t) data.image[ID].array.F[ii];
                    }
                    break;

                case _DATATYPE_DOUBLE :
                    for(ii = 0; ii < nelements; ii++)
                    {
                        array[ii] = (uint16_t) data.image[ID].array.D[ii];
                    }
                    break;

                default :
                    list_image_ID();
                    PRINT_ERROR("atype value not recognised");
                    free(array);
                    printf("ID %ld  datatype = %d\n", ID, datatype);
                    exit(0);
                    break;
            }
        }

        fits_create_file(&fptr, file_name1, &FITSIO_status);
        if(check_FITSIO_status(__FILE__, __func__, __LINE__, 1) != 0)
        {
            fprintf(stderr,
                    "%c[%d;%dm Error while calling \"fits_create_file\" with filename \"%s\" %c[%d;m\n",
                    (char) 27, 1, 31, file_name1, (char) 27, 0);
            if(file_exists(file_name1) == 1)
            {
                fprintf(stderr,
                        "%c[%d;%dm File \"%s\" already exists. Make sure you remove this file before attempting to write file with identical name. %c[%d;m\n",
                        (char) 27, 1, 31, file_name1, (char) 27, 0);
                exit(0);
            }
            else
            {
                fprintf(stderr,
                        "%c[%d;%dm Printing Cfits image buffer content: %c[%d;m\n",
                        (char) 27, 1, 31, (char) 27, 0);
                list_image_ID();
            }
        }

        fits_create_img(fptr, USHORT_IMG, naxis, naxesl, &FITSIO_status);
        if(check_FITSIO_status(__FILE__, __func__, __LINE__, 1) != 0)
        {
            fprintf(stderr,
                    "%c[%d;%dm Error while calling \"fits_create_img\" %c[%d;m\n",
                    (char) 27, 1, 31, (char) 27, 0);
            fprintf(stderr,
                    "%c[%d;%dm Printing Cfits image buffer content: %c[%d;m\n",
                    (char) 27, 1, 31, (char) 27, 0);
            list_image_ID();
        }

        if(datatype == _DATATYPE_UINT16)
        {
            fits_write_img(fptr, TUSHORT, fpixel, nelements, data.image[ID].array.UI16,
                           &FITSIO_status);
        }
        else
        {
            fits_write_img(fptr, TUSHORT, fpixel, nelements, array, &FITSIO_status);
            free(array);
            array = NULL;
        }

        if(check_FITSIO_status(__FILE__, __func__, __LINE__, 1) != 0)
        {
            fprintf(stderr,
                    "%c[%d;%dm Error while calling \"fits_write_img\" %c[%d;m\n",
                    (char) 27, 1, 31, (char) 27, 0);
            fprintf(stderr,
                    "%c[%d;%dm Printing Cfits image buffer content: %c[%d;m\n",
                    (char) 27, 1, 31, (char) 27, 0);
            list_image_ID();
        }

        fits_close_file(fptr, &FITSIO_status);
        if(check_FITSIO_status(__FILE__, __func__, __LINE__, 1) != 0)
        {
            fprintf(stderr,
                    "%c[%d;%dm Error while calling \"fits_close_file\" %c[%d;m\n",
                    (char) 27, 1, 31, (char) 27, 0);
            fprintf(stderr,
                    "%c[%d;%dm Printing Cfits image buffer content: %c[%d;m\n",
                    (char) 27, 1, 31, (char) 27, 0);
            list_image_ID();
        }
    }
    else
    {
        fprintf(stderr,
                "%c[%d;%dm image \"%s\" does not exist in memory %c[%d;m\n",
                (char) 27, 1, 31, ID_name, (char) 27, 0);
    }

    return RETURN_SUCCESS;
}










/* saves an image in a int format (32 bit) */

errno_t save_int32_fits(
    const char *restrict ID_name,
    const char *restrict file_name
)
{
    fitsfile *fptr;
    long  fpixel = 1, naxis, nelements;
    uint32_t naxes[3];
    long naxesl[3];
    int32_t *array = NULL;
    imageID ID;
    long ii;
    long i;
    uint8_t datatype;
    char file_name1[SBUFFERSIZE];


    if((data.overwrite == 1) && (file_name[0] != '!')
            && (file_exists(file_name) == 1))
    {
        PRINT_WARNING("automatic overwrite on file \"%s\"\n", file_name);
        WRITE_FULLFILENAME(file_name1, "!%s", file_name);
    }
    else
    {
        WRITE_FULLFILENAME(file_name1, "%s", file_name);
    }

    ID = image_ID(ID_name);

    if(ID != -1)
    {
        datatype = data.image[ID].md[0].datatype;
        naxis = data.image[ID].md[0].naxis;
        for(i = 0; i < naxis; i++)
        {
            naxes[i] = data.image[ID].md[0].size[i];
            naxesl[i] = (long) naxes[i];
        }

        nelements = 1;
        for(i = 0; i < naxis; i++)
        {
            nelements *= naxes[i];
        }

        if(datatype != _DATATYPE_INT32)   // data conversion required
        {

            printf("Data conversion required\n"); //TEST
            fflush(stdout);


            array = (int32_t *) malloc(SIZEOF_DATATYPE_INT32 * nelements);
            if(array == NULL)
            {
                PRINT_ERROR("malloc error");
                exit(0);
            }

            switch(datatype)
            {
                case _DATATYPE_UINT8 :
                    for(ii = 0; ii < nelements; ii++)
                    {
                        array[ii] = (int32_t) data.image[ID].array.UI8[ii];
                    }
                    break;

                case _DATATYPE_INT8 :
                    for(ii = 0; ii < nelements; ii++)
                    {
                        array[ii] = (int32_t) data.image[ID].array.SI8[ii];
                    }
                    break;

                case _DATATYPE_UINT16 :
                    for(ii = 0; ii < nelements; ii++)
                    {
                        array[ii] = (int32_t) data.image[ID].array.UI16[ii];
                    }
                    break;

                case _DATATYPE_INT16 :
                    for(ii = 0; ii < nelements; ii++)
                    {
                        array[ii] = (int32_t) data.image[ID].array.SI16[ii];
                    }
                    break;

                case _DATATYPE_UINT32 :
                    for(ii = 0; ii < nelements; ii++)
                    {
                        array[ii] = (int32_t) data.image[ID].array.UI32[ii];
                    }
                    break;

                case _DATATYPE_UINT64 :
                    for(ii = 0; ii < nelements; ii++)
                    {
                        array[ii] = (int32_t) data.image[ID].array.UI64[ii];
                    }
                    break;

                case _DATATYPE_INT64 :
                    for(ii = 0; ii < nelements; ii++)
                    {
                        array[ii] = (int32_t) data.image[ID].array.SI64[ii];
                    }
                    break;

                case _DATATYPE_FLOAT :
                    for(ii = 0; ii < nelements; ii++)
                    {
                        array[ii] = (int32_t) data.image[ID].array.F[ii];
                    }
                    break;

                case _DATATYPE_DOUBLE :
                    for(ii = 0; ii < nelements; ii++)
                    {
                        array[ii] = (int32_t) data.image[ID].array.D[ii];
                    }
                    break;

                default :
                    list_image_ID();
                    PRINT_ERROR("atype value not recognised");
                    free(array);
                    printf("ID %ld  datatype = %d\n", ID, datatype);
                    exit(0);
                    break;
            }
        }
        else
        {
            printf("No data conversion required\n"); //TEST
            fflush(stdout);
        }


        fits_create_file(&fptr, file_name1, &FITSIO_status);
        if(check_FITSIO_status(__FILE__, __func__, __LINE__, 1) != 0)
        {
            fprintf(stderr,
                    "%c[%d;%dm Error while calling \"fits_create_file\" with filename \"%s\" %c[%d;m\n",
                    (char) 27, 1, 31, file_name1, (char) 27, 0);
            if(file_exists(file_name1) == 1)
            {
                fprintf(stderr,
                        "%c[%d;%dm File \"%s\" already exists. Make sure you remove this file before attempting to write file with identical name. %c[%d;m\n",
                        (char) 27, 1, 31, file_name1, (char) 27, 0);
                exit(0);
            }
            else
            {
                fprintf(stderr,
                        "%c[%d;%dm Printing Cfits image buffer content: %c[%d;m\n",
                        (char) 27, 1, 31, (char) 27, 0);
                list_image_ID();
            }
        }


        //    32          long integer        TLONG
        fits_create_img(fptr, LONG_IMG, naxis, naxesl, &FITSIO_status);
        if(check_FITSIO_status(__FILE__, __func__, __LINE__, 1) != 0)
        {
            fprintf(stderr,
                    "%c[%d;%dm Error while calling \"fits_create_img\" %c[%d;m\n",
                    (char) 27, 1, 31, (char) 27, 0);
            fprintf(stderr,
                    "%c[%d;%dm Printing Cfits image buffer content: %c[%d;m\n",
                    (char) 27, 1, 31, (char) 27, 0);
            list_image_ID();
        }

        if(datatype == _DATATYPE_INT32)
        {
            printf("Direct copy --- \n");
            fflush(stdout);

            fits_write_img(fptr, TINT, fpixel, nelements, data.image[ID].array.SI32,
                           &FITSIO_status);
        }
        else
        {
            fits_write_img(fptr, TINT, fpixel, nelements, array, &FITSIO_status);
            free(array);
            array = NULL;
        }


        if(check_FITSIO_status(__FILE__, __func__, __LINE__, 1) != 0)
        {
            fprintf(stderr,
                    "%c[%d;%dm Error while calling \"fits_write_img\" %c[%d;m\n",
                    (char) 27, 1, 31, (char) 27, 0);
            fprintf(stderr,
                    "%c[%d;%dm Printing Cfits image buffer content: %c[%d;m\n",
                    (char) 27, 1, 31, (char) 27, 0);
            list_image_ID();
        }

        fits_close_file(fptr, &FITSIO_status);
        if(check_FITSIO_status(__FILE__, __func__, __LINE__, 1) != 0)
        {
            fprintf(stderr,
                    "%c[%d;%dm Error while calling \"fits_close_file\" %c[%d;m\n",
                    (char) 27, 1, 31, (char) 27, 0);
            fprintf(stderr,
                    "%c[%d;%dm Printing Cfits image buffer content: %c[%d;m\n",
                    (char) 27, 1, 31, (char) 27, 0);
            list_image_ID();
        }
    }
    else
    {
        fprintf(stderr,
                "%c[%d;%dm image \"%s\" does not exist in memory %c[%d;m\n",
                (char) 27, 1, 31, ID_name, (char) 27, 0);
    }


    return EXIT_SUCCESS;
}






/* saves an image in a unsigned short int format */

errno_t save_uint32_fits(
    const char *restrict ID_name,
    const char *restrict file_name
)
{
    fitsfile *fptr;
    long  fpixel = 1, naxis, nelements;
    uint32_t naxes[3];
    long naxesl[3];
    uint32_t *array = NULL;
    imageID ID;
    long ii;
    long i;
    uint8_t datatype;
    char file_name1[SBUFFERSIZE];


    if((data.overwrite == 1) && (file_name[0] != '!')
            && (file_exists(file_name) == 1))
    {
        PRINT_WARNING("automatic overwrite on file \"%s\"\n", file_name);
		WRITE_FULLFILENAME(file_name1, "!%s", file_name);
    }
    else
    {
        WRITE_FULLFILENAME(file_name1, "%s", file_name);
    }

    ID = image_ID(ID_name);

    if(ID != -1)
    {
        datatype = data.image[ID].md[0].datatype;
        naxis = data.image[ID].md[0].naxis;
        for(i = 0; i < naxis; i++)
        {
            naxes[i] = data.image[ID].md[0].size[i];
            naxesl[i] = (long) naxes[i]; // CFITSIO expects long int *
        }

        nelements = 1;
        for(i = 0; i < naxis; i++)
        {
            nelements *= naxes[i];
        }

        if(datatype != _DATATYPE_UINT32)   // data conversion required
        {
            array = (uint32_t *) malloc(SIZEOF_DATATYPE_UINT32 * nelements);
            if(array == NULL)
            {
                PRINT_ERROR("malloc error");
                exit(0);
            }

            switch(datatype)
            {
                case _DATATYPE_UINT8 :
                    for(ii = 0; ii < nelements; ii++)
                    {
                        array[ii] = (uint32_t) data.image[ID].array.UI8[ii];
                    }
                    break;

                case _DATATYPE_INT8 :
                    for(ii = 0; ii < nelements; ii++)
                    {
                        array[ii] = (uint32_t) data.image[ID].array.SI8[ii];
                    }
                    break;

                case _DATATYPE_UINT16 :
                    for(ii = 0; ii < nelements; ii++)
                    {
                        array[ii] = (uint32_t) data.image[ID].array.UI16[ii];
                    }
                    break;

                case _DATATYPE_INT16 :
                    for(ii = 0; ii < nelements; ii++)
                    {
                        array[ii] = (uint32_t) data.image[ID].array.SI16[ii];
                    }
                    break;

                case _DATATYPE_INT32 :
                    for(ii = 0; ii < nelements; ii++)
                    {
                        array[ii] = (uint32_t) data.image[ID].array.SI32[ii];
                    }
                    break;

                case _DATATYPE_UINT64 :
                    for(ii = 0; ii < nelements; ii++)
                    {
                        array[ii] = (uint32_t) data.image[ID].array.UI64[ii];
                    }
                    break;

                case _DATATYPE_INT64 :
                    for(ii = 0; ii < nelements; ii++)
                    {
                        array[ii] = (uint32_t) data.image[ID].array.SI64[ii];
                    }
                    break;

                case _DATATYPE_FLOAT :
                    for(ii = 0; ii < nelements; ii++)
                    {
                        array[ii] = (uint32_t) data.image[ID].array.F[ii];
                    }
                    break;

                case _DATATYPE_DOUBLE :
                    for(ii = 0; ii < nelements; ii++)
                    {
                        array[ii] = (uint32_t) data.image[ID].array.D[ii];
                    }
                    break;

                default :
                    list_image_ID();
                    PRINT_ERROR("atype value not recognised");
                    free(array);
                    printf("ID %ld  datatype = %d\n", ID, datatype);
                    exit(0);
                    break;
            }
        }

        fits_create_file(&fptr, file_name1, &FITSIO_status);
        if(check_FITSIO_status(__FILE__, __func__, __LINE__, 1) != 0)
        {
            fprintf(stderr,
                    "%c[%d;%dm Error while calling \"fits_create_file\" with filename \"%s\" %c[%d;m\n",
                    (char) 27, 1, 31, file_name1, (char) 27, 0);
            if(file_exists(file_name1) == 1)
            {
                fprintf(stderr,
                        "%c[%d;%dm File \"%s\" already exists. Make sure you remove this file before attempting to write file with identical name. %c[%d;m\n",
                        (char) 27, 1, 31, file_name1, (char) 27, 0);
                exit(0);
            }
            else
            {
                fprintf(stderr,
                        "%c[%d;%dm Printing Cfits image buffer content: %c[%d;m\n",
                        (char) 27, 1, 31, (char) 27, 0);
                list_image_ID();
            }
        }

        fits_create_img(fptr, USHORT_IMG, naxis, naxesl, &FITSIO_status);
        if(check_FITSIO_status(__FILE__, __func__, __LINE__, 1) != 0)
        {
            fprintf(stderr,
                    "%c[%d;%dm Error while calling \"fits_create_img\" %c[%d;m\n",
                    (char) 27, 1, 31, (char) 27, 0);
            fprintf(stderr,
                    "%c[%d;%dm Printing Cfits image buffer content: %c[%d;m\n",
                    (char) 27, 1, 31, (char) 27, 0);
            list_image_ID();
        }

        if(datatype == _DATATYPE_UINT32)
        {
            fits_write_img(fptr, TUINT, fpixel, nelements, data.image[ID].array.UI32,
                           &FITSIO_status);
        }
        else
        {
            fits_write_img(fptr, TUINT, fpixel, nelements, array, &FITSIO_status);
            free(array);
            array = NULL;
        }

        if(check_FITSIO_status(__FILE__, __func__, __LINE__, 1) != 0)
        {
            fprintf(stderr,
                    "%c[%d;%dm Error while calling \"fits_write_img\" %c[%d;m\n",
                    (char) 27, 1, 31, (char) 27, 0);
            fprintf(stderr,
                    "%c[%d;%dm Printing Cfits image buffer content: %c[%d;m\n",
                    (char) 27, 1, 31, (char) 27, 0);
            list_image_ID();
        }

        fits_close_file(fptr, &FITSIO_status);
        if(check_FITSIO_status(__FILE__, __func__, __LINE__, 1) != 0)
        {
            fprintf(stderr,
                    "%c[%d;%dm Error while calling \"fits_close_file\" %c[%d;m\n",
                    (char) 27, 1, 31, (char) 27, 0);
            fprintf(stderr,
                    "%c[%d;%dm Printing Cfits image buffer content: %c[%d;m\n",
                    (char) 27, 1, 31, (char) 27, 0);
            list_image_ID();
        }
    }
    else
    {
        fprintf(stderr,
                "%c[%d;%dm image \"%s\" does not exist in memory %c[%d;m\n",
                (char) 27, 1, 31, ID_name, (char) 27, 0);
    }

    return RETURN_SUCCESS;
}






/* saves an image in a int format (64 bit) */

errno_t save_int64_fits(
    const char *restrict ID_name,
    const char *restrict file_name
)
{
    fitsfile *fptr;
    long  fpixel = 1, naxis, nelements;
    uint32_t naxes[3];
    long naxesl[3];
    int64_t *array = NULL;
    imageID ID;
    long ii;
    long i;
    uint8_t datatype;
    char file_name1[SBUFFERSIZE];



    if((data.overwrite == 1) && (file_name[0] != '!')
            && (file_exists(file_name) == 1))
    {
        PRINT_WARNING("automatic overwrite on file \"%s\"\n", file_name);
        WRITE_FULLFILENAME(file_name1, "!%s", file_name);
    }
    else
    {
        WRITE_FULLFILENAME(file_name1, "%s", file_name);
    }

    ID = image_ID(ID_name);

    if(ID != -1)
    {
        datatype = data.image[ID].md[0].datatype;
        naxis = data.image[ID].md[0].naxis;
        for(i = 0; i < naxis; i++)
        {
            naxes[i] = data.image[ID].md[0].size[i];
            naxesl[i] = (long) naxes[i];
        }

        nelements = 1;
        for(i = 0; i < naxis; i++)
        {
            nelements *= naxes[i];
        }

        if(datatype != _DATATYPE_INT64)   // data conversion required
        {

            printf("Data conversion required\n"); //TEST
            fflush(stdout);


            array = (int64_t *) malloc(SIZEOF_DATATYPE_INT64 * nelements);
            if(array == NULL)
            {
                PRINT_ERROR("malloc error");
                exit(0);
            }

            switch(datatype)
            {
                case _DATATYPE_UINT8 :
                    for(ii = 0; ii < nelements; ii++)
                    {
                        array[ii] = (int64_t) data.image[ID].array.UI8[ii];
                    }
                    break;

                case _DATATYPE_INT8 :
                    for(ii = 0; ii < nelements; ii++)
                    {
                        array[ii] = (int64_t) data.image[ID].array.SI8[ii];
                    }
                    break;

                case _DATATYPE_UINT16 :
                    for(ii = 0; ii < nelements; ii++)
                    {
                        array[ii] = (int64_t) data.image[ID].array.UI16[ii];
                    }
                    break;

                case _DATATYPE_INT16 :
                    for(ii = 0; ii < nelements; ii++)
                    {
                        array[ii] = (int64_t) data.image[ID].array.SI16[ii];
                    }
                    break;

                case _DATATYPE_UINT32 :
                    for(ii = 0; ii < nelements; ii++)
                    {
                        array[ii] = (int64_t) data.image[ID].array.UI32[ii];
                    }
                    break;

                case _DATATYPE_INT32 :
                    for(ii = 0; ii < nelements; ii++)
                    {
                        array[ii] = (int64_t) data.image[ID].array.SI32[ii];
                    }
                    break;

                case _DATATYPE_UINT64 :
                    for(ii = 0; ii < nelements; ii++)
                    {
                        array[ii] = (int64_t) data.image[ID].array.UI64[ii];
                    }
                    break;

                case _DATATYPE_FLOAT :
                    for(ii = 0; ii < nelements; ii++)
                    {
                        array[ii] = (int64_t) data.image[ID].array.F[ii];
                    }
                    break;

                case _DATATYPE_DOUBLE :
                    for(ii = 0; ii < nelements; ii++)
                    {
                        array[ii] = (int64_t) data.image[ID].array.D[ii];
                    }
                    break;

                default :
                    list_image_ID();
                    PRINT_ERROR("atype value not recognised");
                    free(array);
                    printf("ID %ld  datatype = %d\n", ID, datatype);
                    exit(0);
                    break;
            }
        }
        else
        {
            printf("No data conversion required\n"); //TEST
            fflush(stdout);
        }


        fits_create_file(&fptr, file_name1, &FITSIO_status);
        if(check_FITSIO_status(__FILE__, __func__, __LINE__, 1) != 0)
        {
            fprintf(stderr,
                    "%c[%d;%dm Error while calling \"fits_create_file\" with filename \"%s\" %c[%d;m\n",
                    (char) 27, 1, 31, file_name1, (char) 27, 0);
            if(file_exists(file_name1) == 1)
            {
                fprintf(stderr,
                        "%c[%d;%dm File \"%s\" already exists. Make sure you remove this file before attempting to write file with identical name. %c[%d;m\n",
                        (char) 27, 1, 31, file_name1, (char) 27, 0);
                exit(0);
            }
            else
            {
                fprintf(stderr,
                        "%c[%d;%dm Printing Cfits image buffer content: %c[%d;m\n",
                        (char) 27, 1, 31, (char) 27, 0);
                list_image_ID();
            }
        }


        //    32          long integer        TLONG
        fits_create_img(fptr, LONGLONG_IMG, naxis, naxesl, &FITSIO_status);
        if(check_FITSIO_status(__FILE__, __func__, __LINE__, 1) != 0)
        {
            fprintf(stderr,
                    "%c[%d;%dm Error while calling \"fits_create_img\" %c[%d;m\n",
                    (char) 27, 1, 31, (char) 27, 0);
            fprintf(stderr,
                    "%c[%d;%dm Printing Cfits image buffer content: %c[%d;m\n",
                    (char) 27, 1, 31, (char) 27, 0);
            list_image_ID();
        }

        if(datatype == _DATATYPE_INT32)
        {
            printf("Direct copy --- \n");
            fflush(stdout);

            fits_write_img(fptr, TLONG, fpixel, nelements, data.image[ID].array.SI64,
                           &FITSIO_status);
        }
        else
        {
            fits_write_img(fptr, TLONG, fpixel, nelements, array, &FITSIO_status);
            free(array);
            array = NULL;
        }


        if(check_FITSIO_status(__FILE__, __func__, __LINE__, 1) != 0)
        {
            fprintf(stderr,
                    "%c[%d;%dm Error while calling \"fits_write_img\" %c[%d;m\n",
                    (char) 27, 1, 31, (char) 27, 0);
            fprintf(stderr,
                    "%c[%d;%dm Printing Cfits image buffer content: %c[%d;m\n",
                    (char) 27, 1, 31, (char) 27, 0);
            list_image_ID();
        }

        fits_close_file(fptr, &FITSIO_status);
        if(check_FITSIO_status(__FILE__, __func__, __LINE__, 1) != 0)
        {
            fprintf(stderr,
                    "%c[%d;%dm Error while calling \"fits_close_file\" %c[%d;m\n",
                    (char) 27, 1, 31, (char) 27, 0);
            fprintf(stderr,
                    "%c[%d;%dm Printing Cfits image buffer content: %c[%d;m\n",
                    (char) 27, 1, 31, (char) 27, 0);
            list_image_ID();
        }
    }
    else
    {
        fprintf(stderr,
                "%c[%d;%dm image \"%s\" does not exist in memory %c[%d;m\n",
                (char) 27, 1, 31, ID_name, (char) 27, 0);
    }


    return EXIT_SUCCESS;
}






errno_t save_fits(
    const char *restrict ID_name,
    const char *restrict file_name
)
{
    char savename[1000];
    if(file_name[0] == '!')
    {
        strcpy(savename, file_name + 1);    // avoid the leading '!'
    }
    else
    {
        strcpy(savename, file_name);
    }

    return save_fits_atomic(ID_name, savename);
}



/**
 * atomic save is a two step process:
 *
 * (1) save to temporary unique file
 * (2) change file name
 *
 */
errno_t save_fits_atomic(
    const char *restrict ID_name,
    const char *restrict file_name
)
{
    imageID ID;
    uint8_t datatype;
    char fnametmp[STRINGMAXLEN_FILENAME];
    char savename[STRINGMAXLEN_FILENAME];
    pthread_t self_id;

    ID = image_ID(ID_name);

    // get PID to include in file name, so that file name is unique
    self_id = pthread_self();

    WRITE_FILENAME(fnametmp, "_savefits_atomic_%s_%d_%ld.tmp.fits", ID_name,
                   (int) getpid(), (long) self_id);
    WRITE_FILENAME(savename, "!%s", fnametmp);

    if(ID != -1)
    {
        datatype = data.image[ID].md[0].datatype;
        switch(datatype)
        {

            case _DATATYPE_UINT8:
                save_ush16_fits(ID_name, savename);
                break;

            case _DATATYPE_INT8:
                save_sh16_fits(ID_name, savename);
                break;

            case _DATATYPE_UINT16:
                save_ush16_fits(ID_name, savename);
                break;

            case _DATATYPE_INT16:
                save_sh16_fits(ID_name, savename);
                break;

            case _DATATYPE_UINT32:
                save_uint32_fits(ID_name, savename);
                break;

            case _DATATYPE_INT32:
                save_int32_fits(ID_name, savename);
                break;

            case _DATATYPE_UINT64:
                save_int64_fits(ID_name, savename);
                break;

            case _DATATYPE_INT64:
                save_int64_fits(ID_name, savename);
                break;

            case _DATATYPE_FLOAT:
                save_fl_fits(ID_name, savename);
                break;

            case _DATATYPE_DOUBLE:
                save_db_fits(ID_name, savename);
                break;
        }

        EXECUTE_SYSTEM_COMMAND("mv %s %s", fnametmp, file_name);
    }

    return RETURN_SUCCESS;
}



errno_t saveall_fits(
    const char *restrict savedirname
)
{
    long i;
    char fname[STRINGMAXLEN_FULLFILENAME];

    EXECUTE_SYSTEM_COMMAND("mkdir -p %s", savedirname);

    for(i = 0; i < data.NB_MAX_IMAGE; i++)
        if(data.image[i].used == 1)
        {

            WRITE_FULLFILENAME(fname, "!./%s/%s.fits", savedirname, data.image[i].name);
            save_fits(data.image[i].name, fname);
        }

    return RETURN_SUCCESS;
}












/* =============================================================================================== */
/*                                                                                                 */
/* 2. CUBES                                                                                        */
/*                                                                                                 */
/* =============================================================================================== */


imageID break_cube(
    const char *restrict ID_name
)
{
    imageID ID;
    uint32_t naxes[3];
    long i;

    ID = image_ID(ID_name);
    naxes[0] = data.image[ID].md[0].size[0];
    naxes[1] = data.image[ID].md[0].size[1];
    naxes[2] = data.image[ID].md[0].size[2];

    for(uint32_t kk = 0; kk < naxes[2]; kk++)
    {
        long ID1;

		CREATE_IMAGENAME(framename, "%s_%5u", ID_name, kk);

        for(i = 0; i < (long) strlen(framename); i++)
        {
            if(framename[i] == ' ')
            {
                framename[i] = '0';
            }
        }
        ID1 = create_2Dimage_ID(framename, naxes[0], naxes[1]);
        for(uint32_t ii = 0; ii < naxes[0]; ii++)
            for(uint32_t jj = 0; jj < naxes[1]; jj++)
            {
                data.image[ID1].array.F[jj * naxes[0] + ii] = data.image[ID].array.F[kk *
                        naxes[0] * naxes[1] + jj * naxes[0] + ii];
            }
    }

    return ID;
}




errno_t images_to_cube(
    const char *restrict img_name,
    long                 nbframes,
    const char *restrict cube_name
)
{
    imageID ID;
    imageID ID1;
    long frame;
    uint32_t naxes[2];
    long ii, jj;
    uint32_t xsize, ysize;

    frame = 0;
    
    CREATE_IMAGENAME(imname, "%s%05ld", img_name, frame);

    ID1 = image_ID(imname);
    if(ID1 == -1)
    {
        PRINT_ERROR("Image \"%s\" does not exist", imname);
        exit(0);
    }
    naxes[0] = data.image[ID1].md[0].size[0];
    naxes[1] = data.image[ID1].md[0].size[1];
    xsize = naxes[0];
    ysize = naxes[1];

    printf("SIZE = %ld %ld %ld\n", (long) naxes[0], (long) naxes[1],
           (long) nbframes);
    fflush(stdout);
    ID = create_3Dimage_ID(cube_name, naxes[0], naxes[1], nbframes);
    for(ii = 0; ii < naxes[0]; ii++)
        for(jj = 0; jj < naxes[1]; jj++)
        {
            data.image[ID].array.F[frame * naxes[0]*naxes[1] + (jj * naxes[0] + ii)] =
                data.image[ID1].array.F[jj * naxes[0] + ii];
        }

    for(frame = 1; frame < nbframes; frame++)
    {
        WRITE_IMAGENAME(imname, "%s%05ld", img_name, frame);
        printf("Adding image %s -> %ld/%ld ... ", img_name, frame, nbframes);
        fflush(stdout);

        ID1 = image_ID(imname);
        if(ID1 == -1)
        {
            PRINT_ERROR("Image \"%s\" does not exist - skipping", imname);
        }
        else
        {
            naxes[0] = data.image[ID1].md[0].size[0];
            naxes[1] = data.image[ID1].md[0].size[1];
            if((xsize != naxes[0]) || (ysize != naxes[1]))
            {
                PRINT_ERROR("Image has wrong size");
                exit(0);
            }
            for(ii = 0; ii < naxes[0]; ii++)
                for(jj = 0; jj < naxes[1]; jj++)
                {
                    data.image[ID].array.F[frame * naxes[0]*naxes[1] + (jj * naxes[0] + ii)] =
                        data.image[ID1].array.F[jj * naxes[0] + ii];
                }
        }
        printf("Done\n");
        fflush(stdout);
    }

    return RETURN_SUCCESS;
}






// High-level load to stream


imageID COREMOD_IOFITS_LoadMemStream(
    const char *sname,
    uint64_t   *streamflag,
    uint32_t   *imLOC
)
{
    imageID ID = 0;
    __attribute__((unused)) int updateSHAREMEM =
        0; // toggles to 1 if updating shared mem
    int updateCONFFITS = 0; // toggles to 1 if updating CONF FITS


    int MEMLOADREPORT = 0;
    if(FPFLAG_STREAM_MEMLOADREPORT & *streamflag)   // write report to disk
    {
        MEMLOADREPORT = 1;
    }
    MEMLOADREPORT = 1;// TMP


    *imLOC = STREAM_LOAD_SOURCE_NOTFOUND;
    printf("imLOC = %d\n", *imLOC);




    COREMOD_IOFITS_PRINTDEBUG;

    if(strcmp(sname, "NULL") == 0)   // don't bother looking for it
    {
        *imLOC = STREAM_LOAD_SOURCE_NULL;
        ID = -1;
    }

    int imLOCALMEM;
    if(*imLOC == STREAM_LOAD_SOURCE_NOTFOUND)   // still searching
    {
        // Does image exist in memory ?
        ID = image_ID(sname);
        if(ID == -1)
        {
            imLOCALMEM = 0;
            COREMOD_IOFITS_PRINTDEBUG;
            if(MEMLOADREPORT == 1)
            {
                char msg[STRINGMAXLEN_FPS_LOGMSG];
                SNPRINTF_CHECK(msg, STRINGMAXLEN_FPS_LOGMSG,
                               "%s stream not in local memory", sname);
                functionparameter_outlog("LOADMEMSTREAM", msg);
            }
        }
        else
        {
            imLOCALMEM = 1;
            COREMOD_IOFITS_PRINTDEBUG;
            if(MEMLOADREPORT == 1)
            {
                char msg[STRINGMAXLEN_FPS_LOGMSG];
                SNPRINTF_CHECK(msg, STRINGMAXLEN_FPS_LOGMSG,
                               "%s stream in local memory", sname);
                functionparameter_outlog("LOADMEMSTREAM", msg);
            }
        }

        COREMOD_IOFITS_PRINTDEBUG;

        //    printf("imLOC = %d\n", *imLOC);


        // FORCE_LOCALMEM
        if(FPFLAG_STREAM_LOAD_FORCE_LOCALMEM & *streamflag)
        {
            COREMOD_IOFITS_PRINTDEBUG;
            if(imLOCALMEM == 0)
            {
                *imLOC = STREAM_LOAD_SOURCE_EXITFAILURE; // fail
                COREMOD_IOFITS_PRINTDEBUG;
                if(MEMLOADREPORT)
                {
                    char msg[STRINGMAXLEN_FPS_LOGMSG];
                    SNPRINTF_CHECK(msg, STRINGMAXLEN_FPS_LOGMSG,
                                   "%s EXITFAIL STREAM_LOAD_FORCE_LOCALMEM: Image does not exist in local memory",
                                   sname);
                    functionparameter_outlog("LOADMEMSTREAM", msg);
                }
            }
            else
            {
                *imLOC = STREAM_LOAD_SOURCE_LOCALMEM;
                COREMOD_IOFITS_PRINTDEBUG;
                if(MEMLOADREPORT == 1)
                {
                    char msg[STRINGMAXLEN_FPS_LOGMSG];
                    SNPRINTF_CHECK(msg, STRINGMAXLEN_FPS_LOGMSG,
                                   "%s SUCCESS STREAM_LOAD_FORCE_LOCALMEM", sname);
                    functionparameter_outlog("LOADMEMSTREAM", msg);
                    sprintf(msg, "%s imLOC %u", sname, *imLOC);
                    functionparameter_outlog("LOADMEMSTREAM", msg);
                }
            }
        }
    }

    // FORCE_SHAREMEM
    if(*imLOC == STREAM_LOAD_SOURCE_NOTFOUND)   // still searching
    {
        if(FPFLAG_STREAM_LOAD_FORCE_SHAREMEM & *streamflag)
        {
            COREMOD_IOFITS_PRINTDEBUG;
            // search SHAREMEM
            ID = read_sharedmem_image(sname);
            if(ID == -1)
            {
                *imLOC = STREAM_LOAD_SOURCE_EXITFAILURE; // fail
                if(MEMLOADREPORT == 1)
                {
                    char msg[STRINGMAXLEN_FPS_LOGMSG];
                    SNPRINTF_CHECK(msg, STRINGMAXLEN_FPS_LOGMSG,
                                   "%s EXITFAIL STREAM_LOAD_FORCE_SHAREDMEM: Image does not exist in shared memory",
                                   sname);
                    functionparameter_outlog("LOADMEMSTREAM", msg);
                }
            }
            else
            {
                *imLOC = STREAM_LOAD_SOURCE_SHAREMEM;
                if(MEMLOADREPORT == 1)
                {
                    char msg[STRINGMAXLEN_FPS_LOGMSG];
                    SNPRINTF_CHECK(msg, STRINGMAXLEN_FPS_LOGMSG,
                                   "%s SUCCESS STREAM_LOAD_FORCE_SHAREDMEM", sname);
                    functionparameter_outlog("LOADMEMSTREAM", msg);
                    SNPRINTF_CHECK(msg, STRINGMAXLEN_FPS_LOGMSG,
                                   "%s imLOC %u", sname, *imLOC);
                    functionparameter_outlog("LOADMEMSTREAM", msg);
                }
            }
        }
    }


    // FORCE_CONFFITS
    if(*imLOC == STREAM_LOAD_SOURCE_NOTFOUND)   // still searching
    {
        if(FPFLAG_STREAM_LOAD_FORCE_CONFFITS & *streamflag)
        {
            COREMOD_IOFITS_PRINTDEBUG;
            // search CONFFITS
            char fname[STRINGMAXLEN_FULLFILENAME];
            WRITE_FULLFILENAME(fname, "./conf/shmim.%s.fits", sname);
            ID = load_fits(fname, sname, 0);
            if(ID == -1)
            {
                *imLOC = STREAM_LOAD_SOURCE_EXITFAILURE; // fail
                if(MEMLOADREPORT == 1)
                {
                    char msg[STRINGMAXLEN_FPS_LOGMSG];
                    SNPRINTF_CHECK(msg, STRINGMAXLEN_FPS_LOGMSG,
                                   "%s EXITFAIL STREAM_LOAD_FORCE_CONFFITS: Image does not exist as conf FITS",
                                   sname);
                    functionparameter_outlog("LOADMEMSTREAM", msg);
                }
            }
            else
            {
                *imLOC = STREAM_LOAD_SOURCE_CONFFITS;
                if(MEMLOADREPORT == 1)
                {
                    char msg[STRINGMAXLEN_FPS_LOGMSG];
                    SNPRINTF_CHECK(msg, STRINGMAXLEN_FPS_LOGMSG,
                                   "%s SUCCESS STREAM_LOAD_FORCE_CONFFITS", sname);
                    functionparameter_outlog("LOADMEMSTREAM", msg);
                    SNPRINTF_CHECK(msg, STRINGMAXLEN_FPS_LOGMSG,
                                   "%s imLOC %u", sname, *imLOC);
                    functionparameter_outlog("LOADMEMSTREAM", msg);
                }
            }
        }
    }


    // FORCE_CONFNAME
    if(*imLOC == STREAM_LOAD_SOURCE_NOTFOUND)   // still searching
    {
        if(FPFLAG_STREAM_LOAD_FORCE_CONFNAME & *streamflag)
        {
            COREMOD_IOFITS_PRINTDEBUG;
            // search CONFNAME
            FILE *fp;
            char fname[STRINGMAXLEN_FULLFILENAME] = "";
            char streamfname[STRINGMAXLEN_FULLFILENAME] = "";
            int fscanfcnt = 0;

            WRITE_FULLFILENAME(fname, "./conf/shmim.%s.fname.txt", sname);

            fp = fopen(fname, "r");
            if(fp == NULL)
            {
                printf("ERROR: stream %s could not be loaded from CONF\n", sname);
                *imLOC = STREAM_LOAD_SOURCE_EXITFAILURE; // fail
                if(MEMLOADREPORT == 1)
                {
                    char msg[STRINGMAXLEN_FPS_LOGMSG];
                    SNPRINTF_CHECK(msg, STRINGMAXLEN_FPS_LOGMSG,
                                   "%s EXITFAIL STREAM_LOAD_FORCE_CONFNAME: File %s does not exist", sname, fname);
                    functionparameter_outlog("LOADMEMSTREAM", msg);
                    SNPRINTF_CHECK(msg, STRINGMAXLEN_FPS_LOGMSG,
                                   "%s imLOC %u", sname, *imLOC);
                    functionparameter_outlog("LOADMEMSTREAM", msg);
                }
            }
            else
            {
                fscanfcnt = fscanf(fp, "%s", streamfname);
                if(fscanfcnt == EOF)
                {
                    if(ferror(fp))
                    {
                        perror("fscanf");
                        *imLOC = STREAM_LOAD_SOURCE_EXITFAILURE; // fail
                        if(MEMLOADREPORT == 1)
                        {
                            char msg[STRINGMAXLEN_FPS_LOGMSG];
                            SNPRINTF_CHECK(msg, STRINGMAXLEN_FPS_LOGMSG,
                                           "%s EXITFAIL STREAM_LOAD_FORCE_CONFNAME: fscanf cannot read stream fname",
                                           sname);
                            functionparameter_outlog("LOADMEMSTREAM", msg);
                        }
                    }
                    else
                    {
                        fprintf(stderr,
                                "Error: fscanf reached end of file, no matching characters, no matching failure\n");
                        *imLOC = STREAM_LOAD_SOURCE_EXITFAILURE; // fail
                        if(MEMLOADREPORT == 1)
                        {
                            char msg[STRINGMAXLEN_FPS_LOGMSG];
                            SNPRINTF_CHECK(msg, STRINGMAXLEN_FPS_LOGMSG,
                                           "%s EXITFAIL STREAM_LOAD_FORCE_CONFNAME: fscanf reached end of file, no matching characters",
                                           sname);
                            functionparameter_outlog("LOADMEMSTREAM", msg);
                            SNPRINTF_CHECK(msg, STRINGMAXLEN_FPS_LOGMSG,
                                           "%s imLOC %u", sname, *imLOC);
                            functionparameter_outlog("LOADMEMSTREAM", msg);
                        }
                    }
                }
                fclose(fp);
            }


            if(*imLOC == STREAM_LOAD_SOURCE_NOTFOUND)
            {
                if(fscanfcnt > 0)
                {
                    {
                        ID = load_fits(streamfname, sname, 0);
                        if(ID == -1)
                        {
                            *imLOC = STREAM_LOAD_SOURCE_EXITFAILURE; // fail
                            if(MEMLOADREPORT == 1)
                            {
                                char msg[STRINGMAXLEN_FPS_LOGMSG];
                                SNPRINTF_CHECK(msg, STRINGMAXLEN_FPS_LOGMSG,
                                               "%s EXITFAIL STREAM_LOAD_FORCE_CONFNAME: cannot load stream fname", sname);
                                functionparameter_outlog("LOADMEMSTREAM", msg);
                                SNPRINTF_CHECK(msg, STRINGMAXLEN_FPS_LOGMSG,
                                               "%s imLOC %u", sname, *imLOC);
                                functionparameter_outlog("LOADMEMSTREAM", msg);
                            }
                        }
                        else
                        {
                            *imLOC = STREAM_LOAD_SOURCE_CONFNAME;
                            if(MEMLOADREPORT == 1)
                            {
                                char msg[STRINGMAXLEN_FPS_LOGMSG];
                                SNPRINTF_CHECK(msg, STRINGMAXLEN_FPS_LOGMSG,
                                               "%s SUCCESS STREAM_LOAD_FORCE_CONFFITS", sname);
                                functionparameter_outlog("LOADMEMSTREAM", msg);
                                SNPRINTF_CHECK(msg, STRINGMAXLEN_FPS_LOGMSG,
                                               "%s imLOC %u", sname, *imLOC);
                                functionparameter_outlog("LOADMEMSTREAM", msg);
                            }
                        }
                    }
                }
            }
        }
    }



    // SEARCH LOCALMEM
    if(*imLOC == STREAM_LOAD_SOURCE_NOTFOUND)   // still searching
    {
        COREMOD_IOFITS_PRINTDEBUG;
        //printf("imLOC = %d\n", *imLOC);
        if(!(FPFLAG_STREAM_LOAD_SKIPSEARCH_LOCALMEM & *streamflag))
        {
            COREMOD_IOFITS_PRINTDEBUG;
            if(MEMLOADREPORT == 1)
            {
                char msg[STRINGMAXLEN_FPS_LOGMSG];
                SNPRINTF_CHECK(msg, STRINGMAXLEN_FPS_LOGMSG,
                               "%s Search LOCALMEM", sname);
                functionparameter_outlog("LOADMEMSTREAM", msg);
            }
            if(imLOCALMEM == 1)
            {
                *imLOC = STREAM_LOAD_SOURCE_LOCALMEM;
                if(MEMLOADREPORT == 1)
                {
                    char msg[STRINGMAXLEN_FPS_LOGMSG];
                    SNPRINTF_CHECK(msg, STRINGMAXLEN_FPS_LOGMSG,
                                   "%s SUCCESS found image in LOCALMEM", sname);
                    functionparameter_outlog("LOADMEMSTREAM", msg);
                    SNPRINTF_CHECK(msg, STRINGMAXLEN_FPS_LOGMSG, "%s imLOC %u", sname, *imLOC);
                    functionparameter_outlog("LOADMEMSTREAM", msg);
                }
            }
            else
            {
                char msg[STRINGMAXLEN_FPS_LOGMSG];
                SNPRINTF_CHECK(msg, STRINGMAXLEN_FPS_LOGMSG, "%s localmem stream not found",
                               sname);
                functionparameter_outlog("LOADMEMSTREAM", msg);
            }
        }
        //printf("imLOC = %d\n", *imLOC);
    }


    // SEARCH SHAREMEM
    if(*imLOC == STREAM_LOAD_SOURCE_NOTFOUND)   // still searching
    {
        COREMOD_IOFITS_PRINTDEBUG;
        //printf("imLOC = %d\n", *imLOC);
        if(!(FPFLAG_STREAM_LOAD_SKIPSEARCH_SHAREMEM & *streamflag))
        {
            COREMOD_IOFITS_PRINTDEBUG;
            if(MEMLOADREPORT == 1)
            {
                char msg[STRINGMAXLEN_FPS_LOGMSG];
                SNPRINTF_CHECK(msg, STRINGMAXLEN_FPS_LOGMSG,
                               "%s Search SHAREMEM", sname);
                functionparameter_outlog("LOADMEMSTREAM", msg);
            }
            ID = read_sharedmem_image(sname);
            if(ID != -1)
            {
                *imLOC = STREAM_LOAD_SOURCE_SHAREMEM;
                if(MEMLOADREPORT == 1)
                {
                    char msg[STRINGMAXLEN_FPS_LOGMSG];
                    SNPRINTF_CHECK(msg, STRINGMAXLEN_FPS_LOGMSG,
                                   "%s SUCCESS found image in SHAREMEM", sname);
                    functionparameter_outlog("LOADMEMSTREAM", msg);
                    SNPRINTF_CHECK(msg, STRINGMAXLEN_FPS_LOGMSG,
                                   "%s imLOC %u", sname, *imLOC);
                    functionparameter_outlog("LOADMEMSTREAM", msg);
                }
            }
            else
            {
                char msg[STRINGMAXLEN_FPS_LOGMSG];
                SNPRINTF_CHECK(msg, STRINGMAXLEN_FPS_LOGMSG,
                               "%s sharedmem stream not found", sname);
                functionparameter_outlog("LOADMEMSTREAM", msg);
            }
        }
        //printf("imLOC = %d\n", *imLOC);
    }


    // SEARCH CONFFITS
    if(*imLOC == STREAM_LOAD_SOURCE_NOTFOUND)   // still searching
    {
        if(!(FPFLAG_STREAM_LOAD_SKIPSEARCH_CONFFITS & *streamflag))
        {
            COREMOD_IOFITS_PRINTDEBUG;
            if(MEMLOADREPORT == 1)
            {
                char msg[STRINGMAXLEN_FPS_LOGMSG];
                SNPRINTF_CHECK(msg, STRINGMAXLEN_FPS_LOGMSG,
                               "%s Search CONFFITS", sname);
                functionparameter_outlog("LOADMEMSTREAM", msg);
            }
            //printf("imLOC = %d\n", *imLOC);
            char fname[STRINGMAXLEN_FULLFILENAME];
            WRITE_FULLFILENAME(fname, "./conf/shmim.%s.fits", sname);
            ID = load_fits(fname, sname, 0);
            if(ID != -1)
            {
                *imLOC = STREAM_LOAD_SOURCE_CONFFITS;
                if(MEMLOADREPORT == 1)
                {
                    char msg[STRINGMAXLEN_FPS_LOGMSG];
                    SNPRINTF_CHECK(msg, STRINGMAXLEN_FPS_LOGMSG,
                                   "%s SUCCESS found image in CONFFITS", sname);
                    functionparameter_outlog("LOADMEMSTREAM", msg);
                    SNPRINTF_CHECK(msg, STRINGMAXLEN_FPS_LOGMSG,
                                   "%s imLOC %u", sname, *imLOC);
                    functionparameter_outlog("LOADMEMSTREAM", msg);
                }
            }
            else
            {
                char msg[STRINGMAXLEN_FPS_LOGMSG];
                SNPRINTF_CHECK(msg, STRINGMAXLEN_FPS_LOGMSG,
                               "%s File %s not found", sname, fname);
                functionparameter_outlog("LOADMEMSTREAM", msg);
            }
            // printf("imLOC = %d\n", *imLOC);
        }
    }


    // SEARCH CONFNAME
    if(*imLOC == STREAM_LOAD_SOURCE_NOTFOUND)   // still searching
    {
        if(!(FPFLAG_STREAM_LOAD_SKIPSEARCH_CONFNAME & *streamflag))
        {
            COREMOD_IOFITS_PRINTDEBUG;
            if(MEMLOADREPORT == 1)
            {
                char msg[STRINGMAXLEN_FPS_LOGMSG];
                SNPRINTF_CHECK(msg, STRINGMAXLEN_FPS_LOGMSG,
                               "%s Search CONFNAME", sname);
                functionparameter_outlog("LOADMEMSTREAM", msg);
            }

            //printf("imLOC = %d\n", *imLOC);
            FILE *fp;
            char fname[200] = "";
            char streamfname[200] = "";
            int fscanfcnt = 0;

            sprintf(fname, "./conf/shmim.%s.fname.txt", sname);

            fp = fopen(fname, "r");
            if(fp == NULL)
            {
                printf("ERROR: stream %s could not be loaded from CONF\n", sname);
                if(MEMLOADREPORT == 1)
                {
                    char msg[STRINGMAXLEN_FPS_LOGMSG];
                    SNPRINTF_CHECK(msg, STRINGMAXLEN_FPS_LOGMSG,
                                   "%s Cannot find CONFNAME file %s", sname, fname);
                    functionparameter_outlog("LOADMEMSTREAM", msg);
                    // don't fail... keep going
                }
            }
            else
            {
                fscanfcnt = fscanf(fp, "%s", streamfname);
                if(fscanfcnt == EOF)
                {
                    if(ferror(fp))
                    {
                        perror("fscanf");
                        *imLOC = STREAM_LOAD_SOURCE_EXITFAILURE; // fail
                        if(MEMLOADREPORT == 1)
                        {
                            char msg[STRINGMAXLEN_FPS_LOGMSG];
                            SNPRINTF_CHECK(msg, STRINGMAXLEN_FPS_LOGMSG,
                                           "%s EXITFAILURE fscanf error reading %s", sname,  fname);
                            functionparameter_outlog("LOADMEMSTREAM", msg);
                            SNPRINTF_CHECK(msg, STRINGMAXLEN_FPS_LOGMSG,
                                           "%s imLOC %u", sname, *imLOC);
                            functionparameter_outlog("LOADMEMSTREAM", msg);
                        }
                    }
                    else
                    {
                        fprintf(stderr,
                                "Error: fscanf reached end of file, no matching characters, no matching failure\n");
                        *imLOC = STREAM_LOAD_SOURCE_EXITFAILURE; // fail
                        if(MEMLOADREPORT == 1)
                        {
                            char msg[STRINGMAXLEN_FPS_LOGMSG];
                            SNPRINTF_CHECK(msg, STRINGMAXLEN_FPS_LOGMSG,
                                           "%s EXITFAILURE fscanf error reading %s. fscanf reached end of file, no matching characters",
                                           sname, fname);
                            functionparameter_outlog("LOADMEMSTREAM", msg);
                            SNPRINTF_CHECK(msg, STRINGMAXLEN_FPS_LOGMSG,
                                           "%s imLOC %u", sname, *imLOC);
                            functionparameter_outlog("LOADMEMSTREAM", msg);
                        }
                    }
                }
                fclose(fp);
            }

            if(*imLOC == STREAM_LOAD_SOURCE_NOTFOUND)
            {
                if(fscanfcnt > 0)
                {
                    {
                        char msg[STRINGMAXLEN_FPS_LOGMSG];
                        SNPRINTF_CHECK(msg, STRINGMAXLEN_FPS_LOGMSG,
                                       "%s LOADING %s", sname, streamfname);
                        functionparameter_outlog("LOADMEMSTREAM", msg);
                        ID = load_fits(streamfname, sname, 0);
                        if(ID != -1)
                        {
                            *imLOC = STREAM_LOAD_SOURCE_CONFNAME;
                            updateCONFFITS = 1;
                            if(MEMLOADREPORT == 1)
                            {
                                SNPRINTF_CHECK(msg, STRINGMAXLEN_FPS_LOGMSG, "%s SUCCESS CONFNAME", sname);
                                functionparameter_outlog("LOADMEMSTREAM", msg)
                                ;
                                SNPRINTF_CHECK(msg, STRINGMAXLEN_FPS_LOGMSG,
                                               "%s imLOC %u", sname, *imLOC);
                                functionparameter_outlog("LOADMEMSTREAM", msg);
                            }
                        }
                    }
                }
            }
            //printf("imLOC = %d\n", *imLOC);
        }
    }


    // copy to shared memory
    if(*imLOC == STREAM_LOAD_SOURCE_LOCALMEM)
        if(FPFLAG_STREAM_LOAD_UPDATE_SHAREMEM & *streamflag)
        {
            COREMOD_IOFITS_PRINTDEBUG;
            if(MEMLOADREPORT == 1)
            {
                char msg[STRINGMAXLEN_FPS_LOGMSG];
                SNPRINTF_CHECK(msg, STRINGMAXLEN_FPS_LOGMSG,
                               "%s copy to SHAREMEM", sname);
                functionparameter_outlog("LOADMEMSTREAM", msg);
            }
            copy_image_ID(sname, sname, 1);
        }



    // copy to conf FITS
    if((*imLOC != STREAM_LOAD_SOURCE_NOTFOUND)
            && (*imLOC != STREAM_LOAD_SOURCE_CONFFITS))
        if(FPFLAG_STREAM_LOAD_UPDATE_CONFFITS & *streamflag)
        {
            updateCONFFITS = 1;
        }
    if(updateCONFFITS == 1)
    {
        COREMOD_IOFITS_PRINTDEBUG;
        char fname[STRINGMAXLEN_FULLFILENAME];
        if(MEMLOADREPORT == 1)
        {
            char msg[STRINGMAXLEN_FPS_LOGMSG];
            SNPRINTF_CHECK(msg, STRINGMAXLEN_FPS_LOGMSG,
                           "%s copy to CONFFITS", sname);
            functionparameter_outlog("LOADMEMSTREAM", msg);
        }
        WRITE_FULLFILENAME(fname, "./conf/shmim.%s.fits", sname);
        save_fits(sname, fname);
    }

    COREMOD_IOFITS_PRINTDEBUG;


    if((int) *imLOC == STREAM_LOAD_SOURCE_EXITFAILURE)
    {
        *imLOC = STREAM_LOAD_SOURCE_NOTFOUND;
    }

    if(MEMLOADREPORT == 1)
    {
        char msg[STRINGMAXLEN_FPS_LOGMSG];
        char locstring[100];

        switch(*imLOC)
        {

            case STREAM_LOAD_SOURCE_NOTFOUND :
                strcpy(locstring, STREAM_LOAD_SOURCE_NOTFOUND_STRING);
                break;

            case STREAM_LOAD_SOURCE_LOCALMEM :
                strcpy(locstring, STREAM_LOAD_SOURCE_LOCALMEM_STRING);
                break;

            case STREAM_LOAD_SOURCE_SHAREMEM :
                strcpy(locstring, STREAM_LOAD_SOURCE_SHAREMEM_STRING);
                break;

            case STREAM_LOAD_SOURCE_CONFFITS :
                strcpy(locstring, STREAM_LOAD_SOURCE_CONFFITS_STRING);
                break;

            case STREAM_LOAD_SOURCE_CONFNAME :
                strcpy(locstring, STREAM_LOAD_SOURCE_CONFNAME_STRING);
                break;

            case STREAM_LOAD_SOURCE_NULL :
                strcpy(locstring, STREAM_LOAD_SOURCE_NULL_STRING);
                break;

            case STREAM_LOAD_SOURCE_EXITFAILURE :
                strcpy(locstring, STREAM_LOAD_SOURCE_EXITFAILURE_STRING);
                break;

            default :
                strcpy(locstring, "unknown");
                break;
        }


        SNPRINTF_CHECK(msg, STRINGMAXLEN_FPS_LOGMSG, "%s FINAL imLOC %u %s", sname,
                       *imLOC, locstring);

        functionparameter_outlog("LOADMEMSTREAM", msg);
    }

    return ID;
}



