#!/usr/bin/python
import os
import sys
import json

import tools


def run(argv, projectDir, engineDir):
    targetDir = os.path.join(projectDir, "targets/", argv[1], "res/")
    resDir = os.path.join(projectDir, "build/res/")
    targetResDir = os.path.join(projectDir, "targets/", argv[1], "res/")

    if not os.path.exists(targetResDir):
        os.makedirs(targetResDir)

    print "Resource copy to target..."
    print resDir
    print targetResDir
    tools.copyAll(resDir, targetResDir)
