import { loadGrid } from "./grid.js";
import { loadSidebar } from "./sidebar.js";

export async function refresh() {
    await loadGrid();
    await loadSidebar();
}
