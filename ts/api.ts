import { GifhubImage } from "./types.js";

export const Api = {
    // POST an image
    async upload(form: HTMLFormElement): Promise<Response> {
        return fetch("/upload", {
            method: "POST",
            body: new FormData(form)
        });
    },

    // GET all images
    async fetchImages(): Promise<GifhubImage[]> {
        const res = await fetch("/gallery/images");
        if (!res.ok) throw new Error("Failed to load gallery");
        return res.json();
    },

    // GET image's tags
    async fetchTags(uuid: string): Promise<string[]> {
        const res = await fetch(`/gallery/${uuid}/tags`);
        if (!res.ok) throw new Error("Failed to load gallery");
        return res.json();
    },

    // DELETE an image
    async deleteImage(uuid: string): Promise<Response> {
        return fetch(`/gallery/${uuid}`, {
            method: "DELETE"
        });
    },

    // UPDATE tags
    async updateTags(uuid: string, tags: string[]): Promise<Response> {
        return fetch(`/gallery/${uuid}/tags`, {
            method: "PATCH",
            headers: { "Content-Type": "application/json" },
            body: JSON.stringify({ tags })
        });
    }
};
