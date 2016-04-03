#!/usr/bin/python
import os
import sys
import json

import tools


def run(argv, projectDir, engineDir):
    templateDir = os.path.join(engineDir, "templates/targets", argv[1])
    if (not os.path.isdir(templateDir)):
        print("Target is not supported: " + argv[1])
        exit(2)
    configPath = os.path.join(projectDir, "config.json")

    jsonFile = open(configPath, 'r')
    config = json.load(jsonFile)
    targetDir = os.path.join(projectDir, "targets", argv[1])

    print("Template: " + templateDir)
    print("Target: " + targetDir)
    print("Config: " + configPath)

    tools.replaceAll(templateDir, targetDir, config)
