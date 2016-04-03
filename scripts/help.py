#!/usr/bin/python
import os
import sys
import json
import re

import tools


def run(argv, projectDir, engineDir):
    """Prints help doc from doc/commands"""
    text = open(os.path.join(engineDir,
                             "doc/commands",
                             argv[1] + ".md"),
                'r').read()
    text = re.sub("^.*\n\=*", "", text)
    print text
