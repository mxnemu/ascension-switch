function Sidebar() {

}

Sidebar.prototype.init = function() {
    var _this = this;
    $(".sidebar .tabs .tabButton.frames").click(function() {
        _this.showTab("frames");
    });
    $(".sidebar .tabs .tabButton.combos").click(function() {
        _this.showTab("combos");
    });
    $(".sidebar .tabs .tabButton.animations").click(function() {
        _this.showTab("animations");
    });
}

Sidebar.prototype.showTab = function(tabName) {
    $(".sidebar .content").hide("slow");
    $(".sidebar .content." + tabName).show("slow");
    $(".sidebar .tabs .tabButton").removeClass("selected");
    $(".sidebar .tabs .tabButton." + tabName).addClass("selected");
}
