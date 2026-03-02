import { GifhubImage, TagItem } from "./types.js";

export const Api = {
    // POST an image
    async upload(formData: FormData): Promise<Response> {
        return await fetch("/upload", {
            method: "POST",
            body: formData
        });
    },

    // GET all images
    async getAllImages(): Promise<GifhubImage[]> {
        const res = await fetch("/gallery/images");
        if (!res.ok) throw new Error("Failed to get images");
        return res.json();
    },

    // GET all tags and the number of times they appear
    async getAllTags(): Promise<TagItem[]> {
        const res = await fetch("/gallery/tags");
        if (!res.ok) throw new Error("Failed to get tags");
        return res.json();
    },

    // GET image's tags
    async getTags(uuid: string): Promise<string[]> {
        const res = await fetch(`/gallery/${uuid}/tags`);
        if (!res.ok) throw new Error("Failed to get image's tags");
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
