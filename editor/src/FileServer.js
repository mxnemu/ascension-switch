var path = require("path"),
    fs = require("fs"),
    mime = require("../node_modules/mime/mime.js");

function FileServer() {

}
/// change a relative path into an absolute one
/// make sure the url is in the pub/ directory, otherwise return "pub/403.html"
FileServer.prototype.resolveUrl = function(relativePath) {
    var baseDir = path.join(process.cwd(), "pub");
    url = path.join(baseDir, relativePath);
    url = url.replace(/(\%20)/g, " ");
    url = path.normalize(url);
    
    // only return paths in pub
    if (url.substring(0, baseDir.length) === baseDir) {
        return url;
    } else {
        return path.join(process.cwd(), "pub/403.html");
    }
}

// if url has an .extension dot check for \\.html$, otherwise just asume html
FileServer.prototype.isHtmlUrl = function(url) {
    if (new RegExp("\\.").test(url)) {
        return new RegExp("\\.html$", "i").test(url);
    }
    return true;
}

FileServer.prototype.isPlayableFile = function(filename) {
    return new RegExp("(\.ogg)||(\.flac)||(\.opus)||(\.wav)||(\.aac)||(\.mp3)",
                      "i").test(filename);
}

/// Sends the file or an error message if the file is not accessable 
FileServer.prototype.sendFile = function(res, url) {
    var _this = this;
    url = this.resolveUrl(url);
    
    fs.exists(url, function(exists) {
        if(!exists) {
            _this.sendErrorCode(res, 404, !_this.isHtmlUrl(url));
            return;
        }
        
        fs.readFile(url, "binary", function(error, file) {
            if(error) {
                _this.sendErrorCode(res, 500, !_this.isHtmlUrl(url));
                return;
            }

            res.writeHead(200, {"Content-Type": mime.lookup(url)});
            res.write(file, "binary");
            res.end();
        });
    });
}

/// Sends "pub/"+code+".html" if it exists, or a simple text message
FileServer.prototype.sendErrorCode = function(res, code, sendTextResponse) {
    if (sendTextResponse) {
        res.writeHead(code, {"Content-Type": "text/plain"})
        res.end("Error: " + code);
        return;
    }

    var url = this.resolveUrl(code + ".html");
    fs.exists(url, function(exists) {
        if(!exists) {
            res.writeHead(code, {"Content-Type": "text/plain"})
            res.end("Error: " + code);
            return;
        }
        
        fs.readFile(url, "binary", function(error, file) {
            if(error) {
                res.writeHead(code, {"Content-Type": "text/plain"})
                res.end("Error: " + code);
                return;
            }

            res.writeHead(code, {"Content-Type": mime.lookup(url)});
            res.write(file, "binary");
            res.end();
        });
    });
}

// exported instance
var fileServer = new FileServer();
fileServer.FileServer = FileServer;

module.exports = fileServer;
