import os

import tapebackup

if __name__ == "__main__":
    tapebackup.write(
        f"{os.getenv('HOME')}/test",
        f"{os.getenv('HOME')}/dummy.img",
        verbose=True
    )
    tapebackup.read(".", f"{os.getenv('HOME')}/dummy.img", verbose=True)
    tapebackup.check(f"{os.getenv('HOME')}/dummy.img", verbose=True)
    tapebackup.report_daily("2024-07-01", "2024-08-01")
    tapebackup.report_daily("2024-07-01", "2024-08-01", format="CSV")
    tapebackup.report_daily(
        "2024-07-01", "2024-08-01", export_file_name="daily.txt")
    tapebackup.report_daily(
        "2024-07-01", "2024-08-01", format="CSV", export_file_name="daily.csv")
    tapebackup.report_list("2024-07-01", "2024-08-01")
    tapebackup.report_list("2024-07-01", "2024-08-01", format="CSV")
    tapebackup.report_list(
        "2024-07-01", "2024-08-01", export_file_name="list.txt")
    tapebackup.report_list(
        "2024-07-01", "2024-08-01", format="CSV", export_file_name="list.csv")
    tapebackup.report_lookup("tes")
    tapebackup.report_lookup("tes", format="CSV")
    tapebackup.report_lookup(
        "tes", export_file_name="lookup.txt")
    tapebackup.report_lookup(
        "tes", format="CSV", export_file_name="lookup.csv")
    tapebackup.report_transaction("2024-07-01", "2024-08-01")
    tapebackup.report_transaction("2024-07-01", "2024-08-01", format="CSV")
    tapebackup.report_transaction(
        "2024-07-01", "2024-08-01", export_file_name="transaction.txt")
    tapebackup.report_transaction(
        "2024-07-01",
        "2024-08-01",
        format="CSV",
        export_file_name="transaction.csv")
