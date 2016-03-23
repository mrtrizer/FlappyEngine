#!/usr/bin/python
import re
import os

PATH = "/home/tirengarfio/Desktop/pruebaba"

for path, dirs, files in os.walk(PATH):
    for filename in files:
        fullpath = os.path.join(path, filename)
        with open(fullpath, 'r') as f:
            data = re.sub(r'(\s*function\s+.*\s*{\s*)',
                r'\1echo "The function starts here."',
                f.read())
        with open(fullpath, 'w') as f:
            f.write(data)

