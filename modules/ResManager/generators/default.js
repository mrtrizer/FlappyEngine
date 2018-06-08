"strict"

module.exports.type = "*";

module.exports.generate = function (context, scriptContext, resConfig, resSrcDir, cacheSubDir) {
    const path = require("path");
    const fse = context.require("fs-extra");
    const logger = context.requireFlappyScript("logger");

    logger.logi("Default generator start");
    logger.logi("Res src dir: " + resSrcDir);
    logger.logi("Cache subdir: " + cacheSubDir);

    logger.logi("Res config: " + JSON.stringify(resConfig));
    const inputPath = path.join(resSrcDir, resConfig.input)
    const outputPath = path.join(cacheSubDir, resConfig.input)
    logger.logi("Res type: " + resConfig.type);
    logger.logi("Copy from: " + inputPath);
    logger.logi("Copy to: " + outputPath);
    fse.copySync(inputPath, outputPath);

    const metaData = Object.assign({}, resConfig, {
        "path": resConfig.input,
        "fullPath": outputPath,
        "type": "file"
    });

    return [metaData];
};
