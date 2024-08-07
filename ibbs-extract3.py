#!/usr/bin/env python3
######################################################################
# wwiv ibbs lastcaller extract:                                      #
#                                                                    #
# Usage: ibbs-extract3.py                                            #
#        will extract all ibbslastcall-data                          #
#        encoded in rot47 between MARKERS >>> BEGIN and >>> END      #
# Copyright (C) 2019-2023 Eric Manuel Pareja                         #
######################################################################
import codecs
import configparser
import json
import re
import subprocess
import sys
from binascii import *
from struct import *
from time import *

######################################################################
# requires python3-progress to be installed for pretty progress bars
# on debian: apt install python3-progress
#

try:
  from progress.bar import Bar
except ImportError:
  Bar=False

######################################################################
# read in wwiv.ini and replace the defaults
#
# todo: convert this to a .json config so we don't need wwiv.ini
#

config=configparser.ConfigParser(allow_no_value=True)
c=config.read("wwiv.ini")
if c==['wwiv.ini']:
  try:
    subs=config['wwiv5ibbslastcaller'].get('subs',"IBBSDAT,FSX_DAT").split(',')
    display=int(config['wwiv5ibbslastcaller'].get('display','11'))
  except KeyError:
    print("Oops. No wwiv5ibbslastcaller section in wwiv.ini.")
    exit()
else:
  print("Oops. Can't read wwiv.ini.")
  exit()

######################################################################
def get_host(subname):
  subs_json=json.load(open(f"{wwiv['config']['datadir']}/subs.json"))
  for i in subs_json['subs']:
    if len(i['nets'])>0:
      for j in range(len(i['nets'])):
         if i['nets'][j]['stype']==subname:
            return(int(i['nets'][j]['host']),i['nets'][j]['net_num'],i['filename'])
  return(-1,-1,-1)

def rot47(s):
  l=""
  for i in s:
    j=k=ord(i)
    if (j>32) and (j<80):
      k=j+47
    if (j>79) and (j<127):
      k=j-47
    if (k=="|"):
      k=ord(" ")
    if (k!="\r") and (k!="\n") and k!=13:
      l+=chr(k)
  return str(l)

wwiv=json.load(open("config.json","r"))
networkfile=f"{wwiv['config']['datadir']}networks.json"
n=json.load(open(networkfile,"r"))
systemname=wwiv['config']['systemname']
for sub in subs:
  tosys,net,filename=get_host(sub)
  if tosys>=0:
    networkname=f"{n['networks'][net]['name']}"
    networkdir=f"{n['networks'][net]['dir']}"
    s=""
    l=""
    o=subprocess.run(["wwivutil","messages","dump",filename],capture_output=True)
    lines=o.stdout.decode("latin_1")
    e=open(f"{networkdir}laston.txt","w+b") # get ready to write
    es=f"|#4{' ' * (29 - len(networkname))}InterBBS Last Callers for: {networkname}{' ' * (29 - (len(networkname) - len(networkname) % 2))}|#0\r\n"
    e.write(es.encode("latin_1"))
    es="|#7\xB3|#2Name/Handle  |#7\xB3|#2 Time |#7\xB3|#2  Date  |#7\xB3|#2 City                   |#7\xB3|#2 BBS                  |#7\xBA\r\n"
    e.write(es.encode("latin_1"))
    es="|#7\xC3"+("\xC4"*13) +"\xC5"+("\xC4"*6)+"\xC5"+("\xC4"*8)+"\xC5"+("\xC4"*24)+"\xC5"+("\xC4"*22)+"\xB6\r\n"
    e.write(es.encode("latin_1"))
    count=len(re.findall(">>> BEGIN",lines))
    start=0
    emit=0
    x=0
    y=""
    if Bar:
      bar=Bar(f"Munching on {networkname}...", max=count)
    for line in lines.split("\r\n"):
      if line==">>> END":
        emit=-1
        x=0
        y=s.split("\n")
        if start>=count-display and len(s)>0:
          try:
            e.write(f"|#7\xB3|#1{y[0]:13.13}|#7\xB3|#1{y[3]:6.6}|#7\xB3|#1{y[2]:8.8}|#7\xB3|#1{y[4]:^24.24}|#7\xB3|#1{y[1]:22.22}|#7\xBA|#0\r\n".encode("latin_1"))
          except:
            s=""
            break
        s=""
      if emit==1:
        s=str(s)+rot47(line)+"\n"
        x+=1
      if line[-9:]==">>> BEGIN":
        if Bar:
          bar.next()
        start+=1
        emit=1
        x=1
    e.write(("|#7\xD4"+("\xCD"*13)+"\xCF"+("\xCD"*6)+"\xCF"+("\xCD"*8)+"\xCF"+("\xCD"*24)+"\xCF"+("\xCD"*22)+"\xBC\r\n").encode("latin_1"))
    if Bar:
      bar.finish()
