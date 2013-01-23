function ComboList(comboDetails) {
    this.combos = [];
    this.selectedBranch = null;
    this.selectedCombo = null;
    this.comboDetails = comboDetails;
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
    if (this.selectedBranch) {
        combo = new Combo(selection);
        this.selectedBranch.addFollowUp(combo);
    } else {
        combo = new Combo(selection, "new combo");
        this.selectedBranch = combo;
        this.selectedCombo = combo;
        this.combos.push(combo);
    }
    
    this.addComboNode(combo);
    this.updateNodeSelection();
    console.log(JSON.stringify(this.serialize()));
}

ComboList.prototype.addComboNode = function(combo) {
    var _this = this;
    var parentNode = combo.parent ? combo.parent.listNode : $(".comboList");
    combo.node = $("<li></li>");
    
    combo.nameNode = $("<span class='combo'></span>");
    

    if (combo.name.length > 0) {
        var nextList = $("<ul></ul>");
        combo.nameNode.addClass("branchedCombo");
        combo.nameNode.text(combo.name);
        combo.node.append(combo.nameNode);
        combo.node.append(nextList);
        combo.listNode = nextList;
    } else {
        combo.nameNode.text("step "+combo.getIndexInParent());
        combo.node.append(combo.nameNode);
    }
    
    combo.nameNode.click(function() {
        _this.selectedCombo = combo;
        _this.updateNodeSelection();
        $(".comboDetails .name").get(0).focus();
    });
    
    parentNode.append(combo.node);
}

ComboList.prototype.updateNodeSelection = function() {
    if (!this.selectedCombo) {
        $(".combo").removeClass("selected");
    } else if (this.selectedCombo.node) {
        $(".combo").removeClass("selected");
        this.selectedCombo.nameNode.addClass("selected");
    }
    this.comboDetails.setCombo(this.selectedCombo);
}
