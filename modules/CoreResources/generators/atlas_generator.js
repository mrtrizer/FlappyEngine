"strict"

module.exports.type = "sprite_folder";

function generateJson(coordinates, properties) {
    const path = require('path');
    let sprites = {};
    for (const positionKey in coordinates) {
        const position = coordinates[positionKey];
        const rectName = path.parse(positionKey).name;
        sprites[rectName] = {
            "rect_x": position.x / properties.width,
            "rect_y": position.y / properties.height,
            "rect_w": position.width / properties.width,
            "rect_h": position.height / properties.height,
            "width": position.width,
            "height": position.height,
        };
    }
    return {"sprites":sprites};
}

module.exports.generate = function (context, resConfig, resSrcDir, cacheSubDir) {
    const spritesmith = require('spritesmith');
    const fs = require('fs');
    const path = require('path');
    const fse = context.require("fs-extra");
    const utils = context.require("./utils");
    const deasync = require("deasync");

    const atlasFileName = resConfig["atlas_name"];
    const imageFileName = atlasFileName + ".png";
    const jsonFileName = atlasFileName + ".json";

    const sprites = utils.readDirs(path.join(resSrcDir, resConfig["folder"]));

    var done = false;

    spritesmith.run({src: sprites}, (err, result) => {
        fs.writeFileSync(path.join(cacheSubDir, atlasFileName +  ".png"), result.image);
        const metaDataJson = generateJson(result.coordinates, result.properties);
        fse.writeJsonSync(path.join(cacheSubDir, atlasFileName +  ".json"), metaDataJson, {
            spaces: "    "
        });
        done = true;
    });

    deasync.loopWhile(function(){return !done;});

    return [
        {
            "name": resConfig["name"],
            "type": module.exports.type,
            "image": atlasFileName +  "_image",
            "meta": atlasFileName +  "_meta"
        },
        {
            "name": atlasFileName + "_image",
            "type": "png",
            "input": imageFileName
        },
        {
            "name": atlasFileName + "_meta",
            "type": "json",
            "input": jsonFileName
        }
    ];
};

module.exports.getResList = function (resConfig, resSrcDir, cacheSubDir) {
    const path = require("path");

    const atlasFileName = resConfig["atlas_name"];
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
