import tools


def copyRes(resPath, targetPath):
    tools.copyAll(resPath, os.path.join(targetPath, "./res"))
