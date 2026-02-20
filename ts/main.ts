import { updateGallery, searchImages } from "./gallery.js";
import { initContextMenu } from "./context_menu.js";
import { initUploadForm } from "./upload.js";

function main() {
    // Initialize modular components
    initContextMenu();
    initUploadForm();

    // Handle Search
    const searchForm = document.getElementById("search-form");
    searchForm?.addEventListener("submit", (e) => {
        e.preventDefault();
        const searchInput = document.getElementById("search-input") as HTMLInputElement;
        const query = searchInput?.value;
        searchImages(query);
    });

    // Initial load
    updateGallery();
}

main();
