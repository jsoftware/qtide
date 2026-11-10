"""Standalone Windows JQt GUI driver: private profiles and PID-scoped posted input.
No application imports, databases, clipboard access, real cursor movement or runtime discovery.
"""
import ctypes
from ctypes import wintypes as w
import os
from pathlib import Path
import subprocess
import time

WM_MOUSEMOVE, WM_LBUTTONDOWN, WM_LBUTTONUP = 0x200, 0x201, 0x202
WM_KEYDOWN, WM_KEYUP = 0x100, 0x101
if os.name == 'nt':
    u = ctypes.WinDLL('user32', use_last_error=True)
    callback = ctypes.WINFUNCTYPE(w.BOOL, w.HWND, w.LPARAM)
    u.EnumWindows.argtypes = [callback, w.LPARAM]
    u.GetWindowTextW.argtypes = [w.HWND, w.LPWSTR, ctypes.c_int]
    u.GetWindowThreadProcessId.argtypes = [w.HWND, ctypes.POINTER(w.DWORD)]
    u.PostMessageW.argtypes = [w.HWND, w.UINT, w.WPARAM, w.LPARAM]
    u.GetWindowRect.argtypes = [w.HWND, ctypes.POINTER(w.RECT)]
else:
    u = None


def jq(path):
    return "'" + str(path).replace('\\', '/').replace("'", "''") + "'"


def isolated_command(jbin, executable, script, session):
    """Copy the supplied J profile/boot, isolate its home, disable startup/profilex.

    Only a recognized library profile is accepted. The runtime itself is never edited.
    """
    jbin, session, script = Path(jbin).resolve(), Path(session).resolve(), Path(script).resolve()
    if not str(session).isascii():
        raise ValueError('Use an ASCII --temp-root for the J bootstrap')
    home = session/'home'
    home.mkdir(exist_ok=True)
    boot = (jbin.parent/'system/util/boot.ijs').read_text(encoding='utf-8')
    profile = (jbin/'profile.ijs').read_text(encoding='utf-8')
    startup = "f=. jpath '~config/startup.ijs'"
    hook = "0!:0 <jpathsep system_:,'/util/boot.ijs'"
    home_hook = "home=. >(systype-5){(2!:5'HOME');2!:5'USERPROFILE'"
    overrides = [line for line in profile.splitlines() if '/profilex.ijs' in line]
    if boot.count(startup) != 1 or profile.count(hook) != 1 or profile.count(home_hook) != 1 or len(overrides) != 1:
        raise RuntimeError('Unrecognized J library boot/profile hooks; isolation cannot be guaranteed')
    (session/'boot.ijs').write_text(boot.replace(startup, "f=. ''"), encoding='utf-8')
    profile = profile.replace(home_hook, 'home=. ' + jq(home))
    profile = profile.replace(overrides[0], 'NB. profilex deliberately disabled by the standalone test driver')
    profile = profile.replace(hook, '0!:0 <' + jq(session/'boot.ijs'))
    private_profile = session/'profile.ijs'
    private_profile.write_text(profile, encoding='utf-8')
    sentence = 'load a.{~' + ' '.join(map(str, script.as_posix().encode('utf-8')))
    return [str(jbin/executable), '-jprofile', str(private_profile), '-js', sentence]


def clean_environment(jbin):
    env = {k: v for k, v in os.environ.items()
           if not k.startswith('JQT_TEST_')
           and k not in ('PYTHONHOME', 'PYTHONPATH', 'QT_PLUGIN_PATH', 'QT_QPA_PLATFORM_PLUGIN_PATH')}
    env['QT_PLUGIN_PATH'] = str(jbin)
    env['QT_QPA_PLATFORM_PLUGIN_PATH'] = str(Path(jbin)/'platforms')
    return env


def top_windows():
    found = []
    @callback
    def visit(hwnd, _):
        title, pid = ctypes.create_unicode_buffer(512), w.DWORD()
        u.GetWindowTextW(hwnd, title, len(title))
        u.GetWindowThreadProcessId(hwnd, ctypes.byref(pid))
        found.append((hwnd, title.value, pid.value))
        return True
    u.EnumWindows(visit, 0)
    return found


def rect(hwnd):
    value = w.RECT()
    if not u.GetWindowRect(hwnd, ctypes.byref(value)):
        raise ctypes.WinError(ctypes.get_last_error())
    return value.left, value.top, value.right, value.bottom


def click(hwnd, x, y):
    for msg, button in ((WM_MOUSEMOVE, 0), (WM_LBUTTONDOWN, 1), (WM_LBUTTONUP, 0)):
        u.PostMessageW(hwnd, msg, button, (y << 16) | (x & 0xffff))


def drag(hwnd, x0, y, x1, steps=6):
    u.PostMessageW(hwnd, WM_MOUSEMOVE, 0, (y << 16) | (x0 & 0xffff))
    u.PostMessageW(hwnd, WM_LBUTTONDOWN, 1, (y << 16) | (x0 & 0xffff))
    for index in range(1, steps + 1):
        x = x0 + (x1 - x0) * index // steps
        u.PostMessageW(hwnd, WM_MOUSEMOVE, 1, (y << 16) | (x & 0xffff))
        time.sleep(.03)
    u.PostMessageW(hwnd, WM_LBUTTONUP, 0, (y << 16) | (x1 & 0xffff))


class Gui:
    def __init__(self, base):
        self.base = Path(base)
        self.process = None

    def until(self, predicate, timeout=40, message='timed out'):
        deadline = time.monotonic() + timeout
        while time.monotonic() < deadline:
            try:
                result = predicate()
            except (FileNotFoundError, PermissionError):
                result = None
            if result:
                return result
            time.sleep(.05)
        raise AssertionError(message)

    def j(self, code, timeout=30):
        if self.process.poll() is not None:
            raise AssertionError('JQt exited before the command completed')
        out = self.base/'cmd.out'
        out.unlink(missing_ok=True)
        script = self.base/'cmd.ijs'
        script.with_suffix('.tmp').write_text("cocurrent 'base'\n" + code + '\n', encoding='utf-8')
        os.replace(script.with_suffix('.tmp'), script)
        self.until(lambda: out.exists(), timeout, 'J command timed out: ' + code[:80])
        text = self.until(lambda: self.read_result(out), 5, 'result remained locked')
        if text[0].startswith('J ERROR'):
            raise AssertionError(text[0] + '\n' + code)
        return text[0]

    @staticmethod
    def read_result(out):
        return (out.read_text(encoding='utf-8'),)

    def stop(self):
        if self.process is None:
            return
        status = self.process.poll()
        if status is not None:
            if status != 0:
                raise AssertionError(f'JQt exited abnormally: {status}')
            return
        forced = False
        try:
            self.j("wd 'timer 0;psel t;pclose'\nRESULT_z_=:''", timeout=8)
            for hwnd, title, pid in top_windows():
                if pid == self.process.pid and title == 'Term':
                    u.PostMessageW(hwnd, 0x10, 0, 0)
            self.process.wait(timeout=10)
        except (AssertionError, subprocess.TimeoutExpired):
            forced = True
            self.process.terminate()
            try:
                self.process.wait(timeout=5)
            except subprocess.TimeoutExpired:
                self.process.kill()
                self.process.wait(timeout=5)
        if forced or self.process.returncode != 0:
            raise AssertionError('JQt did not shut down normally; private process reaped')
