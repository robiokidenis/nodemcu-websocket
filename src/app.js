import express from 'express';
import http from 'http';
import { Server } from 'socket.io';

const PORT = process.env.PORT || 4001;

const app = express();
const httpServer = http.createServer(app);
const io = new Server(httpServer, { cors: { origin: '*' } });

app.use(express.static('src/ui'));

let buttonState = false;
var connected = 0;
io.on('connection', socket => {

    connected++;
    console.log('New Connection client ',connected);
    io.to(socket.id).emit('buttonState', buttonState);

    io.to(socket.id).emit("update",{
        clients:connected,
    });

    socket.on('disconnect', () => {
        connected--;
        console.log('Disconnected ',connected);
        socket.broadcast.emit("update",{
            clients:connected
        });
    });

    socket.on('buttonState', value => {
        console.log('buttonState:', value);
        buttonState = value;
        socket.broadcast.emit('buttonState', value);
    });
    socket.on('logging', value => {
        console.log('log:', value);
        socket.broadcast.emit('logging', value);
    });
});


httpServer.listen(PORT, () => {
    console.log('Running on : ', httpServer.address());
});
