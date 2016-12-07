{?printList(include,"INCLUDEPATH +=  ../../" + path + "/*\n")?}

{?printList(fileListExt(projectDir + "/" + path, src_dirs, ".cpp"),"SOURCES += ../../" + path + "/*\n", src_exclude )?}

{?printList(fileListExt(projectDir + "/" + path, src_dirs, ".h"),"HEADERS += ../../" + path + "/*\n", src_exclude )?}

OTHER_FILES += {?projectDir + "/" + path + "/config.json"?}
