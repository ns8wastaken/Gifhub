import { Ids } from "./ids.js";
import { loadGrid } from "./grid.js";
import { setActiveTag, setSearchQuery } from "./state.js";
import { refresh } from "./render.js";

export function selectTag(tag: string) {
    setActiveTag(tag);
    refresh();
}

export function initEvents() {
    // Search
    document.getElementById(Ids.search)!.addEventListener("input", (e) => {
        setSearchQuery((e.target as HTMLInputElement).value.toLowerCase().trim());
        // only re-render grid, no DB call needed
        loadGrid();
    })

    // Keyboard
    document.addEventListener("keydown", (e) => {
        if (e.key === "Escape") {
            document.getElementById(Ids.modalBg)!.classList.remove("visible");
        }
    })
}
