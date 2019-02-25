#pragma pack(push, 1)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <stdarg.h>
#include <time.h>
#include <ctype.h>
#include <sys/stat.h>
#include "/home/xenos/wwv/github/wwiv/sdk/net.h"

#define MT_DESQVIEW 0x01
#define MT_WINDOWS  0x02
#define MT_OS2      0x04
#define MT_NB       0x40

#define MSGPRIVATE      0x0001          /* Private message              */
#define MSGCRASH        0x0002          /* High priority                */
#define MSGREAD         0x0004          /* Read by addressee            */
#define MSGSENT         0x0008          /* Has been sent                */
#define MSGFILE         0x0010          /* File attached to msg         */
#define MSGTRANSIT      0x0020          /* In transit                   */
#define MSGORPHAN       0x0040          /* Unknown node                 */
#define MSGKILL         0x0080          /* Kill after mailing           */
#define MSGLOCAL        0x0100          /* Message was entered here     */
#define MSGHOLD         0x0200          /* Hold for pickup              */
#define MSGUNUSED       0x0400          /* Unused                       */
#define MSGFREQ         0x0800          /* File request                 */
#define MSGRRREQ        0x1000          /* Return receipt request !!    */
#define MSGISRR         0x2000          /* Is return receipt !!         */
#define MSGAREQ         0x4000          /* Audit request !!             */
#define MSGFUPDREQ      0x8000          /* File update request !!       */

#define VERSION "1.50"                 /* WWIVToss Version, Verbose    */
#define VERSION1 1                      /* WWIVToss Version, Major      */
#define VERSION2 50                     /* WWIVToss Version, Minor      */
//#define BETA                          /* Beta or Release?             */
#define DATE "05/22/11"                 /* Release Date                 */
#define PROGRAM "WWIVTOSS"              /* Program Name ID              */
#define MAX_MSG_SIZE 32000              /* Maximum Message Size         */
const char *suffixes[8] = {
                            "SU", "MO", "TU", "WE", "TH", "FR", "SA", NULL
};

extern char  fido_title[81],             /* limited to 72 chars */
                fido_to_user[81],           /* limited to 36 chars */
                fido_from_user[81],         /* limited to 36 chars */
                curr_conf_name[81],         /* current conference fido-name */
                fido_date_line[81],         /* the date in ASCII */
                origi_node[80],             /* origin node */
                temp_dest[40],              /* destination node of packet */
                outbound_packet_name[80];   /* Name of outbound packet for this session */

typedef struct  /* Stoneage Type 2.0 */
{
    unsigned short int  orig_node,        /* Originating node           */
                        dest_node,        /* Destination node           */
                        year,             /* Year created (1993-nnnn)   */
                        month,            /* Month created (0-11)       */
                        day,              /* Day created (1-31)         */
                        hour,             /* Hour created (0-23)        */
                        minute,           /* Minute created (0-59)      */
                        second,           /* Second created (0-59)      */
                        baud,             /* Baudrate (unused)          */
                        packet_type,      /* Packet version (2)         */
                        orig_net,         /* Originating net            */
                        dest_net;         /* Destination net            */
    unsigned char       PCodeLo,          /* Product code (low-order)   */
                        PRevMajor,        /* Major version              */
                        Password[8];      /* Packet password            */
    unsigned short int  QMOrigZone,       /* Originating zone (QMail)   */
                        QMDestZone;       /* Destination zone (QMail)   */
    char                domain[8];        /* Network Domain             */
    unsigned short int  OrigZone,         /* Originating zone (FSC-39)  */
                        DestZone,         /* Destination zone (FSC-39)  */
                        OrigPoint,        /* Originating point (FSC-39) */
                        DestPoint;        /* Destination point (FSC-39) */
    unsigned long       LongData;         /* Product dependent information*/
} packet_header_struct;

typedef struct  {     /* FSC-0045 Type 2.2 */
    unsigned int        onode,
                        dnode,
                        opoint,
                        dpoint;
    char                zeros[8];
    unsigned int        subver,       /* 2 */
                        version,      /* 2 */
                        onet,
                        dnet;
    char                product,
                        rev_lev,
                        password[8];
    unsigned int        ozone,
                        dzone;
    char                odomain[8],
                        ddomain[8];
    long                specific;
} FSC45_header_struct;

