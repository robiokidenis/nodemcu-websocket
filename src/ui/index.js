const socket = io();

const toggleBtn = document.getElementById('toggleBtn');

let buttonState = false;

toggleBtn.addEventListener('click', () => {
    buttonState = !buttonState;
    updateUI();
    socket.emit('buttonState', buttonState);
    // socket.emit('logging', "hitted");
    // socket.emit('1', "hitted");
});

const updateUI = () => {
    buttonState
        ? toggleBtn.classList.add('on')
        : toggleBtn.classList.remove('on');
    toggleBtn.innerText = buttonState ? 'Received' : 'Feeding';
};

socket.on('buttonState', state => {
    console.log('updated state', state);
    buttonState = state;
   
});
socket.on('logging', state => {
    console.log('Hitted', state);
    // document.getElementById('tlogging').value=JSON.parse(state);
    updateUI();
});
