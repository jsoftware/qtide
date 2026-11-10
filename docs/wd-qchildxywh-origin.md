# `qchildxywh` reports a child's offset twice (D2)

Separate from the table extensions on purpose: this changes the output of an **existing stock command** and is not
needed by them. It can be taken, adjusted or rejected independently (`git revert` of its commit restores the old output).

Source: `lib/wd/wd.cpp`, command `qchildxywh`.

## Original behaviour and defect

```cpp
QPoint pos=cc->widget->mapTo(cc->widget->window(),cc->widget->pos());
```

`QWidget::pos()` is already the widget's position **in its parent**. `mapTo(window, p)` maps a point given in the
**widget's own** coordinates. Passing `pos()` therefore adds the widget's offset a second time: the reported x,y is
`true_origin + pos()`, i.e. roughly twice the real offset for a child whose parent is the form. Width and height are correct.

## Corrected behaviour

```cpp
QPoint pos=cc->widget->mapTo(cc->widget->window(),QPoint(0,0));
```

x y are the widget's top-left in the window's client area (logical pixels), as the command name and its position next
to width/height suggest. Capability token `wd.childxywh.origin` (in `qwdcaps`) is present in builds with this change.

## Minimal reproduction (measured, Windows x64, Qt 6.10.3)

```j
wd 'pc t;pn "repro";bin v'
wd 'cc a table 0 3;cc b table 0 3;pas 0 0;pshow'
wd 'pmove 40 40 900 560'
wd 'qform'            NB. x y w h of the form
wd 'qchildxywh a'
wd 'qchildxywh b'
```

| | `qform` | `qchildxywh a` | `qchildxywh b` | b bottom edge (y+h) |
|---|---|---|---|---|
| before | 40 40 900 560 | 18 18 882 268 | 18 566 882 268 | **834 (> form height 560: impossible)** |
| after  | 40 40 900 560 | 9 9 882 268 | 9 283 882 268 | 551 (inside the form, 9 px bottom margin) |

Independent confirmation: with the old values, posted mouse clicks aimed at the reported table header positions missed
(the second table's header was reported outside the visible form), and a resize-grip had to be targeted ~8 px to the left
of the computed edge. With the corrected values the computed positions are hit exactly.

## Compatibility implications (read before accepting)

* **Absolute x,y returned by existing callers change** (they roughly halve for a child directly in the form). Any
  program that compensated for the old values, or stored them, will see different numbers.
* **Differences between two children's positions also change** (they were doubled before).
* Code that only needs width/height is unaffected.
* The capability token only *identifies* the corrected behaviour. It does **not** make the change backward compatible:
  an application that must run on both builds has to test for the token and handle both outputs.
* Nothing in the table extensions depends on this change; they were first verified using a compensating correction.
* No platform API is involved (same Qt call on Linux/macOS), but only Windows has been tested.
