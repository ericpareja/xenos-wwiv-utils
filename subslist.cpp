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
int numsubs;

int main(int argc, char *argv[])
{
    FILE *infile,*outfile;
    char subfile[81],output[81];
    output[0]=0;
// = "/home/xenos/alcove/C:/WWIV/DATA/user.lst";
    switch ( argc ) {
      case 1:
//        file=stdin;
        break;
      case 2:
        strcpy(subfile,argv[1]);
        break;
      case 3:
        strcpy(subfile,argv[1]);
        strcpy(output,argv[2]);
        break;
      default:
        break;
	}



    int i;
    unsigned int loop,num=0;
    unsigned long len;
    subboardrec sub;
    char line[81];

//    if ((f=open(subfile,O_RDWR|O_BINARY,S_IREAD|S_IWRITE))<=0) {
		infile=fopen(subfile,"r");
		if (infile==NULL) {
        printf("\nCould not open Sub Board List  %s\n",subfile);
		return(1);
        };

    if ((outfile=fopen(output,"w"))==NULL) {
        printf("\nCould not open SUB listing text file %s\n",subfile);
        return(1);
        };
		fseek(infile, 0L, SEEK_END);
		len=ftell(infile);
//		fseek(infile, 0L, SEEK_SET);
		rewind(infile);
//    len=filelength(infile);
    num=(len/sizeof(subboardrec))+!(len%sizeof(subboardrec));
    printf("Size of subboardrec: %d\n", sizeof(subboardrec));
    numsubs=num-2;
		printf("Users: %u\n",(num-2));
    for (loop=0;loop<num;loop++){
//        if (sizeof(subboardrec)==read(infile,&sub,sizeof(subboardrec))) {
		if (fread(&sub,sizeof(subboardrec),1,infile)) {
            printf("Processing Sub #%u of %u\r",loop,(num-2));
            if (loop>0){
                if (true){
					sprintf(line,"\n=================================================\n");
                    fputs(line,outfile);
                    sprintf(line,"%s #%u (%s) Key: %c\n\n",sub.name,loop,sub.filename,sub.key);
                    fputs(line,outfile);

                    }
                }
            }
	}
    sprintf(line,"\n=================\nNumber of Subs  %i\n",numsubs);
    fputs(line,outfile);
    fputs("\n\n\n",outfile);
    fclose(infile);
    fclose(outfile);
}
	
