import { Api } from "./api.js";
import { editModal } from "./modal.js";

export function initContextMenu() {
    const menu = document.getElementById("context-menu") as HTMLDivElement | null;
    const gallery = document.getElementById("gallery") as HTMLDivElement | null;
    let currentImage: HTMLImageElement | null = null;

    // Show / hide menu
    gallery?.addEventListener("contextmenu", (event) => {
        event.preventDefault();

        const target = (event.target as HTMLElement)
            .closest(".gallery-img") as HTMLImageElement | null;

        if (target && menu) {
            currentImage = target;

            menu.classList.add("visible");

            const menuWidth = menu.offsetWidth;
            const menuHeight = menu.offsetHeight;
            const windowWidth = window.innerWidth;
            const windowHeight = window.innerHeight;

            // Flip menu if too close to right edge
            let x = event.clientX;
            let y = event.clientY;

            if (x + menuWidth > windowWidth) x = x - menuWidth;
            if (y + menuHeight > windowHeight) y = y - menuHeight;

            menu.style.left = `${x}px`;
            menu.style.top = `${y}px`;

            // Close the menu when clicking anywhere else
            const closeMenu = () => {
                menu.classList.remove("visible");
                document.removeEventListener("click", closeMenu);
            };
            document.addEventListener("click", closeMenu);
        }
    });

    menu?.addEventListener("click", async (event) => {
        const target = event.target as HTMLElement;
        const action = target.dataset.action;

        if (!action) return;
        if (!currentImage) return;

        const uuid = currentImage.dataset.uuid as string;

        switch (action) {
            case "edit-tags": {
                const tags = await Api.fetchTags(uuid);
                editModal(uuid, tags);
                break;
            }

            case "delete": {
                const response = await Api.deleteImage(uuid);
                if (response.ok) {
                    currentImage.remove();
                }
                break;
            }
        }
    });
}
