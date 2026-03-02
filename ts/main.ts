import { initEvents } from "./events.js";
import { initUploads } from "./upload.js";
import { refresh } from "./render.js";

document.addEventListener('DOMContentLoaded', () => {
    initEvents();
    initUploads();
    refresh();
});
