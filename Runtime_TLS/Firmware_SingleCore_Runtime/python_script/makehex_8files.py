#!/usr/bin/env python3
#
# This is free and unencumbered software released into the public domain.
#
# Anyone is free to copy, modify, publish, use, compile, sell, or
# distribute this software, either in source code form or as a compiled
# binary, for any purpose, commercial or non-commercial, and by any
# means.

from sys import argv

binfile = argv[1]
nwords = int(argv[2])
instr_data_type = int(argv[3])

with open(binfile, "rb") as f:
    bindata = f.read()

assert len(bindata) < 4*nwords
# assert len(bindata) % 4 == 0

print("DEPTH=16384;")
print("WIDTH=8;")
print("ADDRESS_RADIX=HEX;")
print("DATA_RADIX=HEX;")
print("CONTENT")
print("BEGIN")

j=0

if(instr_data_type == 0):
    for i in range(nwords // 2):
        if i <= len(bindata) // 4:
            w = bindata[4*i : 4*i+4]
            print("%04x:%02x;" % (i, w[0]))
        else:
            print("%04x:%02x;" % (i, 0))

elif(instr_data_type == 1):
    for i in range(nwords // 2):
        if i < len(bindata) // 4:
            w = bindata[4*i : 4*i+4]
            print("%04x:%02x;" % (i, w[1]))
        elif (len(bindata) % 4 == 1) and (i == len(bindata) // 4):
            w = bindata[4*i : 4*i+4]
            print("%04x:%02x;" % (i, w[1]))
        else:
            print("%04x:%02x;" % (i, 0))

elif(instr_data_type == 2):
    for i in range(nwords // 2):
        if i < len(bindata) // 4:
            w = bindata[4*i : 4*i+4]
            print("%04x:%02x;" % (i, w[2]))
        elif (len(bindata) % 4 == 1) and (i == len(bindata) // 4):
            w = bindata[4*i : 4*i+4]
            print("%04x:%02x;" % (i, w[2]))
        elif (len(bindata) % 4 == 2) and (i == len(bindata) // 4):
            w = bindata[4*i : 4*i+4]
            print("%04x:%02x;" % (i, w[2]))
        else:
            print("%04x:%02x;" % (i, 0))

elif(instr_data_type == 3):
    for i in range(nwords // 2):
        if i < len(bindata) // 4:
            w = bindata[4*i : 4*i+4]
            print("%04x:%02x;" % (i, w[3]))
        elif (len(bindata) % 4 == 1) and (i == len(bindata) // 4):
            w = bindata[4*i : 4*i+4]
            print("%04x:%02x;" % (i, w[3]))
        elif (len(bindata) % 4 == 2) and (i == len(bindata) // 4):
            w = bindata[4*i : 4*i+4]
            print("%04x:%02x;" % (i, w[3]))
        elif (len(bindata) % 4 == 3) and (i == len(bindata) // 4):
            w = bindata[4*i : 4*i+4]
            print("%04x:%02x;" % (i, w[3]))
        else:
            print("%04x:%02x;" % (i, 0))

elif(instr_data_type == 4):
    for i in range((nwords // 2), nwords):
        if i < len(bindata) // 4:
            w = bindata[4*i: 4*i+4]
            print("%04x:%02x;" % (j, w[0]))
        else:
            print("%04x:%02x;" % (j, 0))
        j += 1

elif(instr_data_type == 5):
    for i in range((nwords // 2), nwords):
        if i < len(bindata) // 4:
            w = bindata[4*i : 4*i+4]
            print("%04x:%02x;" % (j, w[1]))
        elif (len(bindata) % 4 == 1) and (i == len(bindata) // 4):
            w = bindata[4*i : 4*i+4]
            print("%04x:%02x;" % (j, w[1]))
        else:
            print("%04x:%02x;" % (j, 0))
        j += 1

elif(instr_data_type == 6):
    for i in range((nwords // 2), nwords):
        if i < len(bindata) // 4:
            w = bindata[4*i : 4*i+4]
            print("%04x:%02x;" % (j, w[2]))
        elif (len(bindata) % 4 == 1) and (i == len(bindata) // 4):
            w = bindata[4*i : 4*i+4]
            print("%04x:%02x;" % (j, w[2]))
        elif (len(bindata) % 4 == 2) and (i == len(bindata) // 4):
            w = bindata[4*i : 4*i+4]
            print("%04x:%02x;" % (j, w[2]))
        else:
            print("%04x:%02x;" % (j, 0))
        j += 1

elif(instr_data_type == 7):
    for i in range((nwords // 2), nwords):
        if i < len(bindata) // 4:
            w = bindata[4*i : 4*i+4]
            print("%04x:%02x;" % (j, w[3]))
        elif (len(bindata) % 4 == 1) and (i == len(bindata) // 4):
            w = bindata[4*i : 4*i+4]
            print("%04x:%02x;" % (j, w[3]))
        elif (len(bindata) % 4 == 2) and (i == len(bindata) // 4):
            w = bindata[4*i : 4*i+4]
            print("%04x:%02x;" % (j, w[3]))
        elif (len(bindata) % 4 == 3) and (i == len(bindata) // 4):
            w = bindata[4*i : 4*i+4]
            print("%04x:%02x;" % (j, w[3]))
        else:
            print("%04x:%02x;" % (j, 0))
        j += 1
print("END;")