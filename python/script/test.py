import os
import sqlite3

import tapebackup

if __name__ == "__main__":
    tapebackup.write(
        f"{os.getenv('HOME')}/test",
        f"{os.getenv('HOME')}/dummy.img",
        verbose=True
    )
    tapebackup.read(".", f"{os.getenv('HOME')}/dummy.img", verbose=True)
    tapebackup.check(f"{os.getenv('HOME')}/dummy.img", verbose=True)
    conn = sqlite3.connect(f"{os.getenv('HOME')}/.tb.db")
    cur = conn.cursor()
    cur.execute(
        '''
        SELECT
            HEX(SIGNATURE),
            WRITE_PROTECTION,
            HEX(HMAC_KEY),
            FILE_NAME,
            HEX(HMAC_VALUE),
            DATETIME(START_TIME, 'unixepoch'),
            DATETIME(COMPLETION_TIME, 'unixepoch'),
            REPORTED_SIZE,
            WRITTEN_SIZE
        FROM
            MAIN
        '''
    )
    for i in cur:
        print(*i, sep="|")
    cur.close()
    conn.close()
