"strict"

module.exports.type = "glsl_shader";

function copyFileToCache(context, file, resSrcDir, cacheSubDir) {
    const path = require("path");
    const fse = context.require("fs-extra");
    const logger = context.require("./logger.js");

    const inputPath = path.join(resSrcDir, file)
    const outputPath = path.join(cacheSubDir, file)
    logger.logi("Copy from: " + inputPath);
    logger.logi("Copy to: " + outputPath);
    fse.copySync(inputPath, outputPath);
}

module.exports.generate = function (context, resConfig, resSrcDir, cacheSubDir) {
    copyFileToCache(context, resConfig.fragment, resSrcDir, cacheSubDir);
    copyFileToCache(context, resConfig.vertex, resSrcDir, cacheSubDir);
    return resConfig;
};

module.exports.getResList = function (resConfig, resSrcDir, cacheSubDir) {
    const path = require("path");

    var list = [
        {
            "path": resConfig.vertex,
            "fullPath": path.join(cacheSubDir, resConfig.vertex),
            "type": "glsl_shader/vertex"
        },
        {
            "path": resConfig.fragment,
            "fullPath": path.join(cacheSubDir, resConfig.fragment),
            "type": "glsl_shader/fragment"
        }
    ]
    return list;
}
