function FrameList(entity) {
    this.entity = entity;
}

FrameList.prototype.addFrame = function(frame) {
    this.entity.frames.push(frame);
    this.addHtmlNode(frame);
}

FrameList.prototype.addHtmlNode = function(frame) {
    frame.node = $("<li class='frame'></li>");
    frame.nameNode = $("<span class='name'></span>");
    frame.nameNode.text("frame " + $.inArray(frame, this.entity.frames));
    frame.node.append(frame.nameNode);

    $(".frameList").append(frame.node);
}
