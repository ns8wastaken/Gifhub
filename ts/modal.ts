type Urgency = "low" | "medium" | "high";

export function modal(
    title: string,
    message: string,
    showCancel: boolean = true,
    urgency: Urgency = "medium"
): Promise<boolean> {
    const container = document.getElementById("custom-modal-container") as HTMLDivElement;
    const titleEl = document.getElementById("modal-title")!;
    const messageEl = document.getElementById("modal-message")!;
    const confirmBtn = document.getElementById("modal-confirm") as HTMLButtonElement;
    const cancelBtn = document.getElementById("modal-cancel") as HTMLButtonElement;

    // Set text
    titleEl.textContent = title;
    messageEl.textContent = message;

    // Format stuff
    cancelBtn.style.display = showCancel ? "block" : "none";

    container.setAttribute("data-urgency", urgency);
    container.style.display = "flex";

    return new Promise((resolve) => {
        const cleanup = (result: boolean) => {
            container.style.display = "none";
            confirmBtn.onclick = null;
            cancelBtn.onclick = null;
            resolve(result);
        };

        confirmBtn.onclick = () => cleanup(true);
        cancelBtn.onclick = () => cleanup(false);
    });
}

export function editModal(uuid: string, currentTags: string[]) {
    const container = document.getElementById("edit-modal-container")!;
    const previewImg = document.getElementById("edit-preview-img") as HTMLImageElement;
    const tagInput = document.getElementById("edit-tags-input") as HTMLInputElement;
    const saveBtn = document.getElementById("edit-save")!;
    const cancelBtn = document.getElementById("edit-cancel")!;

    // 1. Setup initial state
    previewImg.src = `/gallery/${uuid}`;
    tagInput.value = currentTags.join(", ");
    container.style.display = "flex";
    tagInput.focus();

    return new Promise((resolve) => {
        const close = () => {
            container.style.display = "none";
            saveBtn.onclick = null;
            cancelBtn.onclick = null;
        };

        cancelBtn.onclick = () => {
            close();
            resolve(null);
        };

        saveBtn.onclick = async () => {
            const newTags = tagInput.value
                .split(",")
                .map(t => t.trim())
                .filter(t => t !== "");

            close();
            resolve(newTags); // Return the array of tags to the caller
        };
    });
}
