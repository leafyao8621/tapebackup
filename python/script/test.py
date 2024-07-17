import os

import tapebackup

if __name__ == "__main__":
    tapebackup.write(
        f"{os.getenv('HOME')}/test",
        f"{os.getenv('HOME')}/dummy.img"
    )
    tapebackup.read(".", f"{os.getenv('HOME')}/dummy.img")
    tapebackup.check(f"{os.getenv('HOME')}/dummy.img")
