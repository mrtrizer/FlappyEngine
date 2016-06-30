#!/usr/bin/python
import os
import sys
import json
import re
import md5

import tools

def printXCodeFileList(list):
    text = ""
    for item in list:
        line = re.sub("\$", os.path.basename(item), "% /* $ */ = {isa = PBXFileReference; fileEncoding = 4; path = $; sourceTree = \"<group>\"; };\n")
        line = re.sub("\%", md5.new(item).hexdigest()[:24].upper(), line)
        text += line
    return text


def printXCodeHashList(list):
    text = ""
    for item in list:
        line = re.sub("\$", os.path.basename(item), "% /* $ */,\n")
        line = re.sub("\%", md5.new(item).hexdigest()[:24].upper(), line)
        text += line
    return text