typedef struct {               /* FSC-0039 Type 2.+ */
        unsigned int    orig_node,             /* originating node */
                        dest_node,             /* destination node */
                        year,                  /* 1989 - nnnnn */
                        month,
                        day,
                        hour,
                        minute,
                        second,
                        rate,                  /* unused */
                        ver,                   /* 2 */
                        orig_net,              /* originating net */
                        dest_net;              /* destination net */
        char            product,
                        rev_lev,               /* revision level */
                        password[8];
        unsigned int    qm_orig_zone,
                        qm_dest_zone;
        char            filler[2];
        unsigned int    capword2;
        char            product2,
                        rev_lev2;
        unsigned int    capword,
                        orig_zone,             /* originating zone */
                        dest_zone,             /* destination zone */
                        orig_point,            /* originating point */
                        dest_point;            /* destination point */
        long            pr_data;
} FSC39_header_struct;

typedef struct {               /* FSC-0048 Type 2.N */
        unsigned int    orig_node,             /* originating node */
                        dest_node,             /* destination node */
                        year,                  /* 1989 - nnnnn */
                        month,
                        day,
                        hour,
                        minute,
                        second,
                        rate,                  /* unused */
                        ver,                   /* 2 */
                        orig_net,              /* originating net */
                        dest_net;              /* destination net */
        char            product,
                        rev_lev,               /* revision level */
                        password[8];
        unsigned int    qm_orig_zone,
                        qm_dest_zone,
                        aux_net;
        unsigned int    capword2;
        char            product2,
                        rev_lev2;
        unsigned int    capword,
                        orig_zone,             /* originating zone */
                        dest_zone,             /* destination zone */
                        orig_point,            /* originating point */
                        dest_point;            /* destination point */
        long            pr_data;
} FSC48_header_struct;

typedef struct {
    int id;
    int orig_node;
    int dest_node;
    int orig_net;
    int dest_net;
    int attribute;
    int cost;
    char datetime[20];
} message_header_struct;

typedef struct
    {
    char    FromUser[36],               /*Name in By: field             */
            ToUser[36],                 /*Name in To: field             */
            Subject[72],                /*Name in Re: field             */
            Date[20];                   /*ASCIIz creation date          */
    unsigned short int
            TimesRead,                  /*Number of times message read  */
            DestNode,                   /*Destination node              */
            OrigNode,                   /*Originating node              */
            Cost,                       /*Cost to send message          */
            OrigNet,                    /*Originating net               */
            DestNet;                    /*Destination net               */
    unsigned long
            DateWritten,                /*Date message was written      */
            DateArrived;                /*Date message was read/sent    */

    unsigned short int
            ReplyTo,                    /*This is a reply to "ReplyTo"  */
            Attr,                       /*Attribute bits, see above     */
            ReplyNext;                  /*Next reply in thread          */
    } FIDOMSG;

typedef struct {
        char    msg_id[10],
                address[20];
} msgdupe;

typedef struct {
        char            bbs_name[81],
                        sysop_name[81],
                        system_address[20],
                        network[20],
                        version[10];
        unsigned long   date_installed,
                        date_notified;
        unsigned int    node,
                        registered;
} notify_rec;

#define MAX_NODES 20
#define NUM_ARCHIVERS 8 /* 1+Number of archivers (0=.PKT) */
/* ------------------------------------------------------------ */
/* Structure for nodes - Distribution, connects, etc.           */
/* ------------------------------------------------------------ */

typedef struct {
        short int     zone,                   /* Zone of Node         */
                net,                    /* Net of Node          */
                node,                   /* Node of Node         */
                point;                  /* Point of Node        */
} netnode;

/* ------------------------------------------------------------ */
/* Structure for Archivers                                      */
/* ------------------------------------------------------------ */
typedef struct {
        char    name[10],               /* Program ID           */
                archive_file[15],       /* Archive Program      */
                unarchive_file[15],     /* Unarchive Program    */
                archive_switches[30],   /* Archive Switches     */
		unarchive_switches[30]; /* Unarchive Switches   */
        char    reserved[50];           /* Reserved             */
} arc_dat;

/* ------------------------------------------------------------ */
/* Structure for determining where to route netmail             */
/*              Stored in WWIVTOSS.DAT                          */
/* ------------------------------------------------------------ */

typedef struct {
        int16_t     zone,                   /* Zone of Node         */
                net,                    /* Net of Node          */
                node,                   /* Node of Node         */
                point;                  /* Point of Node        */
        int16_t     route_zone;             /* Zone to route on     */
} route_mail;


/* ------------------------------------------------------------ */
/* Structure for areas                                          */
/*              Saved in AREAS.DAT                              */
/* ------------------------------------------------------------ */

