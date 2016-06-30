import os


def copyRes(filePath, targetPath):
    ext = os.path.splitext(filePath)[1]
    if (ext == ".png"):
        return os.path.join(targetPath, "drawable")
    return targetPath
