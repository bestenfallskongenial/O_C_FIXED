// /js/index_carousel.js
// deterministic 3-tile carousel
// text scrolling handled via inner scroll container (.tile-text-scroll)

(() => {
	const MIN_INDEX = 1;
	const MAX_INDEX = 8;

	/* === START CONFIG === */
	const START_LEFT = 1; // left tile start index (controls initial state)
	/* === END CONFIG === */

	// navigation buttons
	const btnPrev = document.getElementById("indexPrev");
	const btnNext = document.getElementById("indexNext");

	// tile slots (fixed DOM positions)
	const tiles = {
		left:   document.querySelector('.index-tile[data-slot="left"]'),
		center: document.querySelector('.index-tile[data-slot="center"]'),
		right:  document.querySelector('.index-tile[data-slot="right"]')
	};

	// hard stop if DOM is incomplete
	if (!btnPrev || !btnNext || !tiles.left || !tiles.center || !tiles.right) return;

	// circular index helper
	function wrap(i) {
		if (i < MIN_INDEX) return MAX_INDEX;
		if (i > MAX_INDEX) return MIN_INDEX;
		return i;
	}

	// center index is derived from left index
	let centerIndex = wrap(START_LEFT + 1);

	// asset path helpers
	function imgPath(i) {
		const s = String(i).padStart(2, "0");
	//	return `/assets/carousel_index/page${s}.jpg`;
		return `/assets/carousel_index_pic${s}.jpg`;
	}

	function textPath(i) {
		const s = String(i).padStart(2, "0");
	//	return `/assets/texts/item${s}.html`;   // HTML, not TXT
		return `/assets/carousel_index_text${s}.html`;   // HTML, not TXT
	}

	function linkPath(i) {
		const s = String(i).padStart(2, "0");
		return `/pages/page${s}.html`;
	}

	// populate one tile
	function fillTile(el, idx) {
		// reset tile (also clears any previously injected <style> in this tile)
		el.innerHTML = "";

		// background image
		el.style.backgroundImage = `url(${imgPath(idx)})`;
		el.style.backgroundSize = "cover";
		el.style.backgroundPosition = "center";

		// click navigates to target page
		el.onclick = (e) => {
			e.preventDefault();
			e.stopPropagation();
			window.location.href = linkPath(idx);
		};

		// PER-TILE HEADER (static, above text block)
		const header = document.createElement("div");
		header.className = "tile-header";

		// overlay defines bounds only (text area)
		const overlay = document.createElement("div");
		overlay.className = "tile-overlay";

		// inner scroll container (existing behavior)
		const scroll = document.createElement("div");
		scroll.className = "tile-text-scroll";

		// build DOM
		el.appendChild(header);
		overlay.appendChild(scroll);
		el.appendChild(overlay);

		// load per-tile HTML (header + body in ONE file)
		fetch(textPath(idx))
			.then(r => r.ok ? r.text() : "")
			.then(html => {
				const tmp = document.createElement("div");
				tmp.innerHTML = html;

				// IMPORTANT: inject <style> blocks into the LIVE DOM so they apply
				tmp.querySelectorAll("style").forEach(s => {
					el.appendChild(s);
				});

				// EXPECTED STRUCTURE INSIDE FILE:
				// <div class="header">...</div>
				// <div class="body">...</div>
				const h = tmp.querySelector(".header");
				const b = tmp.querySelector(".body");

				if (h) header.appendChild(h);
				if (b) scroll.appendChild(b);
			})
			.catch(() => {});
	}

	// render all three tiles based on center index
	function render() {
		fillTile(tiles.left,   wrap(centerIndex - 1));
		fillTile(tiles.center, wrap(centerIndex));
		fillTile(tiles.right,  wrap(centerIndex + 1));
	}

	// navigation handlers
	btnPrev.addEventListener("click", () => {
		centerIndex = wrap(centerIndex - 1);
		render();
	});

	btnNext.addEventListener("click", () => {
		centerIndex = wrap(centerIndex + 1);
		render();
	});

	// initial render
	render();
})();
