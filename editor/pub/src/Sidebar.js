function Sidebar() {
    this.content = new FrameList();
    this.entity = null;
    this.tabName = "frames";
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
    this.tabName = tabName;
    
    if (tabName == "frames") {
        this.content = new FrameList(this.entity);
    } else if (tabName == "combos") {
        this.content = new ComboList(this.entity);
    } else if (tabName == "animations") {
        //this.content = new AnimationList(); TODO implement
    }
}

Sidebar.prototype.addFrame = function(frame) {
    if (this.content.addFrame) {
        this.content.addFrame(frame);
    }
}

Sidebar.prototype.selectFrame = function(frame) {
    if (this.content.selectFrame) {
        this.content.selectFrame(frame);
    }
}
