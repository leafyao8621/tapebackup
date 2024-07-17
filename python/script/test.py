import tapebackup

if __name__ == "__main__":
    tapebackup.write("/home/leaf/test", "/home/leaf/dummy.img")
    tapebackup.read(".", "/home/leaf/dummy.img")
    tapebackup.check("/home/leaf/dummy.img")
