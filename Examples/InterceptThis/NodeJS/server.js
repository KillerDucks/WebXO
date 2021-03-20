var net = require('net');

var server = net.createServer();

server.on("connection", (client) => {
    client.setEncoding("ascii");


    client.on("data", (data) => {
        console.log("Message");

        if(client.writable)
        {
            client.write("test");
        }

    });


});

server.listen(8089, '127.0.0.1');