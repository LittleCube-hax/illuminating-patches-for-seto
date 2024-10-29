# Illuminating Patches for Seto (IPS)

I'd hate to compete with ArsonAssassin (as I am very much outmatched) but I wanted to take a crack at this tooling since it was giving Seto so much trouble with something that should be very simple to do, but was somehow being disagreeable.

## Design

This tool takes a different approach than the original: instead of reading the contents of the patch and modifying them directly, this tool aims to _extract_ the contents into a project folder, which can then be edited in a text editor of the user's choice.

## Usage

This tool is currently specialized for use with Final Fantasy V (Japan) for the SNES, (same as Arson's tool) but its scope may be expanded in the future.

If you need to build from source, you only need `gcc`, `make`, and `cmake`.

To use, simply double-click the `decompile` executable when `rpge.ips` is in the same folder. Once you are satisfied with your edits, double-click the `compile` executable.