/* UserList.C */
/* Print out the User Listing of a WWIV BBS */
/* Written by Xenos AKA Eric Pareja */
/* WWIV #30282 */
#include <stdio.h>
#include <string.h>
#include <sys/io.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "sdk/vardec.h"
//#include "vardec.h"
int numdirs;

int main(int argc, char *argv[])
{
    FILE *infile,*outfile,*upfile;
    char dirfile[81],output[81],uploads[81],p[81];
    output[0]=0;
// = "/home/xenos/alcove/C:/WWIV/DATA/user.lst";
    switch ( argc ) {
      case 1:
//        file=stdin;
        break;
      case 2:
        strcpy(dirfile,argv[1]);
        break;
      case 3:
        strcpy(dirfile,argv[1]);
        strcpy(output,argv[2]);
        break;
      default:
        break;
	}



    int i,j;
    unsigned int loop,num=0,numups;
    unsigned long len,ups;
    directoryrec dir;
    uploadsrec upload;
    char line[81];

//    if ((f=open(dirfile,O_RDWR|O_BINARY,S_IREAD|S_IWRITE))<=0) {
		infile=fopen(dirfile,"r");
		if (infile==NULL) {
        printf("\nCould not open Dir Board List  %s\n",dirfile);
		return(1);
        };

    if ((outfile=fopen(output,"w"))==NULL) {
        printf("\nCould not open SUB listing text file %s\n",dirfile);
        return(1);
        };
		fseek(infile, 0L, SEEK_END);
		len=ftell(infile);
//		fseek(infile, 0L, SEEK_SET);
		rewind(infile);
//    len=filelength(infile);
    num=(len/sizeof(directoryrec))+!(len%sizeof(directoryrec));
    printf("Size of directoryrec: %d\n", sizeof(directoryrec));
    numdirs=num-2;
    printf("Directories: %lu\n",(num-2));
    for (loop=0;loop<num;loop++){
      if (fread(&dir,sizeof(directoryrec),1,infile)) {
            printf("Processing Dir #%u of %u\r",loop,(num-2));
            if (loop>0){
                if (true){
		sprintf(line,"\n==============================================================================\n");
                    fputs(line,outfile);
                    sprintf(line,"%s #%u (%s) \n",dir.name,loop,dir.filename);
                    fputs(line,outfile);
		sprintf(line,"==============================================================================\n");
                    fputs(line,outfile);

		sprintf(uploads,"/home/xenos/alcove/C:/WWIV/DATA/%s.dir",dir.filename);
                upfile=fopen(uploads,"r");
		if (upfile==NULL) {
		        printf("\nCould not open Uploads List  %s\n",uploads);
			return(1);
	        }
		fseek(upfile, 0L, SEEK_END);
		ups=ftell(upfile);
		rewind(upfile);
                numups=(ups/sizeof(uploadsrec)-1);
		sprintf(line,"\nNumber uploads: %d\n",numups);
		fputs(line,outfile);
		fread(&upload,sizeof(uploadsrec),1,upfile);
		for (j=0;j<numups+1;j++){
		  if (fread(&upload,sizeof(uploadsrec),1,upfile)) {
		    sprintf(line,"%3d %s - %s\n",j+1,upload.filename,upload.description);
		printf("%s - %s\n",upload.filename,upload.description);
		fputs(line,outfile);
		  }
		}
		  		fclose(upfile);

                    }
                }
            }
	}
    sprintf(line,"\n=================\nNumber of Dirs  %i\n",numdirs);
    fputs(line,outfile);
    fputs("\n\n\n",outfile);
    fclose(infile);
    fclose(outfile);
}
	
