const express = require('express');

const BUILD_TYPE=process.argv[2]==="release"?"release":"debug";

console.log("Build type = " + BUILD_TYPE);

const app = express();
app.listen(8080);
//app.use(express.static(__dirname));
app.get("/example", (req, res) => {
    res.sendFile(__dirname + "/html/example.html");
});

app.get("/build/*", (req, res) => {
    const options = {
        root: "../cmake-build-emscripten-" + BUILD_TYPE
    };

    res.sendFile(req.path.substring(6), options);
});
