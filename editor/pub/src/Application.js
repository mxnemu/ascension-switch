function Application() {
    this.options = new Options();
    this.frameSelector = new FrameSelector();
}

Application.prototype.init = function() {
    this.frameSelector.loadSpriteSheet("images/person.png");
}

Application.prototype.startDraw = function() {
    var _this = this;
    var drawFunction;
    drawFunction = function() {
        _this.frameSelector.draw();
        window.requestAnimFrame(drawFunction);
    };
    
    window.requestAnimFrame(drawFunction);

}

$(function() {
    window.app = new Application();
    app.init();
    app.startDraw();

});
