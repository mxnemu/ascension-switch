function Combo(selection, name) {
    this.name = name || "";
    this.parent = null;
    this.timeUntilReady = 0;
    this.timeUntilReset = 2000;
    this.followUps = [];
    this.selection = selection;
    this.hitboxSelections = [];
}

Combo.prototype.serialize = function() {
    var serializedFollowUps = [];
    for (var key in this.followUps) {
        serializedFollowUps.push(this.followUps[key].serialize());
    }
    var serializedHitboxes = [];
    for (var key in this.hitboxSelections) {
        serializedFollowUps.push(this.hitboxSelections[key].serialize());
    }

    return {
        name: this.name,
        timeUntilRead: this.timeUntilRead,
        timeUntilReset: this.timeUntilReset,
        selection: this.selection.serialize(),
        followUps: serializedFollowUps,
        hitboxSelections: serializedHitboxes
    };
}

Combo.prototype.restore = function(serialized) {
    var restoredFollowUps = [];
    for (var key in serialized.followUps) {
        var followUp = new Combo(null);
        followUp.restore(serialized.followUps[key]);
        restoredFollowUps.push(followUp);
    }
    
    var restoredHitboxes = [];
    for (var key in serialized.hitboxSelections) {
        var hitbox = new Selection();
        hitbox.restore(serialized.hitboxSelections[key]);
        restoredHitboxes.push(hitbox);
    }
    
    var restoredSelection = new Selection();
    restoredSelection.restore(serialized.selection);

    this.name = serialized.name;
    this.timeUntilRead = serialized.timeUntilRead;
    this.timeUntilReset = serialized.timeUntilReset;
    this.selection = restoredSelection,
    this.followUps = restoredFollowUps;
    this.hitboxSelections = restoredHitboxes;
}

Combo.prototype.getIndexInParent = function() {
    if (!this.parent) {
        return -1;
    }
    
    for (var i=0; i < this.parent.followUps.length; ++i) {
        if (this.parent.followUps[i] == this) {
            return i;
        }
    }
    return -2;
}

Combo.prototype.addFollowUp = function(nextCombo) {
    this.followUps.push(nextCombo);
    nextCombo.parent = this;
}

function ComboList() {
    this.combos = [];
}

ComboList.prototype.serialize = function() {
    var serializedCombos = [];
    for (var key in this.combos) {
        serializedCombos.push(this.combos[key].serialize());
    }
    
    return {
        combos: serializedCombos
    };
}

ComboList.prototype.hasCollidingFrame = function(selection) {

}

ComboList.prototype.addFrame = function(selection) {
    var combo;
    if (this.selectedCombo) {
        combo = new Combo(selection);
        this.selectedCombo.addFollowUp(combo);
    } else {
        combo = new Combo(selection, "new combo");
        this.selectedCombo = combo;
        this.combos.push(combo);
    }
    
    this.addComboNode(combo);
    this.updateNodeSelection();
    console.log(JSON.stringify(this.serialize()));
}

ComboList.prototype.addComboNode = function(combo) {
    var parentNode = combo.parent ? combo.parent.node : $(".comboList");
    combo.node = $("<li class='combo'></li>");

    if (combo.name.length > 0) {
        var nextList = $("<ul></ul>");
        combo.node.addClass("branchedCombo");
        combo.node.text(combo.name);
        parentNode.append(combo.node);
        combo.node.append(nextList);
        combo.node = nextList;
    } else {
        combo.node.text("step "+combo.getIndexInParent());
        parentNode.append(combo.node);
    }
}

ComboList.prototype.updateNodeSelection = function() {
    if (!this.selectedCombo) {
        $(".combo").removeClass("selected");
    } else if (this.selectedCombo.node) {
        $(".combo").removeClass("selected");
        this.selectedCombo.node.addClass("selected");
    }
}
