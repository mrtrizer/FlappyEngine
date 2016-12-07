#!/usr/bin/python
import os
import sys
import json
import re
import md5

import tools


def fileList(sourceDir, targetExt=None):
    fileList = []
    for path, dirs, files in os.walk(sourceDir):
        for fileName in files:
            ext = os.path.splitext(fileName)[1]
            filePath = os.path.join(os.path.relpath(path, sourceDir), fileName)
            if ((ext == targetExt) or (targetExt is None)):
                fileList.append(filePath)
    return fileList

def fileListExt(rootPath, sourceDirs, targetExt=None):
    fileList = []
    for relSourceDir in sourceDirs:
        sourceDir = os.path.join(rootPath, relSourceDir);
        for path, dirs, files in os.walk(sourceDir):
            for fileName in files:
                ext = os.path.splitext(fileName)[1]
                filePath = os.path.join(os.path.relpath(sourceDir, rootPath), fileName)
                if ((ext == targetExt) or (targetExt is None)):
                    fileList.append(filePath)
    return fileList


def printList(list, template, exclude=None):
    text = ""
    for item in list:
        line = re.sub("\*", item, template)
        passLine = False
        if exclude is not None:
            for pattern in exclude:
                pattern = re.sub("\*\*", ".*", pattern)
                if (len(re.findall(pattern, line)) > 0):
                    passLine = True
        if not passLine:
            text += line
    return text
