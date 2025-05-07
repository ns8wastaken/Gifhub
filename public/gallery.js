function update_gallery() {
    fetch('/api/images')
        .then(res => res.json())
        .then(images_paths => {
            const gallery = document.getElementById('gallery');
            gallery.innerHTML = ''

            images_paths.forEach(name => {
                const img = document.createElement('img');
                img.className = 'gallery-img';
                img.src = `/gallery/${name}`;
                img.alt = name;
                gallery.appendChild(img);
            });
        });
}

async function search_images(event) {
        event.preventDefault();

        const query = document.getElementById('search-input').value;
        if (query === '') {
            update_gallery();
            return;
        }

        const result = await fetch(`/api/search?q=${encodeURIComponent(query)}`);
        const images = await result.json();

        const gallery = document.getElementById('gallery');
        gallery.innerHTML = '';

        images.forEach(image => {
            const img = document.createElement('img');
            img.className = 'gallery-img';
            img.src = `/gallery/${image.uuid}`;
            gallery.appendChild(img);
        });
}

document.getElementById('search').addEventListener('submit', search_images)

async function reset_gallery() {
    const response = await fetch('/api/reset-gallery', { method: 'POST' });
    const result = await response.text();

    console.log(result);

    update_gallery();
}

// Load gallery on refresh / page load
update_gallery();
