#!/usr/bin/python
import re
import os
import sys
import json

import tools


def run(argv, projectDir, engineDir):
    if (len(argv) < 2):
        print("Specify a project name.")
        exit(1)
    if (projectDir is None):
        print(
            "Project was initialized earlier.\n"
            "Remove config.json to initialize it again.")
        exit(2)
    else:
        projectDir = "."

    templateDir = os.path.join(engineDir, "templates/project")
    projectName = argv[1]
    config = {"name": projectName}

    tools.replaceAll(templateDir, projectDir, config)
