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
    fse.removeSync(buildDir);
}

module.exports.getHelp = function() {
    return "flappy gen_v8_wrappers - Generate wrappers for v8 js engine.";
}

module.exports.run = function (context) {
    const fs = require('fs');
    const path = require('path');
    const fse = context.require("fs-extra");
    // Generate compile_commands.json
    generateCompilationDB(context);
    // Build
    const buildDir = path.join(__dirname, "v8WrapperGenerator/src/build");
    console.log("Build dir: " + buildDir);
    const llvmDir = findLLVMDir();
    console.log("LLVM found: " + llvmDir);
    const cmakePath = path.join(llvmDir, "lib/cmake/llvm");
    fse.mkdirsSync(buildDir);
    call(`cmake -G \"Unix Makefiles\" -DCMAKE_PREFIX_PATH=\"${cmakePath}\" ..`, buildDir);
    call(`make`, buildDir);
    // Generate
    const sourceList = getSourceList(context);
    if (sourceList.length > 0) {
        const outputDir = path.join(context.cacheDir, "V8JSWrappers");
        fse.mkdirsSync(path.join(outputDir, "wrappers"));
        const clangIncludes1 = path.join(llvmDir, "include/c++/v1");
        const clangIncludes2 = path.join(llvmDir, "lib/clang/5.0.0/include");

        const generateCommand = `./rttr_generator `
                                + ` -extra-arg \"-I${clangIncludes1}\"`
                                + ` -extra-arg \"-I${clangIncludes2}\"`
                                + ` -p \"${context.projectRoot}\"`
                                + ` --output \"${outputDir}\"`
                                + ' ' + sourceList;
        console.log("Generation command: ", generateCommand);
        call(generateCommand, buildDir);
    }
};

module.exports.before = ["gen"];
