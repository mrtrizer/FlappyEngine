import re

def replace(base, config, inData):
    outData = inData;
    for key in config:
        if (isinstance(config[key],dict)):
            outData = replace(key + ".", config[key], outData)
        else:
            outData = re.sub("<\?" + base + r""+ key + "\?>",config[key],outData)
    return outData
