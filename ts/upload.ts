import { Ids } from "./ids.js";
import { UploadModal } from "./upload_modal.js";

export function initUploads() {
    const uploader = new UploadModal();

    const overlay = document.getElementById(Ids.dropOverlay)!;
    let dragCounter = 0;

    document.addEventListener("dragenter", (e) => {
        e.preventDefault();
        dragCounter++;
        overlay.classList.add("visible");
    });

    document.addEventListener("dragleave", () => {
        dragCounter--;
        if (dragCounter <= 0) {
            dragCounter = 0;
            overlay.classList.remove("visible");
        }
    });

    document.addEventListener("dragover", (e) => e.preventDefault());

    document.addEventListener("drop", (e) => {
        e.preventDefault();
        dragCounter = 0;
        overlay.classList.remove("visible");

        const files = e.dataTransfer?.files;
        if (files && files.length > 0) {
            uploader.openModal(files[0]);
        }
    });
}
