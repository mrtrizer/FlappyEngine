"strict"

function getModuleSourceList(context, moduleContext) {
    const path = require('path');
    const utils = context.requireFlappyScript("utils");
    const cxxConfig = moduleContext.config["cxx"];
    if (typeof(cxxConfig) !== "object")
        return [];
    const srcDirs = cxxConfig["src_dirs"];
    if (typeof(srcDirs) !== "object")
        return [];
    let sourceList = [];
    for (const i in srcDirs) {
        const srcDir = path.join(moduleContext.moduleRoot, srcDirs[i]);
        const files = utils.readDirs(srcDir);
        for (const i in files) {
            const filePath = files[i];
            const fileName = path.parse(filePath).base;
            if (fileName.indexOf(".cpp") != -1) {
                console.log(filePath);
                sourceList.push(filePath);
            }
        }
    }
    return sourceList;
}

function getSourceList(context) {
    const utils = context.requireFlappyScript("utils");
    const modules = context.requireFlappyScript("modules");
    const path = require('path');
    const fs = require('fs');
    const fse = context.require("fs-extra");

    let sourceList = [];

    const moduleContexts = modules.findAllModules(context);
    const contexts = [context].concat(moduleContexts);
    for (const i in contexts) {
        sourceList = sourceList.concat(getModuleSourceList(context, contexts[i]));
    }

    return sourceList;
}

function call(command, cwd) {
    const childProcess = require("child_process");
    childProcess.execSync(command, {"cwd": cwd, stdio: "inherit"});
}

function findLLVMDir(context) {
    const fs = require('fs');
    if (typeof context.config.llvmDir === "string")
        console.log("User defined llvmDir: " + context.config.llvmDir);
    const possibleDirs = [
        "/usr/local/Cellar/llvm/5.0.0/",
        "/usr/lib/llvm-5.0/",
        context.config.llvmDir
    ];
    for (const i in possibleDirs) {
        const dir = possibleDirs[i];
        if (fs.existsSync(dir))
            return dir;
    }
    throw new Error("Can't find llvm library. Plese define path to llvm in .flappy/flappy_conf/general.json \"llvmDir\":\"<path>\"");
}

function generateCompilationDB(context) {
    const path = require("path");
    const fse = context.require("fs-extra");
    const utils = context.requireFlappyScript("utils");
    const targetUtils = context.requireFlappyScript("target_utils");
    const genTarget = context.requireFlappyScript("gen_target");

    context.args.plainArgs = ["cmake"];
    genTarget.run(context);
    const cmakeGenerator = targetUtils.findGenerator(context, "cmake");
    const cmakeGeneratorDir = path.parse(cmakeGenerator.path).dir;

    const projectBuildContext = utils.createBuildContext(context, cmakeGeneratorDir, "project_conf");
    const buildDir = path.join(projectBuildContext.targetOutDir, "build");
    console.log(buildDir);
    fse.mkdirsSync(buildDir);
    call(`cmake -G "Unix Makefiles" ..`, buildDir);
    const compileCommandsSource = fse.readJsonSync(path.join(buildDir, "compile_commands.json"));
    for (const i in compileCommandsSource) {
        compileCommandsSource[i].directory = context.projectRoot;
    }
    const compileCommandsPath = path.join(context.projectRoot, "compile_commands.json");
    fse.outputJsonSync(compileCommandsPath, compileCommandsSource,  {"spaces" : 4});
    return compileCommandsPath;
}

module.exports.getHelp = function() {
    return "flappy gen_reflection - Generate reflection.";
}

function genDependencyMap(context, buildDir, sourceList, compilationDBPath) {
    const path = require("path");
    const fse = context.require("fs-extra");
    var dependencyMap = {};
    for (const i in sourceList) {
        const source = sourceList[i];
        console.log("source: " + source);
        compilationDatabase = fse.readJsonSync(compilationDBPath);
        const unitInfo = compilationDatabase.find(element => element["file"] == source);
        if (!unitInfo)
            continue;
        const childProcess = require("child_process");
        const command = unitInfo["command"].replace(/\-o.*?\.o/, "") + " -MM";
        const output = childProcess.execSync(command, {"cwd": buildDir, stdio: "pipe"});
        const rawDependencies = output.toString().replace(/.*?: \\/, "").split("\\");
        const dependencies = Array.from(rawDependencies, item => path.normalize(item.trim()));
        dependencyMap[source] = dependencies;
    }
    return dependencyMap;
}

function generateAllHeadersFile(allHeaders) {
    var allHeadersFileData = "#pragma once\n";
    for (const i in allHeaders) {
        const header = allHeaders[i];
        if (header.indexOf(".cpp") == -1)
            allHeadersFileData += `#include <${header}>\n`;
    }
    return allHeadersFileData;
}

function buildGenerator(context, buildDir, llvmDir) {
    const path = require("path");
    const fse = context.require("fs-extra");
    console.log("LLVM found: " + llvmDir);
    const cmakePath = path.join(llvmDir, "lib/cmake/llvm");
    fse.mkdirsSync(buildDir);
    call(`cmake -G \"Unix Makefiles\" -DCMAKE_PREFIX_PATH=\"${cmakePath}\" ..`, buildDir);
    call(`make`, buildDir);
}

