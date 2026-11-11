"""Synthetic JQt component regressions; see docs/windows-build-and-tests.md.
Adapted from the verified client component suite, without application dependencies.
"""
import os
from pathlib import Path
import subprocess
import sys
import tempfile
import time
import unittest

sys.dont_write_bytecode = True
import harness as h

R9 = os.environ.get('JQT_TEST_BIN')
STOCK = os.environ.get('JQT_TEST_STOCK_BIN')
# Offset from the computed section edge to the point where the header's resize grip is grabbed. With the original
# qchildxywh (defect D2: widget offset counted twice) this had to be calibrated to -8; with the corrected command it is 0.
GRIP = 0

FORM = r"""
LOG_z_=:TESTROOT_z_,'/events.log'
dat_z_=: 3 :'; (DEL,each ":each <"0 y),each DEL'
wd 'pc t;pn "wdtest";bin v'
wd 'cc a table 0 3 selectrows;cc b table 0 3 selectrows;pas 0 0;pshow'
wd 'pmove 40 40 900 560'
wd 'set a shape 20 3;set a hdr A1 A2 A3;set a protect 1'
wd 'set b shape 20 3;set b hdr B1 B2 B3;set b protect 1'
wd 'set a data ',dat i.60
wd 'set b data ',dat 100+i.60
note=: 3 :'(y,LF) fappend LOG'
t_a_hdrclick=: 3 :'note ''a hdrclick '',a_hdrcol,'' | '',a_sortind'
t_b_hdrclick=: 3 :'note ''b hdrclick '',b_hdrcol,'' | '',b_sortind'
t_a_colresize=: 3 :'note ''a colresize '',a_hdrcol,'' '',a_hdrwidth,'' | '',a_colwidth'
t_b_colresize=: 3 :'note ''b colresize '',b_hdrcol,'' '',b_hdrwidth,'' | '',b_colwidth'
t_a_hover=: 3 :'note ''a hover '',a_cell'
t_b_hover=: 3 :'note ''b hover '',b_cell'
"""

# The synthetic form drives the command channel in its own base locale using its timer.
OBSERVE = r"""
cmdpoll=: 3 :0
if. fexist TESTROOT,'/cmd.ijs' do.
 script=. TESTROOT,'/cmd.ijs'
 out=. ''
 try.
  RESULT_z_=: ''
  0!:0 <script
  out=. RESULT_z_
 catch.
  out=. 'J ERROR: ',13!:12''
 end.
 ferase script
 if. 32=3!:0 out do. out=. 8 u: out end.
 (out,'') fwrite TESTROOT,'/cmd.tmp'
 (TESTROOT,'/cmd.out') frename TESTROOT,'/cmd.tmp'
end.
''
)
sys_timer_z_=: cmdpoll_base_
wd 'timer 100'
"""


