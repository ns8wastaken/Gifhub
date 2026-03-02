import { GifhubImage } from "./types.js";

export let activeTag = "";
export let allMedia: GifhubImage[] = [];
export let searchQuery = "";
export let selectedId: string | null = null;

export function setActiveTag(tag: string)           { activeTag = tag; }
export function setAllMedia(media: GifhubImage[])   { allMedia = media; }
export function setSearchQuery(q: string)           { searchQuery = q; }
export function setSelectedId(id: string | null)    { selectedId = id; }
