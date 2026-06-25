# buttLock
Watchapp for Pebble to lock the buttons

This Watchapp locks all buttons (ignores their presses).

In order to exit the app, long press the back (upper left) button.

Get it at the official Pebble AppStore (https://apps.rebble.io/en_US/application/54fad867c2eca30d2a000016?query=butt&section=watchapps).

Unfortunately, it is not possible to override the long press back event, otherwise it would be possible to create some code (e.g. left-right-left) to unlock the watch.

The display is inspired by the Simplicity Watchface (https://github.com/pebble/pebble-sdk-examples/tree/master/watchfaces/simplicity) and the Button Lock Watchapp (https://apps.getpebble.com/applications/543f02a145b1f75605000010).

## Supported platforms

- Pebble / Pebble Steel (`aplite`)
- Pebble Time / Pebble Time Steel (`basalt`)
- Pebble Time Round (`chalk`)
- Pebble 2 (`diorite`)
- Pebble 2 Duo (`flint`, requires PebbleOS 4.9.127 or newer)
- Pebble Time 2 (`emery`)
- Pebble Round 2 (`gabbro`)

## Building

Build with Docker (no local SDK install required):

```bash
make docker-verify
```

This pulls `ghcr.io/skylord123/docker-coredevices-pebble-tool`, builds the app, and checks that platform binaries are present in `build/pebble.pbw`.

`gabbro` (Pebble Round 2) requires Pebble SDK 4.9.127 or newer. The current Docker image ships SDK 4.9.77, which builds the other six platforms. On PebbleOS 4.x, the `chalk` binary is scaled automatically on Round 2 watches. For a native `gabbro` binary in the `.pbw`, build with a newer SDK.

Alternatively, install the [Rebble SDK](https://developer.rebble.io/) locally:

```bash
uv tool install pebble-tool --python 3.13
pebble sdk install latest
```

Build and install on an emulator:

```bash
pebble build
pebble install --emulator aplite
```

Replace `aplite` with any supported platform (`basalt`, `chalk`, `diorite`, `flint`, `emery`, `gabbro`).

If you like this WatchApp you can Flattr it (https://flattr.com/submit/auto?user_id=x29a&url=https%3A%2F%2Fgithub.com%2Fx29a%2FbuttLock).
