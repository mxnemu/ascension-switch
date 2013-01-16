function FrameSelector() {
    var _this = this;
    
    this.canvas = $(".spriteSheet").get(0);
    this.context = this.canvas.getContext("2d");
    this.selectionCanvas = $(".spriteSheetSelection").get(0)
    this.selectionContext = this.selectionCanvas.getContext("2d");

    this.currentSelection = null;
    this.selections = [];

    $(".spriteSheetSelection").mousedown(function(event) {
        _this.currentSelection = new Selection(event.offsetX, event.offsetY);
    });
    
    $(".spriteSheetSelection").mousemove(function(event) {
        if (_this.currentSelection) {
            _this.currentSelection.w = event.offsetX - _this.currentSelection.x;
            _this.currentSelection.h = event.offsetY - _this.currentSelection.y;
        }
    });
    
    $(".spriteSheetSelection").click(function(event) {
        _this.currentSelection.w = event.offsetX - _this.currentSelection.x;
        _this.currentSelection.h = event.offsetY - _this.currentSelection.y;
        if (app.options.snapToAlpha) {
            _this.currentSelection.snapToAlpha(_this.context);
        }
        if (_this.currentSelection.isValid()) {        
            _this.selections.push(_this.currentSelection);
        }
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
        this.selections[i].draw(this.selectionContext);
    }
}

$(function() {
})
