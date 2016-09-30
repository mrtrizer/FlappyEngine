import os
import sys
import json

import tools


def genModules(argv, projectDir, engineDir, config, modules):
    print "genModules"
    templateDir = os.path.join(projectDir,
                               config["engine"],
                               "templates/modules",
                               argv[1])

    print("Template: " + templateDir)

    targetDir = os.path.join(projectDir, "targets", argv[1])

    functions = {"projectDir": projectDir,
                 "engineDir": engineDir}

    commonMethods = tools.loadTargetAll(projectDir,
                                        engineDir,
                                        "methods",
                                        config)

    functions.update(commonMethods.__dict__)

    targetSpec = tools.loadTargetSpec(projectDir,
                                      engineDir,
                                      argv[1],
                                      "methods",
                                      config)

    for moduleConfig in modules:
        functions["modulePath"] = moduleConfig["path"]
        tools.replaceAll(templateDir, targetDir, moduleConfig, functions)
