"""Isolation failures must fail closed rather than executing user startup code."""
from pathlib import Path
import tempfile
import unittest
import harness


class ProfileIsolation(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory(prefix='jqt-profile-')
        self.addCleanup(self.tmp.cleanup)
        self.root = Path(self.tmp.name)
        self.bin = self.root/'runtime/bin'
        self.bin.mkdir(parents=True)
        (self.bin.parent/'system/util').mkdir(parents=True)
        self.profile = self.bin/'profile.ijs'
        self.boot = self.bin.parent/'system/util/boot.ijs'
        self.profile.write_text("home=. >(systype-5){(2!:5'HOME');2!:5'USERPROFILE'\n"
                                "0!:0 <jpathsep system_:,'/util/boot.ijs'\n"
                                "0!:0 <home,'/profilex.ijs' NB. override\n", encoding='utf-8')
        self.boot.write_text("f=. jpath '~config/startup.ijs'\n", encoding='utf-8')
        self.session = self.root/'session'
        self.session.mkdir()

    def command(self):
        return harness.isolated_command(self.bin, 'jqt.exe', self.root/'synthetic.ijs', self.session)

    def test_private_profile_disables_user_hooks_without_editing_runtime(self):
        originals = (self.profile.read_bytes(), self.boot.read_bytes())
        command = self.command()
        profile = (self.session/'profile.ijs').read_text(encoding='utf-8')
        self.assertIn(harness.jq(self.session/'home'), profile)
        self.assertNotIn("0!:0 <home,'/profilex.ijs'", profile)
        self.assertNotIn("jpath '~config/startup.ijs'", (self.session/'boot.ijs').read_text())
        self.assertEqual((self.profile.read_bytes(), self.boot.read_bytes()), originals)
        self.assertEqual(command[1], '-jprofile')
        self.assertEqual(Path(command[2]), self.session/'profile.ijs')

    def test_unknown_boot_layout_is_rejected(self):
        self.boot.write_text('unrecognized startup sequence', encoding='utf-8')
        with self.assertRaisesRegex(RuntimeError, 'isolation cannot be guaranteed'):
            self.command()

    def test_duplicate_profile_hook_is_rejected(self):
        self.profile.write_text(self.profile.read_text()*2, encoding='utf-8')
        with self.assertRaises(RuntimeError):
            self.command()

    def test_j_errors_are_failures(self):
        class Process:
            def poll(self):
                return None
        gui = harness.Gui(self.root)
        gui.process = Process()
        def deliver(predicate, *args):
            (self.root/'cmd.out').write_text('J ERROR: synthetic failure', encoding='utf-8')
            return predicate()
        gui.until = deliver
        with self.assertRaisesRegex(AssertionError, 'J ERROR'):
            gui.j('invalid command')

    def test_dead_child_cannot_report_success(self):
        class Process:
            def poll(self):
                return -1
        gui = harness.Gui(self.root)
        gui.process = Process()
        with self.assertRaisesRegex(AssertionError, 'exited'):
            gui.j('anything')

    def test_abnormal_shutdown_is_not_ignored(self):
        class Process:
            def poll(self):
                return -1
        gui = harness.Gui(self.root)
        gui.process = Process()
        with self.assertRaisesRegex(AssertionError, 'abnormally'):
            gui.stop()

    def test_invalid_command_oracle_rejects_infrastructure_failures(self):
        from test_table import WdTable
        class BrokenDriver:
            def j(self, code):
                raise AssertionError(self.failure)
        driver = BrokenDriver()
        for failure in ('J command timed out', 'JQt exited before the command completed'):
            driver.failure = failure
            with self.subTest(failure=failure), self.assertRaises(AssertionError):
                WdTable.attempt(driver, 'invalid wd command')
        driver.failure = 'J ERROR: wd invalid parameter'
        self.assertFalse(WdTable.attempt(driver, 'invalid wd command')[0])
