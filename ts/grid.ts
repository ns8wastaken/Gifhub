import { activeTag, searchQuery, setAllMedia, allMedia } from "./state.js";
import { Ids } from "./ids.js";
import { Api } from "./api.js";
import { fuzzySort } from "./utils/fuzzy.js";

export async function loadGrid() {
    const images = await Api.getAllImages();
    console.log(images);
    setAllMedia(images);

    // console.log(allMedia);
    const filtered = fuzzySort(
        searchQuery
            ? allMedia.filter(m => m.tags.includes(activeTag))
            : allMedia,
        searchQuery,
        x => x.uuid
    );

    // console.log(filtered);

    document.getElementById(Ids.count)!.textContent =
        `${filtered.length} item${filtered.length !== 1 ? "s" : ""}`;

    const grid = document.getElementById(Ids.grid)!;
    grid.innerHTML = "";

    if (filtered.length === 0) {
        grid.innerHTML = `<div id="empty">
            <div class="big">◻</div>
            <div>no media found</div>
            <div style="font-size:10px;margin-top:4px;color:#444">drag & drop or use + add</div>
        </div>`;
        return;
    }

    for (const m of filtered) {
        const card = document.createElement("div");
        card.className = "media-card";
        card.dataset.id = String(m.uuid);

        const img = document.createElement("img");
        img.className = "media-thumb";
        img.loading = "lazy";
        img.src = `/gallery/${m.uuid}`;

        const tagPills = (m.tags || []).map((t: string) => `<span class="tag-pill">${t}</span>`).join("");
        const info = document.createElement("div");
        info.className = "media-info"
        info.innerHTML = `<div class="media-name">${m.uuid}</div><div class="media-tags">${tagPills}</div>`;

        card.appendChild(img);
        card.appendChild(info);
        // card.addEventListener("click", () => openModal(m.id));
        grid.appendChild(card);
    }
}
