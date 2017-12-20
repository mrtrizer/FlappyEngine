"strict"

module.exports.type = "ttf";

module.exports.generate = function (context, scriptContext, resConfig, resSrcDir, cacheSubDir) {
    const fs = require('fs');
    const path = require('path');
    const fse = context.require("fs-extra");
    const utils = context.requireFlappyScript("utils");
    const deasync = require("deasync");
    const logger = context.requireFlappyScript("logger");

    const atlasName = path.parse(resConfig["font"]).name;
    const jsonFileName = atlasName + ".json";
    const imageFileName = atlasName + ".png";

    logger.logi("Font generator start");
    logger.logi("JSON file name: " + jsonFileName);
    logger.logi("Image file name: " + imageFileName);

    var done = false;

    const generateBMFont = require('msdf-bmfont-xml');

    const fontPath = path.join(resSrcDir, resConfig.font);

    const opt = {
        fieldType: "sdf",
        reuse: false,
        outputType: "json",
        charset: " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯабвгдеёжзийклмнопрстуфхцчшщъыьэюя"
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
            "input": imageFileName,
            "path": imageFileName,
            "fullPath": path.join(cacheSubDir, imageFileName),
            "type": "file"
        },
        {
            "name": atlasName + "_meta",
            "input": jsonFileName,
            "path": jsonFileName,
            "fullPath": path.join(cacheSubDir, jsonFileName),
            "type": "file"
        }
    ];
};
