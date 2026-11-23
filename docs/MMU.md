# XVM Memory and Device Map

## Address space

The implemented address space contains 16 KiB (`0x0000`-`0x3FFF`), although
addresses and the PC are represented with 16 bits. Addresses are byte
addresses; ordinary memory operations transfer one 8-bit byte.

| Address range     |         Size | Region         | Access and behavior                                                                                                                                                                                          |
| ----------------- | ----------- | -------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |
| `0x0000`-`0x0FFF` |        4 KiB | ROM window     | Program load area and instruction fetch region. ROM loading is limited to this range. After a VM ROM is loaded, writes through the I/O bus to this window are ignored while ROM write protection is enabled. |
| `0x1000`-`0x3FFE` | 12,287 bytes | RAM            | Readable and writable byte storage.                                                                                                                                                                          |
| `0x3FFF`          |       1 byte | Input register | Reads the current button bitmask. Writes are ignored.                                                                                                                                                        |

## Input register

| Bit | Mask | Button |
| --- | --- | --- |
| 0 | `0x01` | Up |
| 1 | `0x02` | Down |
| 2 | `0x04` | Left |
| 3 | `0x08` | Right |
| 4 | `0x10` | Action |
| 5-7 | `0xE0` | Unassigned |

The register is read-only from the processor; stores to it have no effect.

## Port-mapped devices
`PORT_OUT` uses a separate 8-bit port-number space; these ports are not memory
addresses. The framebuffer is 64 by 64 pixels by default.

| Port | Name | Write behavior | Read behavior in bus API |
| --- | --- | --- | --- |
| `0x01` | `FB_X` | Set the current X coordinate. | Return current X coordinate. |
| `0x02` | `FB_Y` | Set the current Y coordinate. | Return current Y coordinate. |
| `0x03` | `FB_DRAW` (`FB_COLOR` alias) | Draw at the current X/Y using the value as a palette index. | Not implemented. |
| `0x04` | `FB_CLEAR` | Fill the framebuffer using the value as a palette index. | Not implemented. |

Framebuffer drawing outside its configured dimensions has no effect. If no
framebuffer is attached, writes are ignored with a warning. Unknown or
unsupported ports also produce a warning. 