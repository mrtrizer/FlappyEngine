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
                             argv[1] + ".dox"),
                'r').read()
    text = re.findall("/\*!((.|\n)*)\*/", text)[0][0]
    text = re.sub("@.*", "", text)
    text = re.sub("^\s*\n", "", text)
    print text
