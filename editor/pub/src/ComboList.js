function ComboList(entity) {
    this.entity = entity;
    this.selectedBranch = null;
    this.selectedCombo = null;
    this.comboDetails = new ComboDetails();
}

// TODO call via event
ComboList.prototype.removeCombo = function(combo) {
    if (this.entity.removeCombo(combo)) {
        this.selectedCombo = null; //TODO only reset if affected
        this.rebuildHtmlTree();
        return true;
    }
    return false;
}

ComboList.prototype.hasCollidingFrame = function(selection) {

}

ComboList.prototype.selectFrame = function(frame) {
    var combo;
    if (this.selectedCombo) {
        combo = new Combo(frame);
        this.selectedCombo.addFollowUp(combo);
    } else {
        combo = new Combo(frame, "new combo");
        this.entity.combos.push(combo);
    }
    this.selectedCombo = combo;
    this.addComboNode(combo);
    this.updateNodeSelection();
    //console.log(JSON.stringify(this.entity.serialize()));
    this.entity.save();
}

ComboList.prototype.rebuildHtmlTree = function() {
    var _this = this;
    $(".comboList").empty();
    var rebuildChildNodes = function (array) {
        for (var i=0; i < array.length; ++i) {
            _this.addComboNode(array[i]);
            rebuildChildNodes(array[i].followUps);
        }
    }
    rebuildChildNodes(this.entity.combos);
    this.updateNodeSelection();
}

ComboList.prototype.addComboNode = function(combo) {
    var _this = this;
    var parentNode = combo.parent ? combo.parent.listNode : $(".comboList");
    combo.node = $("<li></li>");
    
    combo.nameNode = $("<span class='combo'></span>");
    
    combo.nameNode.text(combo.displayName);
    
    var nextList = $("<ul></ul>");
    combo.node.append(combo.nameNode);
    combo.node.append(nextList);
    combo.listNode = nextList;
    
    combo.nameNode.click(function() {
        _this.selectedCombo = combo;
        _this.updateNodeSelection();
        $(".comboDetails .name").get(0).focus();
    });
    
    combo.nameNode.bind('contextmenu', function(e) {
        _this.removeCombo(combo);
        return false;
    });
    
    parentNode.append(combo.node);
}

ComboList.prototype.updateNodeSelection = function() {
    $(".combo").removeClass("selected");
    if (this.selectedCombo.node) {
        this.selectedCombo.nameNode.addClass("selected");
        this.selectedFrame = this.selectedCombo.selection;
    } else {
        this.selectedFrame = null;   
    }
    this.comboDetails.setCombo(this.selectedCombo);
}
