function FrameList(entity) {
    this.entity = entity;
}

FrameList.prototype.addFrame = function(frame) {
    this.entity.frames.push(frame);
}
