# Enhanced JQt contribution review

Prepared on 2026-11-09; feature-branch publication authorized by the owner.
This contribution is offered for community review; upstream acceptance is not claimed.
Public destination: <https://github.com/gaetgodi/qtide>.

## Preserved source history

Upstream `jsoftware/qtide` master and the public fork's master/feature refs were inspected at
`58f33c399f45bb092291619f9a02529a3b6ae2b9`. These three local commits descend from that base;
they were not present on the inspected public remote branches:

1. `22bda66fad19bc0a8df9d4b31a668d542c34e4d1`: table extension implementation and API documentation.
   Adds capability discovery; per-table/per-column sizing, minimum width and width readback;
   user resize/header events; configurable header sorting/indicators; cell/bulk tooltip text;
   hover notifications and view-width readback. Changes `lib/wd/table.cpp`, `table.h`, `wd.cpp`
   and adds `docs/wd-table-extensions.md`. Default stretch sizing, inert headers and disabled
   new events preserve the legacy behavior until a caller opts in.
2. `45699d933f0fcfc0842063d72c2a809b60de9bb7`: table follow-ups D1/D3/D4, cell-tip getter and docs.
   Keeps native sort/header/indicator state consistent, rejects malformed numeric arguments
   for the new commands, and preserves the indicator when header interaction is disabled.
   Changes `table.cpp`, `table.h` and the table API documentation.
3. `22a0499b4165a5f0c00f3a5a65fc1ddb1b0d2761`: separate D2 correction in `lib/wd/wd.cpp` and
   `docs/wd-qchildxywh-origin.md`. Maps the child origin into the window's coordinate system
   instead of counting the widget offset twice. Adds capability `wd.childxywh.origin`.
   This corrects an existing command; callers compensating for the old error must remove
   their workaround. It deserves independent upstream review despite the additive table API.

The source delta is five files, 656 insertions and six deletions. This contribution preparation
changes none of those C++ implementations and preserves all three commits and their order.

## Standalone tests and boundaries

`tests/wd/test_table.py` adapts the reusable synthetic two-table component suite validated by
DiskCatalogue. `harness.py` supplies only generic Windows process/window and isolated-J-profile
support. The synthetic form and data are embedded in the test, so there are no external fixtures.
The stock-runtime probe must finish normally; component script errors fail rather than counting
as expected feature rejection. Profile isolation has dedicated negative regressions.

Application-specific scanning, catalogue data, Save As/native picker, packaging/launcher,
F1 Help and application-window ownership integration tests remain in their application project.
They are not reusable JQt component fixtures and are not copied. No application code, private
repository URLs, owner data, binary dependencies or workstation-specific discovery is needed.
The original test implementations remain available to their owner in their original repository.

See [Windows build/test instructions](windows-build-and-tests.md) and
[exact r9 runtime fingerprints](enhanced-runtime-provenance.json).

## GitHub branch layout

Retain `master` at its existing upstream base. Publish
`feature/jqt-table-controls-investigation` with the three original commits, followed by a
separate reviewed commit for these tests/build instructions/provenance documents. This gives
Chris a single branch to clone and exact commit links without rewriting accepted source history.
The r9 runtime remains pinned to source `22a0499...`, even when documentation/tests later move
branch HEAD. An upstream reviewer can assess the table commits and coordinate fix separately.
The contribution preserves that history and adds a separate tests/documentation commit.
No merge into `master` or pull request is part of this publication.

The source-only content review found no specific licensing prohibition or material disclosure
problem. Existing Jsoftware, Qt and third-party notices are retained without alteration.
The Qt LGPL notice, embedded J-source GPLv3 notices and MIT component notices retain their
existing scopes; no blanket relicensing of upstream code is asserted. Qt/MSVC/J runtime redistribution
requires its own review; this source contribution includes none of their binaries. Public source
access will not by itself authorize distribution of a bundled runtime or complete application.
