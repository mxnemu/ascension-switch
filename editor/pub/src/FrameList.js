function FrameList(entity) {
    this.entity = entity;
    this.selectedFrame = null;
}

FrameList.prototype.addFrame = function(frame) {
    this.addHtmlNode(frame);
}


FrameList.prototype.rebuildHtmlTree = function() {
    $(".frameList").empty();
    for (var i=0; i < this.enttiy.frames.length; ++i) {
        this.addHtmlNode(this.enttiy.frames[i]);
    }
    this.setHtmlSelection();
}


FrameList.prototype.addHtmlNode = function(frame) {
    frame.node = $("<li class='frame'></li>");
    frame.nameNode = $("<span class='name'></span>");
    frame.nameNode.text("frame " + $.inArray(frame, this.entity.frames));
    frame.node.append(frame.nameNode);

    $(".frameList").append(frame.node);
    this.selectedFrame = frame;
    this.setHtmlSelection()
}

FrameList.prototype.setHtmlSelection = function() {
    $(".frameList .frame .name").removeClass("selected");
    if (this.selectedFrame) {
        this.selectedFrame.nameNode.addClass("selected");
    }
}
