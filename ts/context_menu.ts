export function initContextMenu() {
    const menu = document.getElementById("context-menu") as HTMLDivElement | null;
    const gallery = document.getElementById("gallery") as HTMLDivElement | null;
    let currentImage: HTMLImageElement | null = null;

    gallery?.addEventListener("contextmenu", event => {
        const target = (event.target as HTMLElement).closest(".gallery-img") as HTMLImageElement;
        if (target && menu) {
            event.preventDefault();
            currentImage = target;
            menu.style.display = "block";
            menu.style.left = `${event.pageX}px`;
            menu.style.top = `${event.pageY}px`;
        }
    });

    document.addEventListener("click", () => {
        if (menu) menu.style.display = "none";
    });

    document.getElementById("remove-img")?.addEventListener("click", async () => {
        if (!currentImage) return;
        const uuid = currentImage.dataset.uuid;

        const response = await fetch(`/gallery/${uuid}`, {
            method: "DELETE"
        });

        if (response.ok) {
            currentImage.remove();
        }
    });
}
