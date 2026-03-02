import { Api } from "./api.js";
import { Ids } from "./ids.js";

export class UploadModal {
    // DOM Elements
    private fileInput     = document.getElementById(Ids.fileInput) as HTMLInputElement;
    private modalBg       = document.getElementById(Ids.modalBg) as HTMLDivElement;
    private modalClose    = document.getElementById(Ids.modalClose) as HTMLButtonElement;
    private modalPreview  = document.getElementById(Ids.modalPreview) as HTMLImageElement;
    private modalFilename = document.getElementById(Ids.modalFilename) as HTMLSpanElement;
    private tagInput      = document.getElementById(Ids.modalTagInput) as HTMLInputElement;
    private tagAddBtn     = document.getElementById(Ids.modalTagAddBtn) as HTMLButtonElement;
    private tagList       = document.getElementById(Ids.modalTagList) as HTMLDivElement;
    private btnCancel     = document.getElementById(Ids.modalBtnCancel) as HTMLButtonElement;
    private btnSave       = document.getElementById(Ids.modalBtnSave) as HTMLButtonElement;

    // State
    private tags: string[] = [];
    private currentFile: File | null = null;
    private currentObjectUrl: string | null = null;

    constructor() {
        this.initEventListeners();
    }

    public openModal(file: File): void {
        this.currentFile = file;

        this.currentObjectUrl = URL.createObjectURL(file);

        this.modalPreview.src = this.currentObjectUrl;
        this.modalFilename.textContent = file.name;
        this.tags = [];
        this.renderTags();

        this.tagInput.value = "";
        this.modalBg.classList.add("visible");
        setTimeout(this.tagInput.focus, 200);
    }

    private async handleSave(): Promise<void> {
        if (!this.currentFile) return;

        // console.log("Saved:", {
        //     filename: this.modalFilename.textContent,
        //     tags: [...this.tags]
        // });

        const formData = new FormData();
        formData.append("file", this.currentFile);

        this.tags.forEach(tag => {
            formData.append("tags", tag);
        });

        Api.upload(formData);

        this.closeModal();
    }

    private initEventListeners(): void {
        // File selection via Input
        this.fileInput.addEventListener("change", (e: Event) => {
            const file = (e.target as HTMLInputElement).files?.[0];
            if (file) this.openModal(file);
        });

        // Tag Logic
        this.tagAddBtn.addEventListener("click", this.addTag);

        this.tagInput.addEventListener("keydown", (e: KeyboardEvent) => {
            if (e.key === "Enter") {
                e.preventDefault();
                this.addTag();
            }
            if (e.key === "Backspace" && this.tagInput.value === "" && this.tags.length) {
                this.tags.pop();
                this.renderTags();
            }
        });

        this.tagList.addEventListener("click", (e: MouseEvent) => {
            const btn = (e.target as HTMLElement).closest(".modal-tag-remove") as HTMLButtonElement;
            if (!btn) return;
            const index = parseInt(btn.dataset.i || "0");
            this.tags.splice(index, 1);
            this.renderTags();
        });

        // Modal Controls
        this.modalClose.addEventListener("click", this.closeModal);
        this.btnCancel.addEventListener("click", this.closeModal);
        this.modalBg.addEventListener("click", (e: MouseEvent) => {
            if (e.target === this.modalBg) this.closeModal();
        });

        this.btnSave.addEventListener("click", () => {
            this.handleSave();
            this.closeModal();
        });
    }

    private closeModal(): void {
        this.modalBg.classList.remove("visible");
        this.fileInput.value = "";
        this.currentFile = null;
        if (this.currentObjectUrl)
            URL.revokeObjectURL(this.currentObjectUrl);
    }

    private addTag(): void {
        const val = this.tagInput.value.trim().toLowerCase().replace(/\s+/g, "-");
        if (!val || this.tags.includes(val)) {
            this.tagInput.value = "";
            return;
        }
        this.tags.push(val);
        this.tagInput.value = "";
        this.renderTags();
        this.tagInput.focus();
    }

    private renderTags(): void {
        this.tagList.innerHTML = this.tags
            .map((tag, i) => `
                <div class="modal-tag-pill">
                    ${tag}
                    <button class="modal-tag-remove" data-i="${i}" title="remove">×</button>
                </div>
            `).join("");
    }
}
