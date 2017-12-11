"strict"

module.exports.type = "glsl_shader";

function copyFileToCache(context, file, resSrcDir, cacheSubDir) {
    const path = require("path");
    const fse = context.require("fs-extra");
    const logger = context.requireFlappyScript("logger");

    const inputPath = path.join(resSrcDir, file)
    const outputPath = path.join(cacheSubDir, file)
    logger.logi("Copy from: " + inputPath);
    logger.logi("Copy to: " + outputPath);
    fse.copySync(inputPath, outputPath);
}

module.exports.generate = function (context, scriptContext, resConfig, resSrcDir, cacheSubDir) {
    const path = require("path");
    copyFileToCache(context, resConfig.fragment, resSrcDir, cacheSubDir);
    copyFileToCache(context, resConfig.vertex, resSrcDir, cacheSubDir);
    return [
        resConfig,
        {
            "path": resConfig.vertex,
            "name": path.parse(resConfig.vertex).name + "_vertex",
            "fullPath": path.join(cacheSubDir, resConfig.vertex),
            "type": "file"
        },
        {
            "path": resConfig.fragment,
            "name": path.parse(resConfig.fragment).name + "_fragment",
            "fullPath": path.join(cacheSubDir, resConfig.fragment),
            "type": "file"
        }
    ]
};
