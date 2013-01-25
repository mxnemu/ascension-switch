function FrameSelector(sidebar) {
    var _this = this;
    this.sidebar = sidebar;
    this.entity = null;
    
    this.canvas = $(".spriteSheet").get(0);
    this.context = this.canvas.getContext("2d");
    this.selectionCanvas = $(".spriteSheetSelection").get(0)
    this.selectionContext = this.selectionCanvas.getContext("2d");

    this.currentSelection = null;

    $(".spriteSheetSelection").mousedown(function(event) {
        // close open selection
        if (_this.currentSelection) {
            _this.currentSelection.w = event.offsetX - _this.currentSelection.x;
            _this.currentSelection.h = event.offsetY - _this.currentSelection.y;
            if (app.options.snapToAlpha) {
                _this.currentSelection.snapToAlpha(_this.context);
            }
            if (_this.currentSelection.validate()) {        
                _this.sidebar.addFrame(_this.currentSelection);
            } else {
                // simple click
                var frame = _this.entity.getFrameForPosition(event.offsetX, event.offsetY);
                if (frame) {
                    _this.sidebar.selectFrame(frame)
                }
            }
            _this.currentSelection = null; 
        } else {
            // create new selection
            _this.currentSelection = new Selection(event.offsetX, event.offsetY);
        }
    });
    
    $(".spriteSheetSelection").mousemove(function(event) {
        if (_this.currentSelection) {
            _this.currentSelection.w = event.offsetX - _this.currentSelection.x;
            _this.currentSelection.h = event.offsetY - _this.currentSelection.y;
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
    
    if (this.entity) {
        this.entity.save();
    }
    this.entity = new Entity();
    this.sidebar.entity = this.entity;
    this.sidebar.content.entity = this.entity;
}

FrameSelector.prototype.draw = function() {
    var _this = this;
    this.selectionContext.clearRect(0,0,this.selectionCanvas.width, this.selectionCanvas.height);
    
    for (var i=0; i < this.entity.frames.length; ++i) {
        if (this.sidebar.content.selectedFrame != this.entity.frames[i]) {
            this.entity.frames[i].draw(this.selectionContext);
        } else {
            this.entity.frames[i].draw(this.selectionContext, "rgba(10,40,240,0.9)");
        }
    }
    
    if (this.currentSelection) {
        this.currentSelection.draw(this.selectionContext, "rgba(10,40,240,0.9)");
    }
}

$(function() {
})
