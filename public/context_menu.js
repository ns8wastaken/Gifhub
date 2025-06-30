const contextMenu = document.getElementById('context-menu');
const gallery = document.getElementById('gallery');

let currentImage = null;

// Show the context menu
gallery.addEventListener('contextmenu', (event) => {
    event.preventDefault();

    console.log(event.target.classList);
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

document.getElementById('remove-img').addEventListener('click', async () => {
    if (currentImage) {
        currentImage.remove();
        contextMenu.style.display = 'none';

        const response = await fetch(`/gallery/${currentImage.uuid}`, {
            method: 'DELETE'
        });
        const result = await response.text();
    }
});
