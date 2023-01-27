#!/usr/bin/env python3
#
# xwibbslastcaller.py by xenos@aliens.ph
#
# this is xenos' wwiv 5 ibbs lastcaller python script
#
# copyright (c) 2019-2023 Eric Manuel Pareja
# license: GNU Public License 2
#################################################################

from struct import *
from time import *
from binascii import *
import sys
import subprocess
import json
import platform
import configparser

# read in wwiv.ini and replace the defaults
config=configparser.ConfigParser(allow_no_value=True)
config.read("wwiv.ini")
bbsaddress=config['wwiv5ibbslastcaller'].get('bbsaddress','https://wwivbbs.org')
dontshowsl=int(config['wwiv5ibbslastcaller'].get('dontshowsl','256'))
subs=config['wwiv5ibbslastcaller'].get('subs',"IBBSDAT,FSX_DAT").split(',')

def get_host(subname):
  subs_json=json.load(open("%s/subs.json" % (wwiv['config']['datadir'])))
  for i in subs_json['subs']:
    if len(i['nets'])>0:
      for j in range(len(i['nets'])):
         if i['nets'][j]['stype']==subname:
            return(int(i['nets'][j]['host']),i['nets'][j]['net_num'])
  return(-1,-1)

def writepacket(f,tosys,touser,fromsys,fromuser,main_type,minor_type,list_len,daten,length,method,message):
  # wwivpacketformat is the header format
  wwivpacketformat = "<HHHHHHHIIH"
  length=len(message)
  daten=int(time())
  packetout=pack(wwivpacketformat,tosys,touser,fromsys,fromuser,main_type,minor_type,list_len,daten,length,method)
  packetout=packetout+message.encode()
  out=open(f,"ab")
  out.write(packetout)
  out.close

def rot47(s):
  l=""
  for i in s:
    j=k=ord(i)
    if (j>32) and (j<80):
      k=j+47
    if (j>79) and (j<127):
      k=j-47
    l+=chr(k)
  return l

def readdoorsys(f):
  door=open(f)
  doorsys=door.readlines()
  return doorsys

wwiv=json.load(open("config.json","r"))
title="ibbslastcall-data"
networkfile="%s/networks.json" % (wwiv['config']['datadir'])
n=json.load(open(networkfile,"r"))
systemname=wwiv['config']['systemname']
doorsys=readdoorsys(sys.argv[1])
sl=int(doorsys[14].strip())
user=doorsys[35].strip()
city=doorsys[10].strip()
title=title+"\0"
sender="ibbslastcall"
date=ctime()
m=">>> BEGIN\n"
m=m+rot47(user)+"\n"                                             # User Handle
m=m+rot47(systemname)+"\n"                                       # BBS Name
m=m+rot47(strftime("%m/%d/%y"))+"\n"                             # Current Date
m=m+rot47(strftime("%H:%M%P"))+"\n"                              # Current Time
m=m+rot47(city)+"\n"                                             # User City
m=m+rot47("%s %s" % (platform.system(),platform.machine()))+"\n" # System OS
m=m+rot47(bbsaddress)+"\n"                                       # BBS Address
m=m+">>> END\n\n"
m=m+"by wwiv5ibbslastcall"
message=m.replace("\n","\r\n")
for sub in subs:
  payload=sub+"\0"+title+sender+"\r\n"+date+"\r\n"+message+"\r\n"
  tosys,net=get_host(sub)
  if tosys >= 0 :
    mysys=n['networks'][net]['sysnum']
    netdir=n['networks'][net]['dir']
    LOCAL=netdir+"local.net"
    OUTBOUND=netdir+"p0.net"
    if sl < dontshowsl:
      writepacket(LOCAL,0,0,mysys,1,26,0,0,0,0,0,payload)
      writepacket(OUTBOUND,tosys,0,mysys,1,26,0,0,0,0,0,payload)
      subprocess.call("networkc")
  else:
    payload=f"\r\nYou aren't subscribed to {sub}.\r\n"
    # we use the first netdir to send SSM
    mysys=n['networks'][0]['sysnum']
    writepacket(n['networks'][0]['dir']+"local.net",mysys,1,mysys,1,15,0,0,0,0,0,payload)
