import { Api } from "./api.js";
import { GifhubImage } from "./types.js"

export async function updateGallery() {
    try {
        let images = await Api.fetchImages();
        renderImages(images);
    } catch (err) {
        console.error("Failed to load gallery:", err);
    }
}

export function renderImages(images: GifhubImage[]) {
    const gallery = document.getElementById("gallery");
    if (!gallery) return;
    gallery.innerHTML = "";

    images.forEach(image => {
        const uuid = typeof image === "string" ? image : image.uuid;
        const img = document.createElement("img");
        img.className = "gallery-img";
        img.src = `/gallery/${uuid}`;
        img.dataset.uuid = uuid;
        img.loading = "lazy";
        gallery.appendChild(img);
    });
}

export async function searchImages(query: string) {
    if (!query) return updateGallery();
    try {
        const res = await fetch(`/gallery/search?q=${encodeURIComponent(query)}`);
        const images = await res.json();
        renderImages(images);
    } catch (err) {
        console.error("Search failed:", err);
    }
}
