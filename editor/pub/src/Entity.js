function Entity() {
    this.frames = [];
    this.combos = [];
    this.animations = [];
}

Entity.prototype.save = function() {
    //TODO call nodejs api to save json
}

Entity.prototype.serialize = function() {
    var serializedFrames = [];
    for (var i=0; i < this.frames.length; ++i) {
        serializedFrames.push(this.frames[i].serialize());
    }
    
    var serializedCombos = [];
    for (var i=0; i < this.combos.length; ++i) {
        serializedCombos.push(this.combos[i].serialize());
    }
    
    var serializedAnimations = [];
    for (var i=0; i < this.animations.length; ++i) {
        serializedAnimations.push(this.animations[i].serialize());
    }
    
    return {
        frames: serializedFrames,
        combos: serializedCombos,
        animations: serializedAnimations
    };
}

Entity.prototype.removeCombo = function(combo) {
    var removed = false;
    var removeCombo = function (array) {
        for (var i=0; i < array.length; ++i) {
            if (array[i] == combo) {
                array.splice(i, 1);
                removed = true;
                return true;
            } else {
                removeCombo(array[i].followUps);
            }
        }
    }
    removeCombo(this.combos);
    
    return removed;
}

Entity.prototype.getFrameForPosition = function(x, y) {
    for (var i=0; i < this.frames.length; ++i) {
        if (this.frames[i].isInside(x,y)) {
            return this.frames[i];
        }
    }
    return null;
}
