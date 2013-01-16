function Options() {

}


Object.defineProperty(Options.prototype, "snapToAlpha", {
    get: function() {
        return true;
    },
    set: function(val) {
        alert("Thy must not set readonly values!");
    }
}); 
