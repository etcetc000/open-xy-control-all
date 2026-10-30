# Control All

An experimental Open-XY aux machine: eight numbered controls affect the matching
engine parameter slots on all eight instrument tracks. It produces no audio.

## Play

Select the CV aux track, use Shift + M1 and choose **controlall**.
Turn the four encoders for controls 1-4; press any encoder to switch to 5-8.
Both rows remain visible. M1 returns to this screen.

- **M2: Relative** (default). Zero leaves each track unchanged. +10 adds ten
  percentage points of the parameter range to each track's underlying value;
  -10 subtracts ten. Values clamp to their endpoints. The underlying values
  include target-track step locks and live edits; offsets never accumulate.
- **M3: Absolute**. Each track receives the same normalized value (0-100) for
  each of the eight slots. Switching mode immediately reinterprets all eight
  current control positions: the relative zero position becomes absolute 50.
- **Mute this aux track** to hear the underlying instrument settings again.
  Unmute reapplies the controls. M4 is unused; no CV or shared LFO page opens.

Hold a sequencer step and turn an encoder to lock a controller value. Repeat
for each control wanted in that step's snapshot, switching rows as needed.
For a complete eight-control snapshot, lock all eight values on that step.
Unspecified values follow the stock sequencer's usual parameter-lock behavior.

## What snapshots mean

A snapshot is eight controller values stored using stock step locks, not a
capture of 64 separate instrument values. Relative preserves differences
between tracks (until clipping); Absolute gives them matching normalized values.
For example, starting values 20 and 60 become 30 and 70 with relative +10,
or both 40 with absolute 40. Relative follows underlying values as they change.

Mode is saved once per project, not per step or pattern. Changing mode affects
existing controller locks too. A project without saved mode starts in Relative.
Mute/removal releases the overlays; this machine never writes its output into
instrument presets or captures instrument encoder edits.

Parameter numbers are positional, not semantic: slot 1 may control pitch on one
engine and a model selector on another. All eight slots on all eight instrument
tracks are targeted, including unused slots; there is no track mask or mapping
editor. Different ranges are normalized, not matched in Hz or other units.

## Development and validation

Requires the local SDK aux ownership prototype in `../open-xy-aux-ownership`
(branch `experiment/aux-track-ownership`), based on PR #234. Stock staging is
not yet sufficient for its M2/M3 mode actions. No new SDK hooks were added for
Control All. The generated CI workflow still targets staging and is a template
until the required SDK changes are published; this repo is not published.

Run from the SDK checkout:

```sh
python sdk.py test --machine ../open-xy-control-all --no-cpu
```

Host tests cover all-track absolute values, relative offsets and saturation,
non-accumulation, changes to underlying values, mute, project mode persistence,
mode/value captions, row switching, and silent audio output. The SDK preview
checks drawing and ABI; it does not simulate multiple tracks or native step
locks. Control runs on the GUI tick: this is not sample-accurate automation.
Actual sequencer delivery, mute restoration, project reload, and CV/gate
suppression still require device testing. No hardware CPU result is claimed.

Keep firmware/packages outside every checkout. Preserve existing layout IDs
when building. Publication and catalog submission remain pending hardware
validation. See [AGENTS.md](AGENTS.md) and [LICENSE](LICENSE).
