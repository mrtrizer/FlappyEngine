"strict"

module.exports.type = "*";

module.exports.generate = function (context, scriptContext, resConfig, resSrcDir, cacheSubDir) {
    const path = require("path");
    const fse = context.require("fs-extra");
    const logger = context.require("./logger.js");

    const inputPath = path.join(resSrcDir, resConfig.input)
    const outputPath = path.join(cacheSubDir, resConfig.input)
    logger.logi("Default generator start");
    logger.logi("Res type: " + resConfig.type);
    logger.logi("Copy from: " + inputPath);
    logger.logi("Copy to: " + outputPath);
    fse.copySync(inputPath, outputPath);

    return [resConfig];
};

module.exports.getResList = function (resConfig, resSrcDir, cacheSubDir) {
    const path = require("path");
    const relativePath = path.join(resConfig.input);
    const outputPath = path.join(cacheSubDir, resConfig.input);

    var list = [
        {
            "path": relativePath,
            "fullPath": outputPath,
            "type": resConfig.type
        }
    ];

    return list;
}
