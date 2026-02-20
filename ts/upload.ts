import { updateGallery } from './gallery.js';

export async function initUploadForm() {
    const uploadButton = document.getElementById("file-input") as HTMLInputElement | null;
    const form = document.getElementById("upload-form") as HTMLFormElement | null;
    const status = document.getElementById("status") as HTMLParagraphElement | null;

    uploadButton?.addEventListener("change", () => {
        if (uploadButton.files?.length === 1) {
            if (status) status.textContent = `Selected: ${uploadButton.files[0].name}`;
            if (status) status.style.color = "var(--accent)";
        }
    });

    form?.addEventListener("submit", async (event) => {
        event.preventDefault();

        if (status) status.textContent = "Uploading...";

        const response = await fetch("/upload", {
            method: "POST",
            body: new FormData(form)
        });

        if (status) status.textContent = await response.text();

        if (response.ok) {
            form.reset();
            updateGallery();
        }
    });
}
