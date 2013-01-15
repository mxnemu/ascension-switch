function Selection(x,y) {
    this.x = x || 0;
    this.y = y || 0;
    this.w = 0;
    this.h = 0;
}

Selection.prototype.draw = function(context) {
    context.fillStyle = "rgba(200, 170, 20, 0.8)";
    context.fillRect(this.x, this.y, this.w, this.h);
}

Selection.prototype.snapToAlpha = function(context) {
    var imgd = context.getImageData(this.x, this.y, this.w, this.h);
    var pix = imgd.data;

    var offsets = [0,0,0,0];

    // x offset    
    var pos = 0;
    for (var x = 0; x < this.w; ++x) {
        var columnHasAlpha = false;

        for (var y = 0; y < this.h; ++y) {
            pos = ((y*this.w) +x) * 4;
            if (pix[pos+3] > 0) {
                columnHasAlpha = true;
            }
        }
        
        if (!columnHasAlpha) {
            offsets[0] += 1;
            console.log("shrink x");
        } else {
            break;
        }
    }
    
    // y offset    
    pos = 0;
    for (var y = 0; y < this.h; ++y) {
        var rowHasAlpha = false;
        
        for (var x = 0; x < this.w; ++x) {
            
            if (pix[pos+3] > 0) {
                rowHasAlpha = true;
            } 
            pos += 4;
        }
        
        if (!rowHasAlpha) {
            offsets[1] += 1;
            console.log("shrink y");
        } else {
            break;
        }
    }
    
    // w offset    
    pos = 0;
    for (var x = this.w-1; x >= 0; --x) {
        var columnHasAlpha = false;

        for (var y = this.h-1; y >= 0; --y) {
            pos = ((y*this.w) +x) * 4;
            if (pix[pos+3] > 0) {
                columnHasAlpha = true;
            }
        }
        
        if (!columnHasAlpha) {
            offsets[2] -= 1;
            console.log("shrink w");
        } else {
            break;
        }
    }
    
    // h offset
    pos = pix.length -4;
    for (var y = this.h-1; y >= 0; --y) {
        var rowHasAlpha = false;
        
        for (var x = this.w-1; x >= 0; --x) {
            
            if (pix[pos+3] > 0) {
                rowHasAlpha = true;
                
            } 
            pos -= 4;
        }
        
        if (!rowHasAlpha) {
            offsets[3] -= 1;
            console.log("shrink h");
        } else {
            break;
        }
    }
    console.log(offsets);
    this.x += offsets[0];
    this.y += offsets[1];
    this.w += offsets[2];
    this.h += offsets[3];
}


function FrameSelector() {
    var _this = this;
    this.canvas = $(".spriteSheet").get(0);
    this.context = this.canvas.getContext("2d");
    
    

    this.currentSelection = null;
    this.selections = [];

    $(".spriteSheet").mousedown(function(event) {
        _this.currentSelection = new Selection(event.offsetX, event.offsetY);
    });
    
    $(".spriteSheet").mousemove(function(event) {
        if (_this.currentSelection) {
            _this.currentSelection.w = event.offsetX - _this.currentSelection.x;
            _this.currentSelection.h = event.offsetY - _this.currentSelection.y;
        }
    });
    
    $(".spriteSheet").click(function(event) {
        _this.currentSelection.w = event.offsetX - _this.currentSelection.x;
        _this.currentSelection.h = event.offsetY - _this.currentSelection.y;
        _this.currentSelection.snapToAlpha(_this.context);
        _this.selections.push(_this.currentSelection);
        _this.currentSelection = null;
    });
}

FrameSelector.prototype.loadSpriteSheet = function(path) {
    var _this = this;
    this.image = new Image();
    this.image.src = path;
}

FrameSelector.prototype.draw = function() {
    this.context.clearRect(0,0,this.canvas.width, this.canvas.height);
    this.context.drawImage(this.image,0,0);
    for (var i=0; i < this.selections.length; ++i) {
        this.selections[i].draw(this.context);
    }
}

$(function() {
    console.log("init");
    window.G_frameSelector = new FrameSelector();
    window.G_frameSelector.loadSpriteSheet("images/person.png")
    
    window.requestAnimFrame = (function(){
      return  window.requestAnimationFrame       || 
              window.webkitRequestAnimationFrame || 
              window.mozRequestAnimationFrame    || 
              window.oRequestAnimationFrame      || 
              window.msRequestAnimationFrame     || 
              function( callback ){
                window.setTimeout(callback, 1000 / 60);
              };
    })();
    
    var drawFunction;
    drawFunction = function() {
        window.G_frameSelector.draw();
        window.requestAnimFrame(drawFunction);
    };
    
    window.requestAnimFrame(drawFunction);
})
