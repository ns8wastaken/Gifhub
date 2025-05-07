const contextMenu = document.getElementById('context-menu');
const gallery = document.getElementById('gallery');
let currentImage = null;
// const images = document.getElementsByClassName('gallery-img');

// Show the context menu
document.addEventListener('contextmenu', (event) => {
    // event.preventDefault();
    //
    // contextMenu.style.display = 'block';
    // contextMenu.style.left = `${event.pageX}px`;
    // contextMenu.style.top = `${event.pageY}px`;

    event.preventDefault();

    if (event.target.classList.contains('gallery-img')) {
        currentImage = event.target;
        contextMenu.style.display = 'block';
        contextMenu.style.left = `${event.pageX}px`;
        contextMenu.style.top = `${event.pageY}px`;
    } else {
        contextMenu.style.display = 'none';
    }
});

// Hide the context menu
document.addEventListener('click', () => {
    contextMenu.style.display = 'none';
});

document.getElementById('remove-img').addEventListener('click', () => {
    if (currentImage) {
        currentImage.remove();
        contextMenu.style.display = 'none';
    }
});
