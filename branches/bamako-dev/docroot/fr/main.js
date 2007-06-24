function search() {
    q = document.getElementById("search-query").value;
    l = document.getElementById("lang").value;
    p = document.getElementById("project").value;
    document.location = "moulin-search://"+p+"/"+l+"/"+q;
}
