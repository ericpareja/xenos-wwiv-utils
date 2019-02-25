#include "/home/xenos/wwv/github/wwiv/sdk/vardec.h"
#include "linuxvars.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <menu.h>
#include <ncurses.h>
#define _DEBUG 1

/* ------------------------------------------------ */
/* Reads configuration options from WWIVTOSS.DAT    */
/* ------------------------------------------------ */
#define ESC 27
//#define ESC KEY_EXIT
#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
//tosser_config cfg;
configrec syscfg;
subboardrec sub;
typedef struct {
    char filename[10];
} subfilesrec;
subfilesrec subfiles;
postrec p;
subfile_header_t subfile_header;
int num_subs,num_posts;
#define WWIVCONFIG "/home/xenos/alcove/config.dat"
#define WWIVHOME "/home/xenos/alcove/"

const char *
yn (int yn) {
    if (yn) {
        return ("Yes");
    } else {
        return ("No ");
    }
}



void
read_wwiv_config (void) {
    FILE *configfile;

    configfile = fopen (WWIVCONFIG, "r");
    if (configfile <= 0) {
//        endwin ();
        printf ("Error opening WWIV CONFIG.DAT!\r\n");
        exit (1);
    }
    fread (&syscfg, 1, sizeof (configrec), configfile);
    fclose (configfile);
}

void
print_wwiv_config(void) {
    printf("System Name: %s\n",syscfg.systemname);
    printf("SysOp Name: %s\n",syscfg.sysopname);
    printf("   MSGS Directory: %s\n",syscfg.msgsdir);
    printf("   DATA Directory: %s\n",syscfg.datadir);
    printf("WWIV Registration: %d\n",syscfg.wwiv_reg_number);
    printf("System Config: %d\n",syscfg.sysconfig);
}

int get_num_posts(int subnum, int *bestof, int *droplist) {
    int np;
    char line[161], *p;
    FILE *fp;
    np=20; // number of posts to send
    *bestof=-1;
    *droplist=0;
//	if ((fp=fopen("autolist.dat","rt")) != NULL) {
//		while
}

int get_num_subs(void) {
    char s[81];
    int i;
    FILE *f;

    sprintf(s,"%s%s/subs.dat",WWIVHOME,syscfg.datadir);
    if ((f=fopen(s,"r"))==NULL) {
        fclose(f);
        exit(1);
    }
    fseek(f, 0L, SEEK_END);
    num_subs=ftell(f)/sizeof(subboardrec);
    fclose(f);
    return(num_subs);
}

void
get_post(uint32_t post) {
    /*

    oops... we need to do the gatseclen thingy
       char s[81];
       int i;
       FILE *f;
       messagerec m;

       sprintf(s,"%s%s%s.dat",WWIVHOME,syscfg.msgsdir,sub.filename);
       if ((f=fopen(s,"r"))==NULL) {
                    printf("Closing %s\n.",s);
                    fclose(f);
                    exit(1);
                }
       fseek(data,,SEEK_SET);
       fclose(data);
    */

}

void
get_subfiles(void) {
    char s[81];
    int i;
    FILE *fp,*msgs,*data;

    get_num_subs();
    printf("There are %d subs.\n",num_subs);

    sprintf(s,"%s%s/subs.dat",WWIVHOME,syscfg.datadir);
    if ((fp=fopen(s,"r"))==NULL) {
        fclose(fp);
        exit(1);
    }
    for (i=0; i<num_subs; i++) {
        fread(&sub, 1, sizeof(subboardrec), fp);
        printf("Subboard: #%d\n",i);
        printf("Name: %s\n",sub.name);
        printf("Sub Data: %s%s%s.sub\n",WWIVHOME,syscfg.datadir,sub.filename);
        sprintf(s,"%s%s%s.sub",WWIVHOME,syscfg.datadir,sub.filename);
        printf("Opening %s\n",s);
        if ((data=fopen(s,"r"))==NULL) {
            fclose(data);
            exit(1);
        }
        fseek(data,0L,SEEK_END);
        printf("Size of data: %ld\n", ftell(data) );
        num_posts=ftell(data)/sizeof(postrec)-1;
        printf("Number of posts: %d\n", num_posts);
        fseek(data,0L,SEEK_SET);
        for (int j=0; j<num_posts; j++) {
            fread(&p, 1, sizeof(postrec), data);
	    if (j) {
              printf("#%d: %s: From: %d@%d\n",j,p.title,p.owneruser,p.ownersys);
              printf("Message stored as: %d in %s\n",p.msg.stored_as,s);
              get_post(p.msg.stored_as);
	    }
        }
        fclose(data);


        /*
                if (num_posts) {
                    printf("Sub Msgs: %s%s%s.dat\n",WWIVHOME,syscfg.msgsdir,sub.filename);
                    sprintf(s,"%s%s%s.dat",WWIVHOME,syscfg.msgsdir,sub.filename);
                    printf("Opening %s\n",s);
                    if ((data=fopen(s,"r"))==NULL) {
                        printf("Closing %s\n.",s);
                        fclose(data);
                        exit(1);
                    }
                    printf("Seeking %s\n.",s);
                    fseek(data,0L,SEEK_END);
                    printf("Size of MSGS: %ld\n", ftell(data) );
                    fclose(data);
                }
        */
        printf("Key: %c\n",sub.key);
        printf("Max messages: %d\n",sub.maxmsgs);
        printf("Storage Type: %d\n",sub.storage_type);
//        dump_sub(i);
    }

    fclose(fp);
}

int
main (int argc, char *argv[]) {
    int bDone = 0, x = 0;
    FILE *fp, *f, *f1;

    read_wwiv_config ();
    print_wwiv_config();
    get_subfiles();

    return (0);
}
