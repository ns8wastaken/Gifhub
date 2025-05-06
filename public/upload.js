const form = document.getElementById('upload-form');
const statusText = document.getElementById('status');

form.addEventListener('submit', async (event) => {
    event.preventDefault();

    const formData = new FormData(form);

    const response = await fetch('/upload', {
        method: 'POST',
        body: formData
    });

    const result = await response.text();
    statusText.textContent = result;

    update_gallery()
});
