{?printList(include,"INCLUDEPATH +=  ../../" + path + "/*\n")?}

{?printList(fileList(projectDir + "/" + path + "/tests", ".cpp"),"SOURCES += ../../" + path + "/tests/*\n")?}

{?printList(fileList(projectDir + "/" + path + "/tests", ".h"),"HEADERS += ../../" + path + "/tests/*\n", src_exclude )?}

{?printList(fileList(projectDir + "/" + path + "/src/", ".cpp"),"SOURCES += ../../" + path + "/src/*\n", src_exclude )?}

{?printList(fileList(projectDir + "/" + path + "/src/", ".h"),"HEADERS += ../../" + path + "/src/*\n", src_exclude )?}

OTHER_FILES += {?projectDir + "/" + path + "/config.json"?}
