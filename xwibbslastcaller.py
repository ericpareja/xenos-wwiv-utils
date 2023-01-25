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

# replace below with the subtype where to post ibbs data to
sub="IBBSDAT"
bbsaddress="https://wwivbbs.org"

def get_host(subname):
  subs_json=json.load(open("%s/subs.json" % (wwiv['config']['datadir'])))
  for i in subs_json['subs']:
    if len(i['nets'])>0:
      for j in range(len(i['nets'])):
         if i['nets'][j]['stype']==subname:
            return(int(i['nets'][j]['host']),i['nets'][j]['net_num'])
  return(0)

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
tosys,net=get_host(sub)
mysys=n['networks'][net]['sysnum']
doorsys=readdoorsys(sys.argv[1])
user=doorsys[35].strip()
city=doorsys[10].strip()
netdir=n['networks'][net]['dir']
title=title+"\0"
sender="ibbslastcall"
date=ctime()
print("User:",user)
print("BBS Name:",systemname)
print("Date:",strftime("%m/%d/%y"))
print("Time:",strftime("%H:%M%P"))
print("City:",city)
print("OS:",platform.system(),platform.machine())
print("BBS Address:",bbsaddress)
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
payload=sub+"\0"+title+sender+"\r\n"+date+"\r\n"+message+"\r\n"
LOCAL=netdir+"local.net"
OUTBOUND=netdir+"p0.net"
writepacket(LOCAL,0,0,mysys,1,26,0,0,0,0,0,payload)
writepacket(OUTBOUND,tosys,0,mysys,1,26,0,0,0,0,0,payload)
subprocess.call("networkc")
