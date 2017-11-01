"strict"

module.exports.type = "js_wrapper";

function isComponentCpp(fileName) {
    return (fileName.indexOf(".cpp") != -1)
            && ((fileName.indexOf("Component") != -1)
            || (fileName.indexOf("Manager") != -1));
}

function getSourceList(context) {
    const utils = context.require("./utils");
    const modules = context.require("./modules");
    const path = require('path');

    let sourceList = [];

    const moduleContexts = modules.findAllModules(context);
    for (const i in moduleContexts) {
        const moduleContext = moduleContexts[i];
        //console.log(JSON.stringify(moduleContext));
        const srcDir = path.join(moduleContext.projectRoot, "src");
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
    return sourceList.join(" ");
}

module.exports.generate = function (context, resConfig, resSrcDir, cacheSubDir) {
    const fs = require('fs');
    const path = require('path');
    const fse = context.require("fs-extra");

    const sourceList = getSourceList(context);

    function call(command, cwd) {
        const childProcess = require("child_process");
        childProcess.execSync(command, {"cwd": cwd, stdio: "inherit"});
    }
    const buildDir = path.join(resSrcDir, "../generators/v8WrapperGenerator/src/build");
    console.log("Build dir: " + buildDir);
    // Make automatic searching of DCMAKE_PREFIX_PATH
    // Search -extra-arg automatically too
    // Run flappy gen cmake and CMake before wrapper generation
    const llvmDir = "/usr/local/Cellar/llvm/5.0.0/"
    const cmakePath = path.join(llvmDir, "lib/cmake/llvm");
    const clangIncludes1 = path.join(llvmDir, "include/c++/v1");
    const clangIncludes2 = path.join(llvmDir, "lib/clang/5.0.0/include");
    fse.mkdirsSync(buildDir);
    const outputDir = path.join(context.projectRoot, "flappy_cache/V8JSEngine");
    fse.mkdirsSync(path.join(outputDir, "wrappers"));
    call(`cmake -G \"Ninja\" -DCMAKE_PREFIX_PATH=\"${cmakePath}\" ..`, buildDir);
    call(`ninja`, buildDir);
    call(`./js_wrapper_generator `
    + ` -extra-arg \"-I${clangIncludes1}\"`
    + ` -extra-arg \"-I${clangIncludes2}\"`
    + ` -p \"${context.targetOutDir}\"`
    + ` --output \"${outputDir}\"`
    + ' ' + sourceList
    , buildDir);

    return [];
};

module.exports.getResList = function (resConfig, resSrcDir, cacheSubDir) {
    const path = require("path");

    var list = [

    ]
    return list;
}
