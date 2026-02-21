import { modal } from './modal.js';
import { updateGallery } from './gallery.js';

export async function initNukeButton() {
    const nukeBtn = document.getElementById('nuke-btn') as HTMLButtonElement | null;

    nukeBtn?.addEventListener("click", async () => {
        // const confirmed = confirm("WARNING: This will delete ALL images and reset the database. Are you sure?");
        const confirmed = await modal(
            "Nuke Gallery?",
            "This will permanently delete all GIFs and tags. Proceed?",
            true,
            "high"
        );

        if (confirmed) {
            try {
                const response = await fetch("/api/nuke-files", {
                    method: "POST"
                });

                if (response.ok) {
                    modal("Gallery nuked successfully.", "", false);
                    updateGallery(); // Refresh the UI
                } else {
                    const err = await response.text();
                    modal("Nuke failed", err, false);
                }
            } catch (error) {
                console.error("Network error during nuke:", error);
            }
        }
    });
}
