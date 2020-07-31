# longmynd-status-curses

A simple curses based status display for the Longmynd ATV receiver https://github.com/myorangedragon/longmynd

## Initial Installation

The program depends on ncurses so first make sure that is installed; for a Linux distribution using apt the following command should work

    sudo apt install libncurses5-dev

Now clone and build

    git clone https://github.com/dbrooke/longmynd-status-curses.git
    cd longmynd-status-curses
    make

This should result in a program called `status`

## Usage

The `status` program replaces the `fake-read` program supplied with longmynd and likewise expects the fifo to be in the directory from which it is run. This is typically the longmynd directory, so it must be run from there with a path to its actual location e.g.

    ../longmynd-status-curses/status

I typically use GNU screen for my terminal sessions and run the status display in a new virtual terminal with

    screen ../longmynd_status_curses/status

To stop the `status` program just kill it e.g. with `^C`

## Updating

To update to the latest version first change directory to your local repository e.g.

    cd longmynd-status-curses

then update and rebuild

    git pull
    make

## TODO

* Add option to specify a local oscillator offset so that the frequency display shows the real receive frequency when using an LNB (or transverter).
* Decode ES types

