import { Api } from './api.js';
import { updateGallery } from './gallery.js';

export async function initUploadForm() {
    const uploadButton = document.getElementById("file-input") as HTMLInputElement | null;
    const form = document.getElementById("upload-form") as HTMLFormElement | null;
    const status = document.getElementById("status") as HTMLParagraphElement | null;

    uploadButton?.addEventListener("change", () => {
        if (uploadButton.files?.length === 1) {
            if (status) status.textContent = `Selected: ${uploadButton.files[0].name}`;
            if (status) status.style.color = "var(--neutral)";
        }
    });

    form?.addEventListener("submit", async (event: SubmitEvent) => {
        event.preventDefault();

        if (status) status.style.color = "var(--neutral)";
        if (status) status.textContent = "Uploading...";

        const response = await Api.upload(form);

        if (status) status.textContent = await response.text();

        if (response.ok) {
            if (status) status.style.color = "var(--success)";
            form.reset();
            updateGallery();
        } else {
            if (status) status.style.color = "var(--success)";
        }
    });
}
