function Selection(x,y) {
    this.x = x || 0;
    this.y = y || 0;
    this.w = 0;
    this.h = 0;
}

Selection.prototype.serialize = function() {
    return {
        x: this.x,
        y: this.y,
        w: this.w,
        h: this.h
    };
}

Selection.prototype.resetore = function(serialized) {
    this.x = serialized.x;
    this.y = serialized.y;
    this.w = serialized.w;
    this.h = serialized.h;
}

Selection.prototype.draw = function(context) {
    context.fillStyle = "rgba(200, 170, 20, 0.8)";
    context.fillRect(this.x, this.y, this.w, this.h);
}

Selection.prototype.validate = function(context) {
    if (this.w < 0) {
        this.x += this.w;
        this.w = -this.w;
    }
    
    if (this.h < 0) {
        this.y += this.h;
        this.h = -this.h;
    }
    
    return this.w > 0 && this.h > 0;
}

Selection.prototype.snapToAlpha = function(context) {
    if (!this.validate()) {
        return;
    }

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
            //console.log("shrink x");
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
            //console.log("shrink y");
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
            offsets[2] += 1;
            //console.log("shrink w");
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
            offsets[3] += 1;
            //console.log("shrink h");
        } else {
            break;
        }
    }
    //console.log(offsets);
    this.x += offsets[0];
    this.y += offsets[1];
    this.w -= offsets[2] + offsets[0];
    this.h -= offsets[3] + offsets[1];
}
