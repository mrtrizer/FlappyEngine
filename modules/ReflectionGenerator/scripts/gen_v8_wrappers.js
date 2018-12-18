"strict"

function isComponentCpp(fileName) {
    return (fileName.indexOf(".cpp") != -1)
            && ((fileName.indexOf("Component") != -1)
            || (fileName.indexOf("Manager") != -1));
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
        const moduleContext = contexts[i];
        if (moduleContext.config.name == "RTTR" || moduleContext.config.name == "RTTRGenerator")
            continue;
        //console.log(JSON.stringify(moduleContext));
        const srcDir = path.join(moduleContext.moduleRoot, "src");
        const files = utils.readDirs(srcDir);
        for (const i in files) {
            const filePath = files[i];
            const fileName = path.parse(filePath).base;
            if (isComponentCpp(fileName)) {
                console.log(filePath);
                sourceList.push(filePath);
            }
        }
    }

    return sourceList;
}

function call(command, cwd) {
    const childProcess = require("child_process");
    childProcess.execSync(command, {"cwd": cwd, stdio: "inherit"});
}

function findLLVMDir() {
    const fs = require('fs');
    const possibleDirs = [
        "/usr/local/Cellar/llvm/5.0.0/",
        "/usr/lib/llvm-5.0/"
    ];
    for (const i in possibleDirs) {
        const dir = possibleDirs[i];
        if (fs.existsSync(dir))
            return dir;
    }
    throw new Error("Can't find llvm-5.0 library.");
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
        const compileCommand = compileCommandsSource[i];
        compileCommand.directory = context.projectRoot;
    }
    const compileCommandsPath = path.join(context.projectRoot, "compile_commands.json");
    fse.outputJsonSync(compileCommandsPath, compileCommandsSource,  {"spaces" : 4});
    //fse.removeSync(buildDir);
}

module.exports.getHelp = function() {
    return "flappy gen_reflection - Generate reflection.";
}

module.exports.run = function (context) {
    const fs = require('fs');
    const path = require('path');
    const fse = context.require("fs-extra");
    const TimestampCache = context.requireFlappyScript("timestamp_cache").TimestampCache;
    const timestampCache = new TimestampCache(context);
    // Generate compile_commands.json
    generateCompilationDB(context);
    // Build
    const buildDir = path.join(__dirname, "ClangASTGenerator/src/build");
    console.log("Build dir: " + buildDir);
    const llvmDir = findLLVMDir();
    console.log("LLVM found: " + llvmDir);
    const cmakePath = path.join(llvmDir, "lib/cmake/llvm");
    fse.mkdirsSync(buildDir);
    call(`cmake -G \"Unix Makefiles\" -DCMAKE_PREFIX_PATH=\"${cmakePath}\" ..`, buildDir);
    call(`make`, buildDir);
    // Generate
    var sourceList = getSourceList(context);
    var dependencyMap = {};
    var allHeaders = [];
    console.log("sourceList: " + JSON.stringify(sourceList));
    for (const i in sourceList) {
        const source = sourceList[i];
        console.log("source: " + source);
        const compileCommandsPath = path.join(context.projectRoot, "compile_commands.json");
        const compilationDatabase = fse.readJsonSync(compileCommandsPath);
        const unitInfo = compilationDatabase.find(element => element["file"] == source);
        if (!unitInfo)
            continue;
        const childProcess = require("child_process");
        const command = unitInfo["command"].replace(/\-o.*?\.o/, "") + " -MM";
        const output = childProcess.execSync(command, {"cwd": buildDir, stdio: "pipe"});
        const rawDependencies = output.toString().replace(/.*?: \\/, "").split("\\");
        const dependencies = Array.from(rawDependencies, item => path.normalize(item.trim()));
        dependencyMap[source] = dependencies;
        allHeaders = allHeaders.concat(dependencies);
    }
    console.log("allHeaders: " + JSON.stringify(allHeaders));
    const fileList = [];
    const outputDir = path.join(context.cacheDir, "RTTRWrappers");
    const rttrToSourcesPath = path.join(context.cacheDir, "rttr_to_sources.json");
    if (fs.existsSync(outputDir) && fs.existsSync(rttrToSourcesPath)) {
        fs.readdirSync(outputDir).forEach(file => {
            fileList.push(path.join(outputDir,file));
        })
        console.log("fileList: " + JSON.stringify(fileList));
        const rttrToHMap = fse.readJsonSync(rttrToSourcesPath);
        console.log("rttrToHMap: " + JSON.stringify(rttrToHMap));
        // Remove all not in list
        for (const i in fileList) {
            const rttrFilePath = fileList[i];
            const headerPath = rttrToHMap[rttrFilePath].trim();
            console.log("headrPIah: " + headerPath);
            if (typeof(headerPath) === "string") {
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

        console.log("Filtered sources: " + JSON.stringify(filteredSourceList));
        sourceList = filteredSourceList;

    }
    //
    // Filter sourceList via cache
    

    // const filteredSourceList = sourceList.filter(item => cache.indexOf(item) == -1 && isChanged(cache[item]));
    // console.log("Filtered list: " + JSON.stringify(filteredSourceList));
    if (sourceList.length > 0) {
        fse.mkdirsSync(outputDir);
        const clangIncludes1 = path.join(llvmDir, "include/c++/v1");
        const clangIncludes2 = path.join(llvmDir, "lib/clang/7.0.0_1/include");

        const generateCommand = `./rttr_generator `
                                + ` -extra-arg \"-I${clangIncludes1}\"`
                                + ` -extra-arg \"-I${clangIncludes2}\"`
                                + ` -p \"${context.projectRoot}\"`
                                + ` --output \"${outputDir}\"`
                                + ' ' + sourceList.join(" ");
        console.log("Generation command: ", generateCommand);
        call(generateCommand, buildDir);
        // save source list to cache
    }
};

module.exports.before = ["gen"];
