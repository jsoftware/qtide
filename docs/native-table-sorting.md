# Preserve table row state during sorting

Qt's QTableWidget sort moves cell items, selection and index widgets, but leaves
vertical header items in place. JQt also keeps coordinate-indexed arrays for
alignment, protection, cell type and editor pointers. Those arrays and widget
callback coordinates must follow the same row permutation as the items.

`Table::sortrows` routes native header clicks, `set sort`, and the legacy header
handler through one correction. Persistent model indexes identify the original
rows, including duplicate keys and missing key items. After Qt sorts, their new
rows determine the permutation of the side arrays, complete vertical header
items, row heights and hidden state. Embedded widgets retain their instances;
their object names are updated to the displayed coordinates. Cached record rows
are remapped and the hover cache is invalidated.

## Signals and invariants

Table and vertical-header signals are blocked while the sort and restoration
run, so JQt does not emit intermediate change/mark events. Model and selection
signals remain enabled because Qt uses them to maintain persistent indexes,
selection and index widgets. Signal blockers restore each object's previous
blocking state. The native `hdrclick` event is sent after restoration completes;
`headerclick event` continues to emit an event without reordering rows.

Debug assertions check widget dimensions, per-cell array lengths, valid
persistent indexes and a complete, unique destination-row permutation. The
existing shape-management paths establish these invariants. Coordinate caches
start at -1 until a cell event or command supplies a coordinate.

## Boundaries

- Native keys remain item text. Equal keys keep their relative order; missing
  key items remain last in both directions. An existing empty-string item is
  distinct from a missing item. Numeric/date/custom ordering is application work.
- Qt preserves item roles, flags, fonts, colors, tooltips and selection identity.
  This correction does not change tooltip delivery or timing.
- Persistent indexes identify rows during a sort; they are not permanent record
  IDs or an application-model permutation API. External models need stable IDs
  or application-managed sorting and repopulation in event mode.
- Block/range coordinates and `dat` (the last setdata input, not canonical row
  data) are not permuted. Column sizing/alignment and sort-arrow state remain
  column state. Selection readback keeps its existing bounding-box format.
- Additional work is O(rows*columns), with one side-array snapshot at a time and
  O(rows) temporary identity/header state. Large-table performance is unmeasured.

## Build and regression tests

The native QtTest target uses the production Table/Form sources and objects,
without a J engine. Build sequentially with one Qt SDK and configuration:

```sh
export JQTSLIM=1
unset JQTFAT JQTWEBKIT NO_OPENGL NO_WEBENGINE
cd lib
qmake6 lib.pro
make -j4
qmake6 -o Makefile.sort-tests table-sort-tests.pro
make -f Makefile.sort-tests -j4
cd ../main
qmake6 main.pro
make -j4
cd ..
QT_QPA_PLATFORM=offscreen bin/table-sort-tests/table-sort-tests
QT_QPA_PLATFORM=xcb xvfb-run -a bin/table-sort-tests/table-sort-tests
```

Use installed Qt base/private headers, Core5Compat, SVG, WebSockets, OpenGL and
QtTest development components. Xvfb/xauth are needed for the virtual X11 lane.
For native desktop lanes, use the actual desktop session's environment and set
`QT_QPA_PLATFORM=wayland` or `xcb`. xcb on a Wayland desktop is XWayland, not a
standalone Xorg session. QtTest events are synthetic, not real-pointer evidence.

For MSVC, use an x64 Native Tools prompt, add the matching Qt bin directory to
PATH, set `JQTSLIM=1`, use `qmake` instead of `qmake6` and `nmake` instead of make,
and run the native executable with `offscreen` and `windows` QPA platforms.
The main target's launcher is under `bin/win-x86_64/bin`; the library is under
`bin/win-x86_64/release`.

To exercise assertions, use a separate clean checkout and append
`-after CONFIG-=release CONFIG+=debug` to every qmake invocation. Do not mix
release and debug production objects. Qt private headers tie each build to its
matching Qt runtime modules.

Tests cover signal suppression/restoration, initialization, row permutations,
repeated sorts, item/header properties, duplicate/missing keys, metadata after
cell recreation and reshape, supported widgets and callback coordinates,
selection, dimensions/visibility, event-only mode, invalid columns and empty
tables. The closed native test form suppresses dispatch into an absent J engine;
it still exercises the real Table slots. The Windows Python regression separately
checks typed reads/writes and captures state in the actual J header handler:

```text
python -B tests/wd/run_tests.py --jqt-bin <private-built-runtime-bin> --stock-bin <stock-runtime-bin> --temp-root <private-session-directory>
```

For interactive J integration, stage the built launcher/library in a disposable
complete J runtime and load `tests/wd/native-sort-repro.ijs`. Repeated header
clicks must keep labels, IDs, values and controls together. Widget callbacks must
report displayed coordinates. Record backend and distinguish synthetic events,
programmatic J assertions and user-observed real-pointer behavior. Never replace
the binaries of an existing working J installation for this test.
