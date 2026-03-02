import { activeTag, allMedia } from "./state.js";
import { selectTag } from "./events.js";
import { Api } from "./api.js";
import { Ids } from "./ids.js";

export async function loadSidebar() {
    const tags = await Api.getAllTags();
    const list = document.getElementById(Ids.tagList)!;
    const allItem = document.getElementById(Ids.allItem)!;

    document.getElementById(Ids.allCount)!.textContent = String(allMedia.length);

    list.innerHTML = "";
    list.appendChild(allItem);
    allItem.className = "tag-item" + (activeTag === "" ? " active" : "");

    for (const t of tags) {
        const el = document.createElement("div");
        el.className = "tag-item" + (activeTag === t.value ? " active" : "");
        el.dataset.tag = t.value;
        el.innerHTML = `<span class="tag-name">${t.value}</span><span class="tag-count">${t.count}</span>`;
        el.addEventListener("click", () => selectTag(t.value));
        list.appendChild(el);
    }
}
