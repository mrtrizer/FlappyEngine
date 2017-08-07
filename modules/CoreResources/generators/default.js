"strict"

function copyFile(source, target, cb) {
    const fs = require("fs")
    var cbCalled = false;

    var rd = fs.createReadStream(source);
    rd.on("error", function(err) {
        done(err);
    });
    var wr = fs.createWriteStream(target);
    wr.on("error", function(err) {
        done(err);
    });
    wr.on("close", function(ex) {
        done();
    });
    rd.pipe(wr);

    function done(err) {
        if (cb && !cbCalled) {
            cb(err);
            cbCalled = true;
        }
    }
}

module.exports.type = "*";
module.exports.generate = function (config, resSrcDir, cacheDir) {
    const path = require("path");
    const fs = require('fs');
    const inputPath = path.join(resSrcDir, config.input)
    const outputPath = path.join(cacheDir, config.input)
    console.log(inputPath);
    console.log(outputPath);
    copyFile(inputPath, outputPath);
};
