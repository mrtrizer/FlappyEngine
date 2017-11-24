"strict"

module.exports.type = "js_wrapper";

function isComponentCpp(fileName) {
    return (fileName.indexOf(".cpp") != -1)
            && ((fileName.indexOf("Component") != -1)
            || (fileName.indexOf("Manager") != -1));
}

function getSourceList(context, cacheSubDir) {
    const utils = context.require("./utils");
    const modules = context.require("./modules");
    const path = require('path');
    const fs = require('fs');
    const fse = context.require("fs-extra");

    let timestampCache = new utils.TimestampCache(context);

    let sourceList = [];

    const moduleContexts = modules.findAllModules(context);
    for (const i in moduleContexts) {
        const moduleContext = moduleContexts[i];
        //console.log(JSON.stringify(moduleContext));
        const srcDir = path.join(moduleContext.moduleRoot, "src");
        const files = utils.readDirs(srcDir);
        for (const i in files) {
            const filePath = files[i];
            if (timestampCache.isChanged(filePath)) {
                const fileName = path.parse(filePath).base;
                if (isComponentCpp(fileName)) {
                    console.log(filePath);
                    sourceList.push(filePath);
                }
            }
        }
    }

    return sourceList.join(" ");
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
    const utils = context.require("./utils");
    const generateProject = context.require("./generate_project");
    var params = utils.findParams(context.projectRoot, "cmake", null, [], []);
    generateProject.generateProject(params);
    const buildDir = path.join(context.targetOutDir, "build");
    fse.mkdirsSync(buildDir);
    call(`cmake -G "Unix Makefiles" ..`, buildDir);
    const compileCommandsSource = fse.readJsonSync(path.join(buildDir, "compile_commands.json"));
    for (const i in compileCommandsSource) {
        const compileCommand = compileCommandsSource[i];
        compileCommand.directory = context.projectRoot;
    }
    const compileCommandsPath = path.join(context.projectRoot, "compile_commands.json");
    fse.outputJsonSync(compileCommandsPath, compileCommandsSource,  {"spaces" : 4});
    fse.removeSync(buildDir);
}

module.exports.generate = function (context, scriptContext, resConfig, resSrcDir, cacheSubDir) {
    const fs = require('fs');
    const path = require('path');
    const fse = context.require("fs-extra");
    // Generate compile_commands.json
    generateCompilationDB(context);
    // Build
    const buildDir = path.join(scriptContext.moduleRoot, "generators/v8WrapperGenerator/src/build");
    console.log("Build dir: " + buildDir);
    // TODO: Make automatic searching of llvmDir
    // TODO: Run flappy gen cmake and CMake before wrapper generation
    const llvmDir = findLLVMDir();
    console.log("LLVM found: " + llvmDir);
    const cmakePath = path.join(llvmDir, "lib/cmake/llvm");
    fse.mkdirsSync(buildDir);
    call(`cmake -G \"Ninja\" -DCMAKE_PREFIX_PATH=\"${cmakePath}\" ..`, buildDir);
    call(`ninja`, buildDir);
    // Generate
    const sourceList = getSourceList(context, cacheSubDir);
    if (sourceList.length > 0) {
        const outputDir = path.join(cacheSubDir, "V8SJWrappers");
        fse.mkdirsSync(path.join(outputDir, "wrappers"));
        const clangIncludes1 = path.join(llvmDir, "include/c++/v1");
        const clangIncludes2 = path.join(llvmDir, "lib/clang/5.0.0/include");

        const generateCommand = `./js_wrapper_generator `
                                + ` -extra-arg \"-I${clangIncludes1}\"`
                                + ` -extra-arg \"-I${clangIncludes2}\"`
                                + ` -p \"${context.targetOutDir}\"`
                                + ` --output \"${outputDir}\"`
                                + ' ' + sourceList;
        console.log("Generation command: ", generateCommand);
        call(generateCommand, buildDir);
    }

    return [];
};

module.exports.getResList = function (resConfig, resSrcDir, cacheSubDir) {
    const path = require("path");

    var list = [

    ]
    return list;
}
