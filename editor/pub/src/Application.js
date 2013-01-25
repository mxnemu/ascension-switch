function Application() {
    this.options = new Options();
    this.sidebar = new Sidebar();
    this.frameSelector = new FrameSelector(this.sidebar);
}

Application.prototype.init = function() {
    this.sidebar.init();
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
