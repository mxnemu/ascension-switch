function AnimationList() {

}

AnimationList.prototype.addFrame = function(frame) {
    this.addHtmlNode(frame);
}


AnimationList.prototype.rebuildHtmlTree = function() {
    $(".animationList").empty();
    for (var i=0; i < this.entity.frames.length; ++i) {
        this.addHtmlNode(this.entity.frames[i]);
    }
}

AnimationList.prototype.addHtmlNode = function(animation) {
    var _this = this;
    this.setHtmlSelection();
}

AnimationList.prototype.setHtmlSelection = function() {
    $(".animationList .animation .name").removeClass("selected");
}
