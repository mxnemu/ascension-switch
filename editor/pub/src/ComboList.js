function Combo(selection, name) {
    this.name = name || "";
    this.parent = null;
    this.timeUntilReady = 0;
    this.timeUntilReset = 2000;
    this.followUps = [];
    this.selection = selection;
    this.hitboxSelections = [];
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

function ComboList() {
    this.combos = [];
}

Combo.prototype.addFollowUp = function(nextCombo) {
    this.followUps.push(nextCombo);
    nextCombo.parent = this;
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
}

ComboList.prototype.addComboNode = function(combo) {
    var parentNode = combo.parent ? combo.parent.node : $(".comboList");
    combo.node = $("<li></li>");

    if (combo.name.length > 0) {
        var nextList = $("<ul></ul>");
        combo.node.text(combo.name);
        parentNode.append(combo.node);
        combo.node.append(nextList);
        combo.node = nextList;
    } else {
        combo.node.text(combo.getIndexInParent());
        parentNode.append(combo.node);
    }
       
    
}
