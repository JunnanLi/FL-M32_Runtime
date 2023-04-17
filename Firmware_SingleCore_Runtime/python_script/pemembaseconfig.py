#!/usr/bin/env python3
#
# This is free and unencumbered software released into the public domain.
#
# Anyone is free to copy, modify, publish, use, compile, sell, or
# distribute this software, either in source code form or as a compiled
# binary, for any purpose, commercial or non-commercial, and by any
# means.

from sys import argv

hexfile     = argv[1]
instrbase   = int(argv[3])
instrlen    = int(argv[5])
database    = int(argv[7])
datalen     = int(argv[9])

# print('Testing by LCL7')

f = open(hexfile, 'r')
lines = f.readlines()
f.close()

# print(type(lines))
# print(len(lines))

for tmp_idx in range(len(lines)):
    for tmp_i in range(tmp_idx, len(lines)):
        if 'ram (rwxai)' in lines[tmp_i]:
            lines[tmp_i] = '\t' + 'ram (rwxai) : '+ \
                            'ORIGIN = ' + hex(instrbase*4) + \
                            ', LENGTH = ' + hex(instrlen*4) + '\n'
            break
    for tmp_j in range(tmp_idx, len(lines)):
        if 'ram_data (rwxai)' in lines[tmp_j]:
            lines[tmp_j] = '\t' + 'ram_data (rwxai) : '+ \
                            'ORIGIN = ' + hex(database*4) + \
                            ', LENGTH = ' + hex(datalen*4) + '\n'
            break
    break

f_w = open(hexfile, 'w')
f_w.writelines(lines)
f_w.close()