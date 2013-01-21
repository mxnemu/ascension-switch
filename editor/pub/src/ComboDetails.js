function ComboDetails() {
    this.setCombo(null);
}

ComboDetails.prototype.setCombo = function(combo) {
    if (combo) {
        $(".comboDetails input").removeAttr("disabled", "false");
        $(".comboDetails select").removeAttr("disabled", "false");
        
        $(".comboDetails .name").val(combo.name);
        $(".comboDetails .timeUntilReady").val(combo.timeUntilReady);
        $(".comboDetails .timeUntilReset").val(combo.timeUntilReset);
        $(".comboDetails .action").val(combo.action);
    } else {
        $(".comboDetails input").attr("disabled", "true");
        $(".comboDetails select").attr("disabled", "true");
        
        $(".comboDetails .name").val("");
        $(".comboDetails .timeUntilReady").val("");
        $(".comboDetails .timeUntilReset").val("");
        $(".comboDetails .action").val("");
    }
}
