#!/usr/bin/python
import re
import os
import json

FROM = "templates/cmake/"
TO = "../projects/cmake/"
CONFIG = "../"

configPath = os.path.join(CONFIG,"config.json")
jsonFile = open(configPath,'r')
config = json.load(jsonFile)
print(config)

def replace(base, config, inData):
    outData = inData;
    for key in config:
        if (isinstance(config[key],dict)):
            outData = replace(key + ".", config[key], outData)
        else:
            print(r"<?" + base + key + r"?>" + " : " + config[key])
            outData = re.sub("<\?" + base + r""+ key + "\?>",config[key],outData)
    return outData


for path, dirs, files in os.walk(FROM):
    for filename in files:
        inPath = os.path.join(path, filename)
        outPath = os.path.join(TO, filename)
        outDir = os.path.dirname(outPath);
        print("inPath: " + inPath + " outPath: " + outPath)
        with open(inPath, 'r') as f:
            inData = f.read();
            outData = replace("",config,inData)
            #print(outData)
            #outData = re.findall(r"<\?.*?\?>",inData)
            #print(outData)
            if not os.path.exists(outDir):
                os.makedirs(outDir)
            out = open(outPath, 'w')
            out.write(outData);
        #     data = re.sub(r'(\s*function\s+.*\s*{\s*)',
        #         r'\1echo "The function starts here."',
        #         f.read())
        # with open(fullpath, 'w') as f:
        #     f.write(data)
