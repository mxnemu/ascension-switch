function FrameSelector(comboList) {
    var _this = this;
    this.comboList = comboList;
    
    this.canvas = $(".spriteSheet").get(0);
    this.context = this.canvas.getContext("2d");
    this.selectionCanvas = $(".spriteSheetSelection").get(0)
    this.selectionContext = this.selectionCanvas.getContext("2d");

    this.currentSelection = null;
    this.selections = [];

    $(".spriteSheetSelection").mousedown(function(event) {
        // close open selection
        if (_this.currentSelection) {
            _this.currentSelection.w = event.offsetX - _this.currentSelection.x;
            _this.currentSelection.h = event.offsetY - _this.currentSelection.y;
            if (app.options.snapToAlpha) {
                _this.currentSelection.snapToAlpha(_this.context);
            }
            if (_this.currentSelection.validate()) {        
                _this.comboList.addFrame(_this.currentSelection);
            } else {
                _this.removeSelection(_this.currentSelection);
            }
            _this.currentSelection = null; 
        } else {
            // create new selection
            _this.currentSelection = new Selection(event.offsetX, event.offsetY);
            _this.selections.push(_this.currentSelection);
        }
    });
    
    $(".spriteSheetSelection").mousemove(function(event) {
        if (_this.currentSelection) {
            _this.currentSelection.w = event.offsetX - _this.currentSelection.x;
            _this.currentSelection.h = event.offsetY - _this.currentSelection.y;
        }
    });
}

FrameSelector.prototype.removeSelection = function(selection) {
    var _this = this;
    $.each(this.selections, function(key) {
        if (this == selection) {
            _this.selections.splice(key, 1);
        }
    });
}

FrameSelector.prototype.loadSpriteSheet = function(path) {
    var _this = this;
    this.image = new Image();
    this.image.onload = function()  {
        _this.canvas.width = _this.image.width;
        _this.canvas.height = _this.image.height;
        _this.selectionCanvas.width = _this.image.width;
        _this.selectionCanvas.height = _this.image.height;
        _this.context.drawImage(_this.image,0,0);
    }
    this.image.src = path;
    
}

FrameSelector.prototype.draw = function() {
    this.selectionContext.clearRect(0,0,this.selectionCanvas.width, this.selectionCanvas.height);
    for (var i=0; i < this.selections.length; ++i) {
        this.selections[i].draw(this.selectionContext);
    }
}

$(function() {
})
