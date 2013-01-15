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
    
    if(uri.pathname == "/api/sumthing") {

    } else if (uri.pathname == "/api/somethingElse") {

    } else {
        res.writeHead(200, {"Content-Type": "application/json"});
        res.end('{' +
                '"error":404,' +
                '"description":"No Action for Url '+uri.pathname+' with query '+
                uri.query +
                '"}');
    }
    
}


// exported instance
var api = new Api();
api.Api = Api;

module.exports = api;
