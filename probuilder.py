#!/usr/bin/python
import re
import os
import sys
import json

assert(len(sys.argv) == 3)
engineDir = os.path.dirname(os.path.realpath(sys.argv[0]))
templateDir = os.path.join(engineDir,"templates",sys.argv[2])
projectDir = os.path.join(os.path.realpath(os.getcwd()),"projects",sys.argv[2])
configPath = os.path.realpath(sys.argv[1])

print("Template: " + templateDir)
print("Target: " + projectDir)
print("Config: " + configPath)

jsonFile = open(configPath,'r')
config = json.load(jsonFile)

def replace(base, config, inData):
    outData = inData;
    for key in config:
        if (isinstance(config[key],dict)):
            outData = replace(key + ".", config[key], outData)
        else:
            outData = re.sub("<\?" + base + r""+ key + "\?>",config[key],outData)
    return outData


for path, dirs, files in os.walk(templateDir):
    for filename in files:
        inPath = os.path.join(path, filename)
        outPath = os.path.join(projectDir, filename)
        outDir = os.path.dirname(outPath);
        with open(inPath, 'r') as f:
            inData = f.read();
            outData = replace("",config,inData)
            if not os.path.exists(outDir):
                os.makedirs(outDir)
            out = open(outPath, 'w')
            out.write(outData);
