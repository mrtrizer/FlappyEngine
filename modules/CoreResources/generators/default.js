"strict"

module.exports.type = "*";

module.exports.generate = function (context, resConfig, resSrcDir, cacheSubDir) {
    const path = require("path");
    const fse = context.require("fs-extra");
    const inputPath = path.join(resSrcDir, resConfig.input)
    const outputPath = path.join(cacheSubDir, resConfig.input)
    console.log(inputPath);
    console.log(outputPath);
    fse.copySync(inputPath, outputPath);
};

module.exports.getResList = function (resConfig, resSrcDir, cacheSubDir) {
    const path = require("path");
    const outputPath = path.join(cacheSubDir, resConfig.input);
    var list = [
        {
            "path": resConfig.input,
            "fullPath": outputPath,
            "type": resConfig.type
        }
    ]
    return list;
}
