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