typedef struct
{
	char            name[81],       /* Area Name                    */
			comment[81],    /* Comment on Area              */
                        origin[64],     /* Origin to use for this area  */
                        subtype[9],     /* WWIV Subtype To Use          */
                        directory[81];  /* Directory to .MSG section    */
        char            res_char[50];   /* Reserved Strings             */
        netnode         feed[MAX_NODES];/* Feed Structure               */
        short int             type;           /* Passthru, local, regular     */
        short int             alias_ok,       /* Ok to pass aliases?          */
                        pass_color,     /* Ok to pass WWIV Colors?      */
                        high_ascii,     /* Ok to pass High ASCII?       */
                        net_num,        /* WWIV Network Number          */
                        group,          /* Area Group #                 */
                        count,          /* Count of messages            */
                        active,         /* If 0, marks end of list      */
                        keep_tag,       /* Keep tagline?                */
                        def_origin,     /* Using Default Origin?        */
                        count_out,      /* Export Count                 */
                        translate,      /* Use Translation Tables       */
                        val_incoming;   /* Validate Incoming Messages?  */
        unsigned char   res_byte[18];   /* Reserved Bytes               */
} arearec;

/* ------------------------------------------------------------ */
/* Structure of nodes for connect/feed data                     */
/*              Saved in NODES.DAT                              */
/* ------------------------------------------------------------ */

 struct
{
        char            address[40],    /* Network Address of Node      */
                        sysop_name[81], /* Sysop of Node                */
                        pkt_pw[30],     /* Packet Password to Use       */
                        sysop_phone[15],/* Sysop's Voice Phone          */
                        domain[10],     /* Node's Domain Name           */
                        res_char[75];   /* Reserved Bytes               */
        unsigned char   compression;    /* Compression to use           */
        unsigned short    daynum,         /* Day Of The Year              */
                        counter;        /* Counter of sent packets      */
        char            areafix_pw[12]; /* Areafix Password             */
        short             archive_status; /* Packet Archive Status        */
        short            direct;         /* Direct Flag                  */
        short             allow_areafix;  /* Can this node Areafix?       */
        unsigned short  groups;         /* Groups this node is allowed  */
        short             packet_type;    /* Packet Type To Send          */
        char            res_bytes[24];  /* Reserved bytes               */
} nodes [20];

/* ------------------------------------ */
/* Structure for the Uplink Manager     */
/* ------------------------------------ */
 struct
{
        char            address[40],            /* Address of Uplink    */
                        areafix_prog[30],       /* Areafix Program      */
                        areafix_pw[30],         /* Areafix Password     */
                        areas_filename[81],     /* Areas Filename       */
                        origin_address[40],     /* Origin AKA To Use    */
                        res_char[80];           /* Reserved Characters  */
        short             add_plus,               /* Use '+' to add areas */
                        areas_file_type,        /* Areas File Type      */
                        unconditional,          /* Send Unconditioanlly */
                        res_int[20];            /* Reserved Integers    */
        unsigned short  groups;                 /* Groups Useable       */
} uplink[20];

/* ------------------------------------------------------------ */
/* Translation Table Structure.  Stored in XLAT.DAT             */
/* ------------------------------------------------------------ */

 struct
{
        unsigned char   in,     /* Character to change to - Incoming    */
                        out;    /* Character to change to - Outgoing    */
} xlat[225];

/* ------------------------------------------------------------ */
/* Structure for this system's AKA information.                 */
/*              Saved in WWIVTOSS.DAT as tosser_cfg.akas        */
/* ------------------------------------------------------------ */

typedef  struct {
        int16_t     zone,                   /* Zone of Node         */
                net,                    /* Net of Node          */
                node,                   /* Node of Node         */
                point;                  /* Point of Node        */
        int16_t     wwiv_node,              /* WWIV Network Node    */
                wwiv_netnum;            /* WWIV Network Number  */
} akas;


/* ------------------------------------------------------------ */
/* Structure for area group names                               */
/*              Stored in tosser_cfg                            */
/* ------------------------------------------------------------ */
typedef struct {
        char    group_id[2],            /* Group I.D. (A-Z)     */
                group_name[21];         /* Group Name           */
} groups;

/* ------------------------------------------------------------ */
/* Structure for main WWIVTOSS Configuration Data               */
/*              Saved as WWIVTOSS.DAT                           */
/* ------------------------------------------------------------ */


