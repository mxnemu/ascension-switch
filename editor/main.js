var http = require("http"),
    url = require("url"),
    fileSever = require("./src/FileServer.js"),
    api = require("./src/Api.js");

var port = 8081;

http.createServer(function(req, res) {
    var uri = url.parse(req.url);
    if (api.isApiUrl(uri.pathname)) {
        api.handleUri(res, uri);
    } else if(uri.pathname == "/") {
        fileSever.sendFile(res, "index.html");
    } else {
        fileSever.sendFile(res, uri.pathname);
    }
}).listen(port, "localhost");
console.log("started server on port " + port + " for local access");
