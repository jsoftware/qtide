"""Run the standalone Windows component suite with explicit runtime inputs."""
import argparse
import os
from pathlib import Path
import tempfile
import unittest


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--jqt-bin', type=Path, required=True)
    parser.add_argument('--stock-bin', type=Path, required=True)
    parser.add_argument('--temp-root', type=Path, required=True,
                        help='Existing writable ASCII directory for private test sessions')
    args = parser.parse_args()
    if os.name != 'nt':
        parser.error('These GUI regressions require Windows and an interactive desktop')
    for runtime in (args.jqt_bin, args.stock_bin):
        for name in ('jqt.exe', 'jqt.dll', 'j.dll', 'profile.ijs', '../system/util/boot.ijs'):
            if not (runtime/name).is_file():
                parser.error(f'Incomplete J runtime: {runtime/name}')
    root = args.temp_root.resolve()
    if not root.is_dir() or not str(root).isascii():
        parser.error('--temp-root must be an existing ASCII directory')
    tempfile.tempdir = str(root)
    os.environ['JQT_TEST_BIN'] = str(args.jqt_bin.resolve())
    os.environ['JQT_TEST_STOCK_BIN'] = str(args.stock_bin.resolve())
    suite = unittest.defaultTestLoader.discover(str(Path(__file__).parent), pattern='test_*.py')
    result = unittest.TextTestRunner(verbosity=2).run(suite)
    allowed = [reason for test, reason in result.skipped
               if test.id() == 'test_table.WdTable.test_15_tooltip_popup_appears_on_real_hover'
               and 'manual' in reason.lower()]
    return 0 if result.wasSuccessful() and len(allowed) == len(result.skipped) else 1


if __name__ == '__main__':
    raise SystemExit(main())
