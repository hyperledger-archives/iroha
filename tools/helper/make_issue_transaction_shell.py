# -*- coding: utf-8 -*-

import sys

(gm, ca) = sys.argv

cb = (ca[0]).upper() + ca[1:]
cc = ca.upper()

inf = open("template_make_issue_transaction.hpp")
outf = open("issue_transaction_"+ca+".hpp" ,'w')

for line in inf.readlines():
    line = line.replace('COMMAND_NAME_A',ca)\
        .replace('COMMAND_NAME_B',cb)\
        .replace('COMMAND_NAME_C',cc)

    if line.find("##CODE") != -1:
        for i in range(1,4):
            code_in = open("temp_"+str(i))
            outf.write(code_in.read())
    else:
        outf.write(line)