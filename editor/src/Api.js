var fs = require("fs"),
    path = require("path"),
    fileServer = require("./FileServer.js"),
    querystring = require("querystring");

function Api() {

}

Api.prototype.isApiUrl = function(url) {
    return new RegExp("\\/api\\/").test(url);
}

// ugly
Api.prototype.handleUri = function(res, uri) {
    var query = querystring.parse(uri.query || "");
    console.log(query);
    console.log(uri.pathname);
    if(uri.pathname == "/api/saveEntity" && query["name"] && query["data"]) {
        this.saveEntity(res, query["name"], query["data"]);
    } else {
        res.writeHead(200, {"Content-Type": "application/json"});
        res.end('{' +
                '"error":404,' +
                '"description":"No Action for Url '+uri.pathname+' with query '+
                uri.query +
                '"}');
    }
}

Api.prototype.saveEntity = function(res, name, data) {
    // TODO unsafe bullshit
    fs.writeFile("../scripts/" + name, data, function(error) {
        if (error) {
            res.writeHead(500, {"Content-Type": "application/json"});
            res.end('{"error": "could not write ' + name + '"}');
        }
        res.writeHead(200, {"Content-Type": "application/json"});
        res.end('{"status": "writing finished fine"}');
    });
}

// exported instance
var api = new Api();
api.Api = Api;

module.exports = api;
