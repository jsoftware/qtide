# wd `table` extensions (r9 investigation branch)

Branch: `feature/jqt-table-controls-investigation` of the personal fork `gaetgodi/qtide`.
Source: `lib/wd/table.cpp`, `lib/wd/table.h`, `lib/wd/wd.cpp`.

All additions are **backward compatible**. A form that does not use any of the new commands, options or events behaves
exactly as before: columns stay in Qt's `Stretch` mode, the header is not clickable, no new events are raised, and
`set data` still resizes columns to contents. No existing command changed meaning.

Widths are **logical pixels** (Qt device-independent pixels), identical to `set colwidth`. They are not physical pixels,
so no DPI conversion is needed in J.

## Feature detection

```j
wd 'qwdcaps'
```

Returns a space-separated list of capability tokens (a JQt without the extensions raises *unrecognized command*, which a
caller may treat as "no capabilities"):

```
table.colsizing table.colminwidth table.colwidth.get table.colresize.event
table.headerclick table.hdrclick.event table.sortindicator
table.celltip table.tooltips table.hover.event table.viewwidth.get
table.sortindicator.auto table.celltip.get wd.childxywh.origin
```

`wd.childxywh.origin` is not a table feature: it marks a build whose `qchildxywh` returns true positions
(see `wd-qchildxywh-origin.md`).

## Column sizing

| Command | Meaning |
|---|---|
| `set <id> colsizing <policy> [stretchlast\|nostretchlast]` | `policy` is `stretch` (the default), `interactive` (user-draggable), `fixed` (not resizable) or `contents`. Applies to the columns of the current `set block` (default: all columns). |
| `set <id> colminwidth <w>` | Smallest width a section may be dragged to. |
| `set <id> colwidth <w...>` | Existing command; unchanged. Widths set this way never raise `colresize`. |
| `get <id> colwidth [<col>]` | All current widths (space separated) or one. |
| `get <id> viewwidth` | Width of the table viewport (logical px; excludes the vertical scroll bar) - the room available for columns. |
| `get <id> colsizing` | e.g. `interactive nostretchlast` (`mixed ...` if a block was given a different policy). |

Constructor option: `interactive` is shorthand for `set colsizing interactive nostretchlast`.

While the whole table is `interactive` or `fixed`, `set data` and in-cell edits **do not** resize columns to contents
(they would discard the widths the user chose). `set <id> resizecol` still fits columns to contents explicitly.

## Column resize event

`colresize` is raised once, when the user releases the mouse after dragging a section edge (or double-clicking it to
auto-fit). Changes made by the program (`set colwidth`, `set resizecol`, `set data`, layout) never raise it.

| Variable | Value |
|---|---|
| `<id>_hdrcol` | section that changed |
| `<id>_hdrwidth` | its new width |
| `<id>_colwidth` | all widths, as `get colwidth` |

Handler: `<id>_colresize`.

## Header click and sort indicator

| Command | Meaning |
|---|---|
| `set <id> headerclick off\|event\|native` | `off` (default): header inert. `event`: header clickable, sort indicator shown; a click raises `hdrclick`. `native`: as `event`, and the rows are also sorted by item text. |
| `set <id> sortindicator <col> [asc\|desc]` | Show the indicator on `col`. Does not reorder rows. |
| `set <id> sortindicator none` | Hide it. |
| `get <id> sortindicator` | `<col> <order>`; `-1 0` when none; order `0` ascending, `1` descending. |

`hdrclick` variables: `<id>_hdrcol` (column clicked) and `<id>_sortind` (`<col> <order>` after Qt has toggled the
indicator). Handler: `<id>_hdrclick`. In `event` mode the application sorts (numerically, by date, by any key) and
repopulates; the indicator is already correct. `native` is a convenience for plain text tables.

## Per-cell tooltips

| Command | Meaning |
|---|---|
| `set <id> celltip <row> <col> <text>` | Tooltip of one cell. Empty text clears it. |
| `set <id> tooltips <data>` | Bulk, using the same data shapes and `set block` rules as `set data` (one value for all cells, one per column of the block, or one per cell). |

Tooltips are stored on the cell item and shown by Qt's normal tooltip timing. `celltip` may be issued from a `hover`
handler to supply text lazily.

## Hover event

`set <id> hover 1` raises `hover` whenever the pointer moves to a different cell (and `-1 -1` when it leaves the cells).
Variable `<id>_cell` = `<row> <col>`. Handler: `<id>_hover`. `set <id> hover 0` turns it off.

## Independence between tables

Every setting and event above lives in the `Table` instance (no static or shared state); `<id>` selects the table and
events carry the originating table's own variables. Two tables on one form (or in two forms) have independent sizing,
indicators, hover state and events.

## Corrections made after the first GUI test pass

| Id | Original behaviour | Now |
|---|---|---|
| D1 | `headerclick event`/`native`: the indicator was never shown by a user click, and `get sortindicator` stayed `-1 0` although the `hdrclick` event reported `_sortind`. | A click toggles the order of the current sort column (or starts ascending on a new one), shows the indicator and then raises `hdrclick`; `get sortindicator` agrees with `_sortind`. `set sortindicator` still overrides. Capability `table.sortindicator.auto`. |
| D3 | `set colminwidth abc` and `set sortindicator abc` were silently read as 0. | Rejected with the usual `must specify` error; J negatives (`_1`) are still accepted. `set colwidth` (stock) is unchanged. |
| D4 | `set headerclick off` hid the sort arrow. | Arrow visibility follows the sort state, not clickability: a disabled header keeps showing the current sort. |

`get <id> celltip <row> <col>` returns a cell's tooltip text (capability `table.celltip.get`).

(The numbering skips D2: a defect in the stock `qchildxywh` command, which is not a table command, is documented and
committed separately in `docs/wd-qchildxywh-origin.md`.)

## Notes and limits

* Sorting by number or date is done by the application in `event` mode; `native` sorts by item text only.
* Native/stock sorting preserves row labels, per-cell metadata, widget callback coordinates and Qt selection identity.
  Missing key items stay at the end; duplicate keys are stable. See [sorting correction and tests](native-table-sorting.md).
* `hdrclick` needs the header to be visible (`set <id> hdr ...`).
* The sorting correction has been validated on Windows x64 (Qt 6.10.3/MSVC) and Linux x86-64
  (Ubuntu 26.04.1, Qt 6.10.2/GCC), including Wayland and xcb/XWayland. This does not establish
  compatibility for every extension or platform. Qt 6.4 and macOS validation remain outstanding.
