# Updater for CISS465NetworkGame

The directory that this document is located in contains the scripts used to
update CISS465NetworkGame for all connected clients if needed.

Following are the instructions for steps involved in the procedure.

## Prerequisites

1. All interested clients are using the pre-distributed F18-SpaceGame-VM.
   Note that it is hosted over at [Google Docs.](https://docs.google.com/TBD)
2. The scripts "spacegame" and "updater.sh" in the aforementioned VM have not
   been tampered with.
3. The developers are adhering to the update model assumed in this document.


## Client Side instructions for playing the game
Simply enter the following in the terminal:

    $ /home/student/Desktop/SpaceGame/spacegame

Or go through the following sequence of clicks from student's Desktop:
`SpaceGame > spacegame

Note that this checks for available updates as well.


## Client Side instructions for updating the game only
Simply enter the following in the terminal:

    $ /home/student/Desktop/SpaceGame/updater.sh

Or go through the following sequence of clicks from student's Desktop:
`SpaceGame > updater.sh


## Developer Side instructions
Please don't move the directory containing this document (and it's contents)
from it's current location. Please don't remove the lines updating the
TimeStamp in the makefile for the client. 


## How it works
This is a very basic solution that only downloads the latest version of a
script that further downloads *whatever* as needed. For now, the script simply
downloads the whole client folder and removes source. At the very least, we
probably want to set up a distribution branch and put only resources necessary
for the client there and have the updater script download only those.
Thankfully, we can change how the updater works and the clients will never
need to know.
