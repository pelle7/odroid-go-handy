# go-play
Retro emulation for the ODROID-GO

2019-06-01 Testversion on feature branch

- Quicksave and quickload
- Speedup toggle

The Volume button is defined differently now.

Holding down the volume button freezes the emulator. Now there are two possibilities:

- Releasing the button activates a simple menu
 - LEFT, RIGHT, button A and the VOLUME button are valid actions
- While holding the button
 - Button B: Saves the quicksave
 - Button A: Loads the quicksave
 - UP: Modifies the volume
 - -> A simple text notification is printed the last action

------

This is just a fork containing some simple changes to the original ODROID-GO.
To try it out you can download a pre-compiled FW file from the release folder.

1. More intuitive mapping of the Start/Pause Button for SMS (see: https://forum.odroid.com/viewtopic.php?f=159&t=33470)
2. Changes to the menu buttons from gentleben (see: https://forum.odroid.com/viewtopic.php?f=159&t=31982&p=235847&hilit=go+play#p235847)
3. A nicer Hourglas :)
4. Some more color paletts for the gameboy emulation.

More changes will follow.