typedef struct
{
        char            bbs_name[81],           /* BBS Name             */
                        sysop_name[81],         /* SysOp Name           */
                        password[20],           /* WTSETUP Password     */
                        system_address[20],     /* Main Network Address */
                        inbound_path[64],       /* Inbound Packet Dir   */
                        outbound_path[64],      /* Outbound Packet Dir  */
                        outbound_temp_path[64], /* Temp Outbound Dir    */
                        netmail_path[64],       /* NetMail Dir          */
                        temp_path[64],          /* Temporary Path       */
                        badecho_path[64],       /* Where Bad Echos Go   */
                        origin_line[64],        /* Default Origin Line  */
                        log_file[81],           /* Path of log file     */
                        registration[80],       /* Registration Key     */
                        location[40];           /* Site Location        */
        char            reserved[100];          /* Reserved Bytes       */
        akas            aka_list[11];           /* List of AKAs         */
        route_mail      route_to[11];           /* Where to Route Mail  */
        arc_dat         archive[10];            /* Archive Information  */
	groups          group[26];              /* Group Information    */
        unsigned char   crash,                  /* Crash Mail Flag      */
                        direct,                 /* Direct Mail Flag     */
                        center,                 /* Allow centering?     */
                        delete_sent,            /* Delete SENT Mail?    */
                        import_recd,            /* Import Rec'd Mail?   */
                        use_areafix,            /* Use AREAFIX?         */
                        skip_mail,              /* Skip Importing Mail  */
                        notify,                 /* Notify Author Flag   */
                        default_compression,    /* Default Compression  */
                        registered,             /* Registered Flag      */
                        mailer,                 /* Mailer Type          */
                        installed,              /* Installed!           */
                        cleanup,                /* Cleanup Nets         */
                        kill_recd,              /* Delete Rec'd Mail?   */
                        num_areas,              /* Number of Areas      */
                        num_nodes,              /* Number of Nodes      */
                        set_immediate;          /* Use IMMEDIATE Flag?  */
//        unsigned long   date_installed;         /* Date installed       */
		time_t	date_installed;
        int             auto_add,               /* Auto Add Areas?      */
                        log_feeds,              /* Log Feed Echos?      */
                        add_hard_cr,            /* Add Hard C/R         */
                        add_line_feed,          /* Add Line Feed        */
                        high_ascii,             /* Pass High ASCII Mail */
                        soft_to_hard,           /* Soft To Hard C/R     */
                        hard_to_soft,           /* Hard To Soft C/R     */
                        add_soft_cr,            /* Add Soft C/R         */
                        route_me,               /* Route Mail?          */
                        log_days,               /* Days To Keep Log     */
                        color_to,               /* Colorize TO: Line?   */
                        initial_quote,          /* Initial Quote?       */
                        num_uplinks,            /* Number of uplinks    */
                        bounce_mail,            /* Bounce Bad Mail?     */
                        check_dupes,            /* Check Dupe Messages? */
                        pass_origin;            /* Use Existing Origin? */
        char            res_byte[46];           /* Reserved Bytes       */
} tosser_config;

/* ------------------------------------------------------------ */
/* Tracking structure for keeping track of incoming/outgoing    */
/* number of bytes for writing to NET.LOG                       */
/* ------------------------------------------------------------ */

 struct
{
        int     wwiv_node,                      /* Fake WWIV Node       */
                used,                           /* Did any traffic flow */
                out_used,                       /* Any Outbound?        */
                in_used;                        /* Any Inbound          */
        long    in_k,                           /* Amount Coming In     */
                out_k;                          /* Amount Going Out     */
        char    net_name[30];                   /* Network Name         */
} track[12];

/* ------------------------------------------------------------ */
/* Miscellaneous Global Options                                 */
/* ------------------------------------------------------------ */

int filelength(FILE *f){
		int len;
		fseek(f, 0L, SEEK_END);
		len=ftell(f);
		return(len);
};

// copied from dos.h by dj delorie

struct date {
  short da_year;
  char  da_day;
  char  da_mon;
};

struct time {
  unsigned char ti_min;
  unsigned char ti_hour;
  unsigned char ti_hund;
  unsigned char ti_sec;
};


struct tm *
getdateSystem (const char *d)
{
//#warning If warning messages about getdate appear, ignore them.
  return (getdate (d));
}

time_t
dostounix (struct date *d, struct time *t)
{
   struct tm utm, *ptm;
   time_t the_time = time (0);
 
   ptm = localtime (&the_time);
   memset (&utm, '\0', sizeof (struct tm));
   memcpy ((void *) &utm, (void *) ptm, sizeof (struct tm));
 
   utm.tm_sec = t->ti_sec;
   utm.tm_min = t->ti_min;
   utm.tm_hour = t->ti_hour;
   utm.tm_mday = d->da_day;
   utm.tm_mon = d->da_mon - 1;
   utm.tm_year = d->da_year - 1900;
 
   return (mktime (&utm));
}


#pragma pack(pop)