@unittest.skipUnless(os.name == 'nt' and R9 and (Path(R9) / 'jqt.exe').exists(), 'run run_tests.py with an enhanced Windows JQt runtime')
class WdTable(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.tmp = tempfile.TemporaryDirectory(prefix='jqt-wd-')
        cls.addClassCleanup(cls.tmp.cleanup)
        cls.base = Path(cls.tmp.name)
        (cls.base / 'cmd.ijs').unlink(missing_ok=True)
        wrapper = cls.base / 'app.ijs'
        wrapper.write_text(f"TESTROOT_z_=:'{cls.base.as_posix()}'\n" + FORM + OBSERVE, encoding='utf-8')
        cls.g = h.Gui(cls.base)
        cls.addClassCleanup(cls.g.stop)
        from harness import isolated_command
        env = h.clean_environment(Path(R9))
        cls.proc = subprocess.Popen(isolated_command(Path(R9), 'jqt.exe', wrapper, cls.base), env=env,
                                    stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL, creationflags=0x4000)
        cls.g.process = cls.proc
        cls.g.hwnd = cls.g.until(lambda: next((hw for hw, t, p in h.top_windows() if p == cls.proc.pid and t == 'wdtest'), None), 40, 'window')

    def j(self, code):
        return self.g.j("wd 'psel t'\nRESULT_z_=:" + code)

    def events(self):
        p = self.base / 'events.log'
        return p.read_text(encoding='utf-8').splitlines() if p.exists() else []

    def clear(self):
        (self.base / 'events.log').unlink(missing_ok=True)

    def settle(self):
        time.sleep(.5)

    # ------------------------------------------------------------------ helpers over the live form
    def nums(self, code):
        return [int(float(x.replace('_', '-'))) for x in self.j(code).split()]

    def attempt(self, code):
        """(ok, text): the J result, or the J error text, without failing the test."""
        try:
            return True, self.j(code)
        except AssertionError as e:
            if not str(e).startswith('J ERROR:'):
                raise  # timeouts/dead children are infrastructure failures, not wd rejection
            return False, str(e).split('\n')[0]

    def geom(self, tid):
        """x y w h of a table in the form's client area, exactly as `qchildxywh` reports it (D2 fixed in this JQt)."""
        return self.nums(f"wd 'qchildxywh {tid}'")

    def widths(self, tid):
        return self.nums(f"wd 'get {tid} colwidth'")

    def reset(self):
        """Back to a known state for every test: stretch columns, no sorting, no hover, original data."""
        self.j("wd 'set a colsizing stretch stretchlast;set b colsizing stretch stretchlast;set a headerclick off;"
               "set b headerclick off;set a sortindicator none;set b sortindicator none;set a hover 0;set b hover 0;set a colminwidth 0;set b colminwidth 0;"
               "set a shape 20 3;set b shape 20 3;set a data ',(dat i.60),';set b data ',(dat 100+i.60)\n''")
        self.clear()
        self.settle()

    def setUp(self):
        self.assertIsNone(self.proc.poll(), 'jqt.exe died during an earlier test')
        self.reset()

    def post_click(self, tid, col, dy=8):
        x, y, w_, h_ = self.geom(tid)
        ws = self.widths(tid)
        h.click(self.g.hwnd, x + 1 + sum(ws[:col]) + ws[col] // 2, y + 1 + dy)

    def post_drag_border(self, tid, col, delta):
        x, y, w_, h_ = self.geom(tid)
        ws = self.widths(tid)
        edge = x + 1 + sum(ws[:col + 1]) + GRIP   # GRIP: calibrated, see below
        h.drag(self.g.hwnd, edge, y + 1 + 8, edge + delta)

    def sweep(self, tid, points, hold=True):
        """Press inside the table and move through `points` (relative to the table widget) with the left button HELD, then
        release. Qt on Windows ignores a bare posted WM_MOUSEMOVE (no button), so this is the only pointer path available
        without moving the real cursor. It exercises the real MouseMove -> hover path of the viewport."""
        x, y, *_ = self.geom(tid)
        U, hw = h.u.PostMessageW, self.g.hwnd
        pos = lambda rx, ry: ((y + ry) << 16) | ((x + rx) & 0xFFFF)
        r0 = points[0]
        U(hw, h.WM_MOUSEMOVE, 0, pos(*r0))
        U(hw, h.WM_LBUTTONDOWN, 1, pos(*r0))
        for rx, ry in points[1:]:
            U(hw, h.WM_MOUSEMOVE, 1, pos(rx, ry))
            time.sleep(.08)
        U(hw, h.WM_LBUTTONUP, 0, pos(*points[-1]))
        time.sleep(.4)

    # =========================================================================================== capability
    def test_01_capabilities_are_discoverable(self):
        want = set('table.colsizing table.colminwidth table.colwidth.get table.colresize.event table.headerclick '
                   'table.hdrclick.event table.sortindicator table.celltip table.tooltips table.hover.event '
                   'table.viewwidth.get table.sortindicator.auto wd.childxywh.origin table.celltip.get'.split())
        self.assertEqual(set(self.j("wd 'qwdcaps'").split()), want)

    def test_02_stock_runtime_reports_no_capabilities(self):
        if not STOCK:
            self.skipTest('no stock slim runtime')
        # a stock JQt must answer 'unrecognized command' to qwdcaps (the documented "no capabilities" signal)
        import harness as crp
        base = self.base / 'stock'
        base.mkdir(exist_ok=True)
        script = base / 'probe.ijs'
        out = (base / 'out.txt').as_posix()
        script.write_text("""probe=: 3 :0
try.
 r=. wd 'qwdcaps'
catch.
 r=. 'NOCAPS'
end.
r fwrite '""" + out + """'
)
probe ''
exit 0
""", encoding='utf-8')
        env = h.clean_environment(Path(STOCK))
        p = subprocess.Popen(crp.isolated_command(STOCK, 'jqt.exe', script, base), env=env, stdout=subprocess.DEVNULL,
                             stderr=subprocess.DEVNULL, creationflags=0x4000)
        try:
            self.g.until(lambda: (base / 'out.txt').exists(), 40, 'stock probe produced nothing')
            time.sleep(.3)
            self.assertEqual((base / 'out.txt').read_text(), 'NOCAPS')
            self.assertEqual(p.wait(timeout=10), 0, 'stock probe must exit normally')
        finally:
            # This probe has no application window/event loop to close. Reap only
            # the subprocess we created, and verify shutdown before temp cleanup.
            if p.poll() is None:
                p.terminate()
            try:
                p.wait(timeout=5)
            except subprocess.TimeoutExpired:
                p.kill()
                p.wait(timeout=5)

    # =========================================================================================== independence / widths
    def test_03_two_tables_have_independent_widths_and_policies(self):
        b0 = self.widths('b')
        self.j("wd 'set a colsizing interactive nostretchlast;set a colwidth 50 60 70'\n''")
        self.assertEqual(self.widths('a'), [50, 60, 70])
        self.assertEqual(self.widths('b'), b0, 'b must not follow a')
        self.assertEqual(self.j("wd 'get a colsizing'"), 'interactive nostretchlast')
        self.assertEqual(self.j("wd 'get b colsizing'"), 'stretch stretchlast')
        self.j("wd 'set b colsizing fixed nostretchlast;set b colwidth 90 40 30'\n''")
        self.assertEqual(self.widths('a'), [50, 60, 70])
        self.assertEqual(self.widths('b'), [90, 40, 30])
        self.assertEqual(self.events(), [], 'programmatic width changes must not raise colresize')

    def test_04_per_column_widths_and_minimum_width_on_user_drag(self):
        self.j("wd 'set a colsizing interactive nostretchlast;set a colminwidth 45;set a colwidth 100 100 100'\n''")
        self.post_drag_border('a', 0, -90)                  # try to shrink col 0 far below the minimum
        self.g.until(lambda: self.widths('a')[0] != 100, 8, 'drag had no effect')
        self.settle()
        self.assertEqual(self.widths('a')[0], 45, 'the minimum width must clamp a user drag')
        self.assertEqual(self.widths('a')[1:], [100, 100])

    def test_05_user_resize_raises_one_event_with_correct_values_only_for_that_table(self):
        self.j("wd 'set a colsizing interactive nostretchlast;set a colwidth 80 80 80;set b colsizing interactive nostretchlast'\n''")
        before_b = self.widths('b')
        self.post_drag_border('a', 1, 40)
        self.g.until(lambda: self.events(), 8, 'no colresize event')
        self.settle()
        ev = self.events()
        self.assertEqual(len(ev), 1, ev)
        self.assertRegex(ev[0], r'^a colresize 1 120 \| 80 120 80$')
        self.assertEqual(self.widths('a'), [80, 120, 80])
        self.assertEqual(self.widths('b'), before_b)

    def test_06_widths_persist_across_data_updates_in_interactive_mode(self):
        self.j("wd 'set a colsizing interactive nostretchlast;set a colwidth 33 44 55'\n''")
        for i in range(5):
            self.j(f"wd 'set a data ',dat {i}+i.60\n''")
        self.assertEqual(self.widths('a'), [33, 44, 55])
        self.j("wd 'set a shape 5 3;set a data ',dat i.15\n''")
        self.assertEqual(self.widths('a'), [33, 44, 55], 'a shape change must not discard user widths')

    def test_07_default_stretch_mode_is_unchanged_from_stock_behaviour(self):
        # documented backward compatibility: no new command used => stretch columns, header inert, no events
        self.assertEqual(self.j("wd 'get a colsizing'"), 'stretch stretchlast')
        self.assertEqual(self.j("wd 'get a sortindicator'"), '-1 0')
        self.post_click('a', 1)
        self.settle()
        self.assertEqual(self.events(), [], 'header must be inert until headerclick is enabled')

    # =========================================================================================== header click / sorting
    def test_08_header_click_event_reports_column_and_toggling_order(self):
        self.j("wd 'set a headerclick event'\n''")
        for col in (1, 1, 2):
            n = len(self.events())
            self.post_click('a', col)
            self.g.until(lambda: len(self.events()) > n, 8, 'click missing')
            time.sleep(.7)                             # longer than the system double-click interval
        self.settle()
        self.assertEqual(self.events(), ['a hdrclick 1 | 1 0', 'a hdrclick 1 | 1 1', 'a hdrclick 2 | 2 0'])
        self.assertEqual(self.j("wd 'get b sortindicator'"), '-1 0', 'b indicator must be independent')

    def test_08b_D1_indicator_follows_user_clicks_and_agrees_with_the_event(self):
        """D1 (fixed): the documented contract is that in event/native mode the indicator is shown and already correct when
        hdrclick is raised. Originally `get sortindicator` stayed '-1 0' after clicks (indicator never shown)."""
        self.j("wd 'set a headerclick event'\n''")
        expect = [(2, '2 0'), (2, '2 1'), (2, '2 0'), (0, '0 0')]
        for i, (col, want) in enumerate(expect):
            self.post_click('a', col)
            self.g.until(lambda: len(self.events()) > i, 8, 'no event')
            self.settle()
            self.assertEqual(self.j("wd 'get a sortindicator'"), want, f'after click {i}')
            self.assertTrue(self.events()[-1].endswith('| ' + want), self.events()[-1])
            time.sleep(.6)                          # past the double-click interval
        self.assertEqual(self.j("wd 'get b sortindicator'"), '-1 0')

    def test_08c_D1_application_can_still_override_the_indicator(self):
        self.j("wd 'set a headerclick event'\n''")
        self.post_click('a', 1)
        self.g.until(lambda: self.events(), 8, 'no event')
        self.settle()
        self.j("wd 'set a sortindicator 2 desc'\n''")
        self.assertEqual(self.j("wd 'get a sortindicator'"), '2 1')
        time.sleep(.6)
        self.post_click('a', 2)                     # the next click toggles from the application's state
        self.g.until(lambda: len(self.events()) > 1, 8, 'no second event')
        self.settle()
        self.assertEqual(self.j("wd 'get a sortindicator'"), '2 0')

    def test_08d_D1_native_mode_sorts_and_shows_the_indicator(self):
        self.j("wd 'set a headerclick native'\n''")
        self.post_click('a', 0)
        self.g.until(lambda: self.events(), 8, 'no event')
        self.settle()
        self.assertEqual(self.j("wd 'get a sortindicator'"), '0 0')
        col0 = [self.j(f"wd 'get a cell {r} 0'") for r in range(20)]
        self.assertEqual(col0, sorted(col0))
        time.sleep(.6)
        self.post_click('a', 0)
        self.g.until(lambda: len(self.events()) > 1, 8, 'no second event')
        self.settle()
        self.assertEqual(self.j("wd 'get a sortindicator'"), '0 1')
        col0 = [self.j(f"wd 'get a cell {r} 0'") for r in range(20)]
        self.assertEqual(col0, sorted(col0, reverse=True))

    def test_08f_D4_disabling_the_header_keeps_the_current_indicator_visible(self):
        """D4 (fixed): `headerclick off` hid the arrow (a client may temporarily disable header interaction)."""
        self.j("wd 'set a headerclick event;set a sortindicator 1 desc'\n''")
        self.j("wd 'set a headerclick off'\n''")
        self.assertEqual(self.j("wd 'get a sortindicator'"), '1 1')
        self.post_click('a', 2)
        self.settle()
        self.assertEqual(self.events(), [], 'an off header is inert')
        self.assertEqual(self.j("wd 'get a sortindicator'"), '1 1')
        self.j("wd 'set a headerclick event'\n''")
        self.assertEqual(self.j("wd 'get a sortindicator'"), '1 1')

    def test_08e_D2_child_geometry_is_the_true_position_and_tables_do_not_overlap(self):
        """D2 (fixed): qchildxywh added the widget's own offset twice (second child at ~2x its y, first at 18 not 9)."""
        ax, ay, aw, ah = self.geom('a')
        bx, by, bw, bh = self.geom('b')
        self.assertEqual((ax, ay), (9, 9), 'layout margin is 9 px; the first child sits at the margin')
        self.assertEqual(bx, ax)
        self.assertGreaterEqual(by, ay + ah, 'b starts below a')
        self.assertLessEqual(by - (ay + ah), 12, 'only the layout spacing separates them')
        client_h = h.rect(self.g.hwnd)[3] - h.rect(self.g.hwnd)[1] - 31
        self.assertLessEqual(by + bh, client_h + 40, 'b lies inside the form')

    def test_09_event_isolation_between_tables(self):
        self.j("wd 'set a headerclick event;set b headerclick event'\n''")
        self.post_click('b', 0)
        self.g.until(lambda: self.events(), 8, 'no event')
        self.settle()
        self.assertEqual(self.events(), ['b hdrclick 0 | 0 0'])
        self.assertEqual(self.j("wd 'get a sortindicator'"), '-1 0')

    def test_10_programmatic_sortindicator_does_not_reorder_or_raise(self):
        self.j("wd 'set a sortindicator 2 desc'\n''")
        self.assertEqual(self.j("wd 'get a sortindicator'"), '2 1')
        self.assertEqual(self.j("wd 'get a cell 0 0'"), '0', 'rows must not move')
        self.j("wd 'set a sortindicator none'\n''")
        self.assertEqual(self.j("wd 'get a sortindicator'"), '-1 0')
        self.assertEqual(self.events(), [])

    def test_11_native_sort_is_textual_and_rows_keep_their_identity(self):
        self.j("wd 'set a headerclick native'\n''")
        self.post_click('a', 0)
        self.g.until(lambda: self.events(), 8, 'no hdrclick event')
        self.settle()
        col0 = [self.j(f"wd 'get a cell {r} 0'") for r in range(20)]
        self.assertEqual(col0, sorted(col0), 'native sort is by item text')
        self.assertNotEqual(col0, [str(3 * r) for r in range(20)], 'rows were not reordered')
        for r in range(20):                       # each row's other cells travelled with its first cell
            v = int(col0[r])
            self.assertEqual([self.j(f"wd 'get a cell {r} {c}'") for c in (1, 2)], [str(v + 1), str(v + 2)], f'row {r}')
        self.assertEqual([self.j(f"wd 'get b cell {r} 0'") for r in (0, 1, 2)], ['100', '103', '106'], 'b untouched')

    def test_12_application_sort_in_event_mode_keeps_indicator_and_widths(self):
        """A client-side sorting workflow: on hdrclick it sorts, repopulates and sets the indicator itself."""
        self.j("wd 'set a headerclick event;set a colsizing interactive nostretchlast;set a colwidth 61 62 63'\n''")
        self.post_click('a', 0)
        self.g.until(lambda: self.events(), 8, 'no event')
        self.settle()
        self.j("wd 'set a data ',dat |.i.60\nwd 'set a sortindicator 0 desc'\n''")
        self.assertEqual(self.j("wd 'get a sortindicator'"), '0 1')
        self.assertEqual(self.widths('a'), [61, 62, 63], 'widths survive repopulation')
        self.assertEqual(self.j("wd 'get a cell 0 0'"), '59')
        self.j("wd 'set a data ',dat i.60\n''")
        self.assertEqual(self.j("wd 'get a sortindicator'"), '0 1', 'indicator survives a second repopulation')

    def test_12b_native_sort_preserves_tips_and_typed_cell_reads(self):
        # Heterogeneous per-row metadata used to stay behind after Qt sorted.
        self.j("wd 'set a shape 0 3;set a shape 3 3;set a hdr Key ID Value;"
               "set a data B idB valueB A idA valueA C idC valueC;"
               "set a block 0 2;set a type 100;set a cell 0 2 1;"
               "set a block 1 2;set a type 200;set a cell 1 2 \"0 red green\";"
               "set a block;set a celltip 0 1 tipB;set a celltip 1 1 tipA;"
               "set a headerclick native'\n''")
        # Snapshot in the actual J handler, before a later command-channel read.
        self.j("''\nt_a_hdrclick=: 3 :0\n"
               "sortids_z_=: (wd 'get a cell 0 1'),'|',(wd 'get a cell 1 1'),'|',wd 'get a cell 2 1'\n"
               "sorttip_z_=: wd 'get a celltip 1 1'\n"
               "sortvalue_z_=: wd 'get a cell 0 2'\n"
               "note 'a hdrclick ',a_hdrcol,' | ',a_sortind\n)\n''")
        try:
            for ids, values, tips in ((['idA', 'idB', 'idC'], ['red', '1', 'valueC'], ['tipA', 'tipB', '']),
                                      (['idC', 'idB', 'idA'], ['valueC', '1', 'red'], ['', 'tipB', 'tipA']),
                                      (['idA', 'idB', 'idC'], ['red', '1', 'valueC'], ['tipA', 'tipB', ''])):
                self.clear()
                self.post_click('a', 0)
                self.g.until(lambda: self.events(), 8, 'native sort event')
                self.settle()
                self.assertEqual(self.j('sortids_z_'), '|'.join(ids))
                self.assertEqual(self.j('sorttip_z_'), 'tipB')
                self.assertEqual(self.j('sortvalue_z_'), values[0])
                self.assertEqual([self.j(f"wd 'get a cell {r} 1'") for r in range(3)], ids)
                self.assertEqual([self.j(f"wd 'get a cell {r} 2'") for r in range(3)], values)
                self.assertEqual([self.j(f"wd 'get a celltip {r} 1'") for r in range(3)], tips)
            # The displayed checkbox and combo must still support typed writes.
            self.j("wd 'set a cell 1 2 0;set a cell 0 2 1'\n''")
            self.assertEqual(self.j("wd 'get a cell 1 2'"), '0')
            self.assertEqual(self.j("wd 'get a cell 0 2'"), 'green')
        finally:
            self.j("''\nt_a_hdrclick=: 3 :'note ''a hdrclick '',a_hdrcol,'' | '',a_sortind'\n''")
            self.j("wd 'set a shape 0 3;set a shape 20 3;set a hdr A1 A2 A3;set a protect 1'\n''")

    # =========================================================================================== tooltips / hover
    def hover_events(self, tid):
        return [e.split(' ', 2)[2] for e in self.events() if e.startswith(f'{tid} hover ')]

    def test_13_hover_events_report_cells_and_stay_isolated(self):
        self.j("wd 'set a hover 1'\n''")                    # b stays off
        self.sweep('a', [(20, 30), (20, 50), (20, 70), (60, 70), (20, 90), (700, 90)])
        got = self.hover_events('a')
        cells = [tuple(int(v.replace('_', '-')) for v in g_.split()) for g_ in got]
        self.assertGreaterEqual(len(cells), 3, got)
        self.assertEqual(cells[-1], (-1, -1), 'leaving the cells reports -1 -1')
        valid = [c for c in cells if c != (-1, -1)]
        self.assertTrue(all(0 <= r < 20 and 0 <= c < 3 for r, c in valid), cells)
        self.assertTrue(all(a != b for a, b in zip(cells, cells[1:])), 'unchanged cells must not raise duplicate hover events')
        self.assertEqual(self.hover_events('b'), [], 'b must not report a movement')
        self.clear()
        self.sweep('b', [(20, 30), (20, 60), (20, 90)])      # b has hover off: silent
        self.assertEqual(self.events(), [])

    def test_13b_hover_can_be_enabled_per_table_and_turned_off(self):
        self.j("wd 'set a hover 1;set b hover 1'\n''")
        self.sweep('b', [(20, 30), (20, 60), (20, 90)])
        self.assertTrue(self.hover_events('b'))
        self.assertEqual(self.hover_events('a'), [])
        self.j("wd 'set b hover 0'\n''")
        self.clear()
        self.sweep('b', [(20, 30), (20, 60), (20, 90)])
        self.assertEqual(self.events(), [])

    def test_14_tooltips_accept_newlines_unicode_clearing_and_bulk(self):
        self.j("wd 'set a celltip 1 1 ',('first line',LF,'second line')\n''")
        self.j("wd 'set a celltip 2 0 ',8 u: 'Ünïcode 漢字 😀'\n''")
        self.j("wd 'set a celltip 3 2 '\n''")      # empty text clears
        self.j("wd 'set b tooltips ',; DEL,each (<'bulk tip'),each DEL\n''")
        self.assertIsNone(self.proc.poll())
        self.assertEqual(self.j("wd 'get a cell 1 1'"), '4', 'cell text is untouched by tooltips')

    def test_14b_celltip_readback_round_trips_exact_text(self):
        """`get celltip` lets the tooltip TEXT be verified without a real pointer (the popup itself still needs one)."""
        multi = 'line one\nline two\n\ttabbed'
        self.j("wd 'set a celltip 1 1 ',('line one',LF,'line two',LF,TAB,'tabbed')\n''")
        self.assertEqual(self.j("wd 'get a celltip 1 1'"), multi)
        self.j("wd 'set a celltip 2 0 ',8 u: 'Ünïcode 漢字 😀'\n''")
        self.assertEqual(self.j("wd 'get a celltip 2 0'"), 'Ünïcode 漢字 😀')
        self.assertEqual(self.j("wd 'get a celltip 0 0'"), '', 'no tip set')
        self.j("wd 'set a celltip 1 1 '\n''")
        self.assertEqual(self.j("wd 'get a celltip 1 1'"), '', 'cleared')
        self.j("wd 'set b tooltips ',; DEL,each (<'bulk tip'),each DEL\n''")
        self.assertEqual(self.j("wd 'get b celltip 19 2'"), 'bulk tip')
        self.assertEqual(self.j("wd 'get a celltip 2 0'"), 'Ünïcode 漢字 😀', 'a unaffected by b')
        for code in ("wd 'get a celltip 99 0'", "wd 'get a celltip x y'", "wd 'get a celltip 1'"):
            self.assertFalse(self.attempt(code)[0], code)

    @unittest.skip('BLOCKED: Qt shows tooltips only for a real pointer (no posted-message path), and this harness never moves the '
                   'real cursor. The tooltip TEXT is verified by test_14b; the popup itself needs a manual check.')
    def test_15_tooltip_popup_appears_on_real_hover(self):
        pass

    # =========================================================================================== edge cases
    def test_16_empty_tables(self):
        self.j("wd 'set a shape 0 3'\n''")
        self.j("wd 'set a colsizing interactive nostretchlast;set a colwidth 70 70 70;set a headerclick event'\n''")
        self.assertEqual(self.widths('a'), [70, 70, 70])
        self.post_click('a', 1)
        self.g.until(lambda: self.events(), 8, 'header click on an empty table must still raise hdrclick')
        self.attempt("wd 'set a celltip 0 0 x'")   # out of range on an empty table: must not crash
        self.assertIsNone(self.proc.poll())
        self.assertEqual(self.events(), ['a hdrclick 1 | 1 0'])

    def test_17_repeated_updates_are_stable(self):
        self.j("wd 'set a colsizing interactive nostretchlast;set a colwidth 41 42 43'\n''")
        t0 = time.perf_counter()
        for i in range(100):
            self.j(f"wd 'set a data ',dat {i}+i.60\nwd 'set a celltip 1 1 tip{i}'\n''")
        elapsed = time.perf_counter() - t0
        self.assertEqual(self.widths('a'), [41, 42, 43])
        self.assertIsNone(self.proc.poll())
        print(f'\n[perf] 100 data+celltip updates via the command channel: {elapsed:.1f}s ({elapsed / 100 * 1000:.0f} ms each, includes the 100 ms poll)')

    def test_18_unicode_cell_text_round_trips(self):
        self.j("wd 'set a cell 0 0 ',8 u: 'é漢字😀'\n''")
        self.assertEqual(self.j("wd 'get a cell 0 0'"), 'é漢字😀')

    def test_19_invalid_parameters_never_crash(self):
        bad = ["wd 'set a colsizing bogus'", "wd 'set a colminwidth abc'", "wd 'set a colminwidth _5'", "wd 'set a colwidth x y z'",
               "wd 'set a colwidth 10 20'", "wd 'set a colwidth 10 20 30 40 50'", "wd 'set a celltip 99 99 x'",
               "wd 'set a celltip _1 _1 x'", "wd 'set a sortindicator 99'", "wd 'set a sortindicator 1 sideways'",
               "wd 'set a headerclick bogus'", "wd 'set a hover maybe'", "wd 'get a colwidth 99'", "wd 'get a bogusquery'",
               "wd 'set nosuch colsizing interactive'", "wd 'set a tooltips'"]
        print('\n[invalid-parameter behaviour]')
        for code in bad:
            ok, text = self.attempt(code)
            print(f"  {code:52s} {'accepted' if ok else 'error: ' + text[:60]}")
            self.assertIsNone(self.proc.poll(), 'process died on: ' + code)
        self.assertGreaterEqual(self.j("wd 'qwdcaps'").count('table.'), 11, 'still responsive after invalid input')

    def test_20_D3_non_numeric_arguments_of_new_commands_are_rejected(self):
        """D3 (fixed): `colminwidth abc` and `sortindicator abc` were silently read as 0. `set colwidth` is a stock command
        and keeps its historical leniency (documented, not changed)."""
        for code in ("wd 'set a colminwidth abc'", "wd 'set a colminwidth 1x'", "wd 'set a colminwidth '",
                     "wd 'set a sortindicator abc'", "wd 'set a sortindicator 1x asc'"):
            ok, text = self.attempt(code)
            self.assertFalse(ok, f'{code} must be rejected')
            self.assertIn('must specify', text)
        self.assertEqual(self.j("wd 'get a sortindicator'"), '-1 0', 'a rejected command changes nothing')
        for code in ("wd 'set a colminwidth 0'", "wd 'set a colminwidth 33'", "wd 'set a sortindicator 2 desc'",
                     "wd 'set a sortindicator none'", "wd 'set a sortindicator _1'"):
            self.assertTrue(self.attempt(code)[0], f'{code} must still be accepted')
