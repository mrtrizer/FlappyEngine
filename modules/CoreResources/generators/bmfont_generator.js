"strict"

module.exports.type = "ttf";

module.exports.generate = function (context, resConfig, resSrcDir, cacheSubDir) {
    const fs = require('fs');
    const path = require('path');
    const fse = context.require("fs-extra");
    const utils = context.require("./utils");
    const deasync = require("deasync");

    const atlasName = path.parse(resConfig["font"]).name;
    const jsonFileName = atlasName + ".json";

    var done = false;

    const generateBMFont = require('msdf-bmfont-xml');

    const fontPath = path.join(resSrcDir, resConfig.font);

    const opt = {
        fieldType: "sdf",
        reuse: false,
        outputType: "json"
    };

    generateBMFont(fontPath, opt, (error, textures, font) => {
        if (error)
            throw error;
        textures.forEach((sheet, index) => {
            const texturePath = path.join(cacheSubDir, `${atlasName}.png`);
            fs.writeFileSync(texturePath, sheet.texture);
        });
        fs.writeFileSync(path.join(cacheSubDir, jsonFileName), font.data);
        done = true;
    });

    deasync.loopWhile(function(){return !done;});

    return [
        {
            "name": resConfig["name"],
            "type": module.exports.type,
            "font": resConfig["font"],
            "image":  atlasName + "_image",
            "meta": atlasName + "_meta"
        },
        {
            "name": atlasName + "_image",
            "type": "png",
            "input": atlasName + ".png"
        },
        {
            "name": atlasName + "_meta",
            "type": "json",
            "input": jsonFileName
        }
    ];
};

module.exports.getResList = function (resConfig, resSrcDir, cacheSubDir) {
    const path = require("path");

    const atlasFileName = path.parse(resConfig["font"]).name;
    const imageFileName = atlasFileName + ".png";
    const jsonFileName = atlasFileName + ".json";

    var list = [
        {
            "path": imageFileName,
            "fullPath": path.join(cacheSubDir, imageFileName),
            "type": "file/drawable/png"
        },
        {
            "path": jsonFileName,
            "fullPath": path.join(cacheSubDir, jsonFileName),
            "type": "file/json"
        }
    ]
    return list;
}
