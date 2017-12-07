#!/usr/bin/env node
"use strict"

module.exports.getHelp = function getHelp() {
    return "flappy pack_res <template> - Pack resources for target platform. (Should be generated first)";
}

function packResources(context, templateName) {
    const utils = context.require("./utils.js");
    const logger = context.require("./logger.js");

    const generatorPath = utils.findTemplate(context.searchDirs, templateName);
    const buildContext = utils.createBuildContext(context, generatorPath, "project_conf");
    var projectGenerator = utils.requireGeneratorScript(generatorPath);
    try {
        projectGenerator.packResources(buildContext);
    } catch (e) {
        logger.loge("Resouce packer filed: " + e.message);
    }
}

module.exports.run = function(context, args) {
    if (args.length < 1)
        throw new Error("At least template name expected");
    const templateName = args[0];

    packResources(context, templateName);
}

module.exports.after = ["gen_res"];
