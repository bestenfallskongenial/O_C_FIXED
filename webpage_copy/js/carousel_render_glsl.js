// /js/carousel.js
(() => {
	const PAGE_SIZE = 8;
	const MIN_INDEX = 1;
	const MAX_INDEX = 40;

	const grid = document.getElementById("galleryGrid");
	const btnPrev = document.getElementById("galleryPrev");
	const btnNext = document.getElementById("galleryNext");
	const label = document.getElementById("galleryPageLabel");

	if (!grid || !btnPrev || !btnNext || !label) return;

	const FIRST_PAGE = MIN_INDEX;
	const LAST_PAGE  = MIN_INDEX + Math.floor((MAX_INDEX - MIN_INDEX) / PAGE_SIZE) * PAGE_SIZE;

		const GRID_COLUMNS = 2;
	const GRID_ROWS = Math.ceil(PAGE_SIZE / GRID_COLUMNS);
	grid.style.setProperty("--gallery-columns", GRID_COLUMNS);
	grid.style.setProperty("--gallery-rows", GRID_ROWS);

	let pageStart = FIRST_PAGE;

	function picPath(i) {
		const s = String(i).padStart(2, "0");
		return `/assets/carousel_render/pic${s}.jpg`;
	}

	function render() {
		const pageEnd = Math.min(pageStart + PAGE_SIZE - 1, MAX_INDEX);
		label.textContent = `${pageStart}–${pageEnd}`;

		grid.innerHTML = "";

		for (let i = 0; i < PAGE_SIZE; i++) {
			const idx = pageStart + i;

			const tile = document.createElement("div");
			tile.className = "thumb";

			if (idx <= MAX_INDEX) {
				const img = document.createElement("img");
				img.src = picPath(idx);
				img.alt = `pic${String(idx).padStart(2, "0")}`;
				img.loading = "lazy";
				tile.appendChild(img);
				tile.dataset.picIndex = String(idx);
			}

			grid.appendChild(tile);
		}
	}

	btnPrev.addEventListener("click", () => {
		pageStart -= PAGE_SIZE;
		if (pageStart < FIRST_PAGE) pageStart = LAST_PAGE;
		render();
	});

	btnNext.addEventListener("click", () => {
		pageStart += PAGE_SIZE;
		if (pageStart > LAST_PAGE) pageStart = FIRST_PAGE;
		render();
	});

	// === ADD THIS ===
	grid.addEventListener("click", (e) => {
		const tile = e.target.closest(".thumb");
		if (!tile) return;

		const idx = tile.dataset.picIndex;
		if (!idx) return;

		loadShaderByIndex(parseInt(idx, 10));
	});
	// === END ADD ===

	render();
})();
