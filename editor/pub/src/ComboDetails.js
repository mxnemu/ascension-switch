function ComboDetails() {
    this.setCombo(null);
}

ComboDetails.prototype.setCombo = function(combo) {
    var _this = this;
    this.saveCombo();
    this.combo = combo;
    if (combo) {
        $(".comboDetails input").removeAttr("disabled", "false");
        $(".comboDetails select").removeAttr("disabled", "false");
        
        $(".comboDetails .name").val(combo.name);
        $(".comboDetails .timeUntilReady").val(combo.timeUntilReady);
        $(".comboDetails .timeUntilReset").val(combo.timeUntilReset);
        $(".comboDetails .action").val(combo.action);
        
        $(".comboDetails input").bind("input", function() {
            _this.saveCombo();
        });
        
    } else {
        $(".comboDetails input").attr("disabled", "true");
        $(".comboDetails select").attr("disabled", "true");
        
        $(".comboDetails .name").val("");
        $(".comboDetails .timeUntilReady").val("");
        $(".comboDetails .timeUntilReset").val("");
        $(".comboDetails .action").val("");
    }
}

ComboDetails.prototype.saveCombo = function() {
    if (!this.combo) {
        return;
    }
    
    this.combo.name = $(".comboDetails .name").val();
    this.combo.timeUntilReady = $(".comboDetails .timeUntilReady").val();
    this.combo.timeUntilReset = $(".comboDetails .timeUntilReset").val();
    this.combo.action = $(".comboDetails .action").val();
    
    if (this.combo.nameNode) {
        this.combo.nameNode.text(this.combo.displayName);
    }
}
