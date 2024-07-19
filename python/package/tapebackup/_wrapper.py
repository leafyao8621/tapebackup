import tapebackup._base

def write(
    path: str,
    dev: str,
    write_protect=False,
    verbose=False,
    block_size_read: int=65536,
    block_size_write: int=65536,
    block_size_hmac: int=65536,
    block_size_archive: int=65536):
    """_summary_

    Args:
        path (str): _description_
        dev (str): _description_
        write_protect (bool, optional): _description_. Defaults to False.
        verbose (bool, optional): _description_. Defaults to False.
        block_size_read (int, optional): _description_. Defaults to 65536.
        block_size_write (int, optional): _description_. Defaults to 65536.
        block_size_hmac (int, optional): _description_. Defaults to 65536.
        block_size_archive (int, optional): _description_. Defaults to 65536.
    """
    tapebackup._base.write(
        write_protect,
        verbose,
        path,
        dev,
        block_size_read,
        block_size_write,
        block_size_hmac,
        block_size_archive
    )

def read(
    path: str,
    dev: str,
    verbose=False,
    block_size_read: int=65536,
    block_size_write: int=65536,
    block_size_hmac: int=65536,
    block_size_archive: int=65536):
    """_summary_

    Args:
        path (str): _description_
        dev (str): _description_
        verbose (bool, optional): _description_. Defaults to False.
        block_size_read (int, optional): _description_. Defaults to 65536.
        block_size_write (int, optional): _description_. Defaults to 65536.
        block_size_hmac (int, optional): _description_. Defaults to 65536.
        block_size_archive (int, optional): _description_. Defaults to 65536.
    """
    tapebackup._base.read(
        verbose,
        path,
        dev,
        block_size_read,
        block_size_write,
        block_size_hmac,
        block_size_archive
    )

def check(
    dev: str,
    verbose=False,
    block_size_read: int=65536,
    block_size_write: int=65536,
    block_size_hmac: int=65536,
    block_size_archive: int=65536):
    """_summary_

    Args:
        dev (str): _description_
        verbose (bool, optional): _description_. Defaults to False.
        block_size_read (int, optional): _description_. Defaults to 65536.
        block_size_write (int, optional): _description_. Defaults to 65536.
        block_size_hmac (int, optional): _description_. Defaults to 65536.
        block_size_archive (int, optional): _description_. Defaults to 65536.
    """
    tapebackup._base.check(
        verbose,
        dev,
        block_size_read,
        block_size_write,
        block_size_hmac,
        block_size_archive
    )

def report_daily(
    beginning: str="1900-01-01",
    ending: str="2100-01-01",
    format: str="TEXT",
    export_file_name: str | None=None):
    """_summary_

    Args:
        beginning (str, optional): _description_. Defaults to "1900-01-01".
        ending (str, optional): _description_. Defaults to "2100-01-01".
        format (str, optional): _description_. Defaults to "TEXT".
        export_file_name (str | None, optional): _description_. Defaults to None.
    """
    tapebackup._base.report_daily(
        beginning,
        ending,
        format,
        export_file_name
    )

def report_list(
    beginning: str="1900-01-01",
    ending: str="2100-01-01",
    format: str="TEXT",
    export_file_name: str | None=None):
    """_summary_

    Args:
        beginning (str, optional): _description_. Defaults to "1900-01-01".
        ending (str, optional): _description_. Defaults to "2100-01-01".
        format (str, optional): _description_. Defaults to "TEXT".
        export_file_name (str | None, optional): _description_. Defaults to None.
    """
    tapebackup._base.report_list(
        beginning,
        ending,
        format,
        export_file_name
    )

def report_lookup(
    path: str,
    format: str="TEXT",
    export_file_name: str | None=None):
    """_summary_

    Args:
        path (str): _description_
        format (str, optional): _description_. Defaults to "TEXT".
        export_file_name (str | None, optional): _description_. Defaults to None.
    """
    tapebackup._base.report_lookup(
        path,
        format,
        export_file_name
    )
