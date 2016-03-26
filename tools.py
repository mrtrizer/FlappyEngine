import re
import os
import json


class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'


def replace(base, config, inData):
    outData = inData
    for key in config:
        if (isinstance(config[key], dict)):
            outData = replace(key + ".", config[key], outData)
        else:
            outData = re.sub("<\?" + base + r"" + key + "\?>", config[key], outData)
    return outData


def replaceAll(templateDir, targetDir, config):
    print "Parameters:"
    print json.dumps(config, sort_keys=True, indent=4, separators=(',', ': '))
    print "Processing..."
    for path, dirs, files in os.walk(templateDir):
        for fileName in files:
            print bcolors.OKGREEN + " [PROC] " + bcolors.ENDC + fileName
            relPath = os.path.relpath(path, templateDir)
            inPath = os.path.join(path, fileName)
            outPath = os.path.join(targetDir, relPath, fileName)
            outDir = os.path.dirname(outPath)
            with open(inPath, 'r') as f:
                inData = f.read()
                outData = replace("", config, inData)
                if not os.path.exists(outDir):
                    os.makedirs(outDir)
                out = open(outPath, 'w')
                out.write(outData)
