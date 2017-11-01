"strict"

module.exports.type = "js_wrapper";

module.exports.generate = function (context, resConfig, resSrcDir, cacheSubDir) {
    const fs = require('fs');
    const path = require('path');
    const fse = context.require("fs-extra");
    const utils = context.require("./utils");

    function call(command, cwd) {
        const childProcess = require("child_process");
        childProcess.execSync(command, {"cwd": cwd, stdio: "inherit"});
    }
    const buildDir = path.join(resSrcDir, "../generators/v8WrapperGenerator/src/build");
    console.log("Build dir: " + buildDir);
    // Make automatic searching of DCMAKE_PREFIX_PATH
    // Search -extra-arg automatically too
    // Run flappy gen cmake and CMake before wrapper generation
    // Scan all modules for files with Component and Manager postfix
    fse.mkdirsSync(buildDir);
    const outputDir = path.join(context.projectRoot, "flappy_cache/V8JSEngine");
    fse.mkdirsSync(path.join(outputDir, "wrappers"));
    call("cmake -G \"Ninja\" -DCMAKE_PREFIX_PATH=\"/usr/local/Cellar/llvm/5.0.0/lib/cmake/llvm\" ..", buildDir);
    call("ninja", buildDir);
    call("./js_wrapper_generator "
    + ' -extra-arg "-I/usr/local/Cellar/llvm/5.0.0/include/c++/v1/"'
    + ' -extra-arg "-I/usr/local/Cellar/llvm/5.0.0/lib/clang/5.0.0/include"'
    + ' -p "' + context.targetOutDir + '"'
    + ' --output \"' + outputDir + '\"'
    + ' ' + path.join(context.projectRoot, '../../CoreComponents/src/TransformComponent.cpp')
    , buildDir);

    return [];
};

module.exports.getResList = function (resConfig, resSrcDir, cacheSubDir) {
    const path = require("path");

    var list = [

    ]
    return list;
}
