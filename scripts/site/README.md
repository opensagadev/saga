# Site templates

`home.html` is the homepage. `head.html`, `header.html`, and `footer.html` are shared by the
homepage, progress explorer, player, and unlisted `/unpack/` page. `head.html` owns the favicon and stylesheet links.
The Python generator fills their markers and copies the
styles into the generated site. Internal links are relative so custom domains
and GitHub project URLs both work.

`unpack.html` and `unpack.js` are the unpacker's two files. Its styles are
inline in the HTML; the JavaScript includes the nudat WASM decoder and also runs
as the decoder and download workers. The page has no navigation link and asks
crawlers not to index it; anyone with its URL can still open it.

All four pages use the same compiled Tailwind CSS 4.3.3 stylesheet. With Node.js 24 and npm installed:

```sh
npm --prefix scripts/site ci
npm --prefix scripts/site run build
bazel run //scripts:plot_binary_match_map
```

The shared colors, typography, layouts, and controls live in `../site.css`;
`header.css` contains the navigation styles. `input.css` imports both and scans
all three page templates, including the explorer's dynamically created controls.

Each page's main content uses `site-container site-content`. Change `.site-content`
in `../site.css` to adjust the space below the navigation on all pages; do not add
page-specific top padding. Shared headings use `.site-title` and `.site-subtitle`,
intro paragraphs use `.site-intro`, and divided sections use `.site-section`.
The FAQ's typography and spacing are grouped under `.site-faq` in the same file.

Commit the generated `site/site.css` (relative to `scripts/`) with template or
style changes. CI rebuilds it and checks for differences. Keeping the compiled CSS in the
repository lets the existing Python and Bazel generators work without Node.js.
For CSS development, `npm --prefix scripts/site run watch` rebuilds on changes;
rerun the site generator to copy the output into `doc/pages/`.

The homepage displays `measures.fuzzy_match_percent` from `matching.json` as
"Decompilation progress". Only the progress explorer receives the full report.
The player owns the WASM runtime and isolation service worker.

All pages follow the system light/dark preference, including changes while the
page is open. The progress explorer redraws its canvases when that preference
changes; SVG labels and ordinary controls update through CSS.

## Assets

- `assets/favicon.svg`: the logo's stud stack centered in a square, with a
  transparent background. `assets/favicon.png` is the 32px fallback. The generator
  copies both to the site root. To regenerate the PNG with librsvg:
  `rsvg-convert -w 32 -h 32 -o scripts/site/assets/favicon.png scripts/site/assets/favicon.svg`.
- `assets/logo.svg`: the approved opensaga wordmark and four-stud stack,
  cropped for the header. Stud proportions reference James Jessiman's
  [LDraw 6141](https://library.ldraw.org/parts/10969), licensed under
  [CC BY 4.0](https://creativecommons.org/licenses/by/4.0/); the drawing is
  simplified and the wordmark uses `currentColor` for light and dark pages.
- `assets/discord.svg` and `assets/github.svg`: from
  [Simple Icons 16.31.0](https://github.com/simple-icons/simple-icons/tree/16.31.0),
  distributed under [CC0](https://github.com/simple-icons/simple-icons/blob/16.31.0/LICENSE.md).
