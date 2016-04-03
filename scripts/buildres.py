#!/usr/bin/python
import os
import sys

import tools


def run(argv, projectDir, engineDir):
    engineResDir = os.path.join(engineDir, "res")
    projectResDir = os.path.join(projectDir, "res")
    templateConfigDir = os.path.join(engineDir, "templates/config/")
    outResDir = os.path.join(projectDir, "build/res/")
    outSrcDir = os.path.join(projectDir, "build/src/")

    if not os.path.exists(outResDir):
        os.makedirs(outResDir)

    if not os.path.exists(outSrcDir):
        os.makedirs(outSrcDir)

    resDirs = [engineResDir, projectResDir]

    print "Resource processing..."
    for resDir in resDirs:
        tools.copyAll(resDir, outResDir)

    print "Config generation..."
    config = {"revision": os.popen("git describe --always").read().strip()}
    tools.replaceAll(templateConfigDir, outSrcDir, config)
