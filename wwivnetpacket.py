#!/usr/bin/env python3
from struct import *
from time import *
from binascii import *
import sys
showmessage=0
strfile = "S60.NET"
if len(sys.argv) > 0:
	strfile = sys.argv[1]
strformat = "<HHHHHHHIIH"
def readtest(f):
  s=f.tell()
  buffer = f.read(24)
  tosys,touser,fromsys,fromuser,main_type,minor_type,list_len,daten,length,method=unpack(strformat,buffer)
  if list_len > 0:
    tolist = f.read(list_len * 2)
  buffer = f.read(length)
  e=f.tell()
  print("chunk start-end: %d-%d/%d date: %s" % (s+1,e,L,ctime(daten)))
  print("from: %d@%d -> %d@%d type: %d.%d length: %d list length: %d method: %d" % (fromuser,fromsys,touser,tosys,main_type,minor_type,length,list_len,method))
  if showmessage:
    print("Message:")
    print(buffer)
    print("")


with open(strfile,"br") as f:
  print("\npacket name: %s" % (strfile))
  f.seek(0,2)
  L=f.tell()
  f.seek(0)
  while f.tell() < L:
    readtest(f)

#    a=input()

  
#\nfrom: %d@%d\ntype: %d.%d\nlist_len: %d\ndaten %d\nlength %d\nmethod %d\n" % tosys,touser,fromsys,fromuser,main_type,minor_type,list_len,daten,length,method)

