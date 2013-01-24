function FrameSelector(comboList) {
    var _this = this;
    this.comboList = comboList;
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
                _this.comboList.addFrame(_this.currentSelection);
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
    this.comboList.entity = this.entity;
}

FrameSelector.prototype.draw = function() {
    var _this = this;
    this.selectionContext.clearRect(0,0,this.selectionCanvas.width, this.selectionCanvas.height);
    
    var drawCombos = function(combos) {
        for (var i=0; i < combos.length; ++i) {
            
            if (_this.comboList.selectedCombo != combos[i]) {
                combos[i].selection.draw(_this.selectionContext);
            } else {
                combos[i].selection.draw(_this.selectionContext, "rgba(10,40,240,0.9)");
            }
            drawCombos(combos[i].followUps);
        }
    }
    drawCombos(this.entity.combos);
    
    if (this.currentSelection) {
        this.currentSelection.draw(this.selectionContext, "rgba(10,40,240,0.9)");
    }
}

$(function() {
})