function removeOutdateFiles(context, outputDir, rttrToSourcesPath, timestampCache) {
    const fse = context.require('fs-extra');
    const path = require("path");
    const fileList = [];
    const mapExists = fse.existsSync(rttrToSourcesPath);
    const rttrToHMap = mapExists ? fse.readJsonSync(rttrToSourcesPath) : {};
    console.log("rttrToHMap: " + JSON.stringify(rttrToHMap));
    fse.readdirSync(outputDir).forEach(file => {
        fileList.push(path.join(outputDir,file));
    })
    console.log("fileList: " + JSON.stringify(fileList));
    for (const i in fileList) {
        const rttrFilePath = fileList[i];
        if (typeof(rttrToHMap[rttrFilePath]) === "string") {
            const headerPath = rttrToHMap[rttrFilePath].trim();
            console.log("headrPath: " + headerPath);
            const index = allHeaders.indexOf(path.normalize(headerPath))
            console.log("Index:" + index);
            if (index === -1) {
                console.log("Header: " + path.normalize(headerPath));
                fs.unlinkSync(rttrFilePath);
                timestampCache.isChanged(headerPath);
                console.log("Removed: " + rttrFilePath);
            }
        }
    }
}

function filterSourceList(context, outputDir, dependencyMap, timestampCache, rttrToSourcesPath, sourceList) {
    const fse = context.require('fs-extra');
    const path = require("path");

    if (fse.existsSync(outputDir) && fse.existsSync(rttrToSourcesPath)) {
        var filteredSourceList = [];
        for (const i in sourceList) {
            const source = sourceList[i];
            const dependencies = dependencyMap[source];
            for (const i in dependencies) {
                const dependency = dependencies[i].trim();
                if (timestampCache.isChanged(dependency)) {
                    console.log("Cached " + source + " because of " + dependency);
                    if (filteredSourceList.indexOf(source) == -1)
                        filteredSourceList.push(source);
                }
            }
        }
        return filteredSourceList;

    } else {
        return sourceList;
    }
}

function writeGeneratedReflectionHpp(context, outputDir) {
    const fse = context.require('fs-extra');
    const path = require('path');

    const data = `#include <Reflection.hpp>\n
    \n
    flappy::Reflection extractReflectionDb();`;
    fse.mkdirsSync(outputDir);
    fse.writeFileSync(path.join(outputDir, "GeneratedReflection.hpp"), data);
}

module.exports.run = function (context) {
    const fs = require('fs');
    const path = require('path');
    const fse = context.require("fs-extra");
    const TimestampCache = context.requireFlappyScript("timestamp_cache").TimestampCache;
    const timestampCache = new TimestampCache(context);

    // Should be first because project includes this file
    const outputDir = path.join(context.cacheDir, "GeneratedReflection");
    writeGeneratedReflectionHpp(context, outputDir);

    // Build
    const buildDir = path.join(__dirname, "ClangASTGenerator/src/build");
    console.log("Build dir: " + buildDir);
    const llvmDir = findLLVMDir(context);
    buildGenerator(context, buildDir, llvmDir);

    // Generate compile_commands.json
    const compilationDBPath = generateCompilationDB(context);

    // Generate
    const sourceList = getSourceList(context);
    console.log("sourceList: " + JSON.stringify(sourceList));
    const dependencyMap = genDependencyMap(context, buildDir, sourceList, compilationDBPath);

    const rttrToSourcesPath = path.join(context.cacheDir, "rttr_to_sources.json");

    removeOutdateFiles(context, outputDir, rttrToSourcesPath, timestampCache);

    var allHeaders = [];
    for (const i in dependencyMap)
        allHeaders = allHeaders.concat(dependencyMap[i]);
    fse.mkdirsSync(path.join(outputDir, "../Tmp"));
    fse.writeFile(path.join(outputDir, "../Tmp/AllHeaders.hpp"), generateAllHeadersFile(allHeaders));

    const filteredSourceList = filterSourceList(context,
                                                outputDir,
                                                dependencyMap,
                                                timestampCache,
                                                rttrToSourcesPath,
                                                sourceList);

    console.log("Filtered sources: " + JSON.stringify(filteredSourceList));

    if (filteredSourceList.length > 0) {
        fse.mkdirsSync(outputDir);

        const clangIncludes1 = path.join(llvmDir, "include/c++/v1");
        const clangIncludes2 = path.join(llvmDir, "lib/clang/5.0.0/include");

        const generateCommand = `./reflection_generator `
                                + ` -extra-arg \"-I${clangIncludes1}\"`
                                + ` -extra-arg \"-I${clangIncludes2}\"`
                                + ` -p \"${context.projectRoot}\"`
                                + ` --output \"${outputDir}\"`
                                + ' ' + filteredSourceList.join(" ");
        console.log("Generation command: ", generateCommand);
        call(generateCommand, buildDir);
        // save source list to cache
    }
};

module.exports.before = ["gen"];
